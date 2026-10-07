#include "trailer_manipulation.hpp"

#include <algorithm>
#include <atomic>

#include "imgui.h"

#include "core.hpp"
#include "hooks/function_hook.hpp"
#include "memory/memory_utils.hpp"
#include "prism/offsets.hpp"
#include "settings/settings.hpp"

namespace ts_extra_utilities
{
    constexpr uint32_t max_trailers = 20;

    // Shared between the render thread (UI) and the game thread (the hook). The hook only
    // compares the pointer, it never dereferences it, so a stale entry is harmless.
    struct LockedSteering
    {
        std::atomic< prism::vehicle_wheel_steering_data_t* > data{ nullptr };
        std::atomic< float > angle{ 0.f };
    };

    LockedSteering locked_steering[ max_trailers ];

    // UI-only state, render thread
    bool locked_ui[ max_trailers ] = {};
    float angle_ui[ max_trailers ] = {};

    std::shared_ptr< CFunctionHook > set_individual_steering_hook = nullptr;

    /**
     * \brief Hook for set_individual_steering, which the game calls every steering advance with the
     * angle it computed from the joint. For a trailer the user took control of, swap in their angle.
     *
     * Before 1.61 this feature skipped physics_trailer_u::steering_advance entirely, but that function
     * now also runs a per-wheel update after setting the angle, so only the angle is replaced here.
     */
    void hk_set_individual_steering( prism::vehicle_wheel_steering_data_t* data, float angle )
    {
        for ( auto& locked : locked_steering )
        {
            if ( data != nullptr && locked.data.load( std::memory_order_relaxed ) == data )
            {
                angle = locked.angle.load( std::memory_order_relaxed );
                break;
            }
        }
        set_individual_steering_hook->get_original< prism::set_individual_steering_fn >()( data, angle );
    }

    void release_all()
    {
        for ( uint32_t i = 0; i < max_trailers; ++i )
        {
            locked_steering[ i ].data.store( nullptr );
            locked_ui[ i ] = false;
        }
    }

    CTrailerManipulation::CTrailerManipulation() = default;

    CTrailerManipulation::~CTrailerManipulation()
    {
        release_all();
        set_individual_steering_hook.reset();
    }

    bool CTrailerManipulation::init()
    {
        const auto address = memory::get_address_for_pattern( "48 89 5c 24 08 48 89 74 24 10 57 48 83 ec ? 8b 41 ? 48 8b d9 0f 29 74" );

        if ( address == 0 )
        {
            CCore::g_instance->error( "Could not find 'set_individual_steering' function" );
            return false;
        }
        CCore::g_instance->debug( "Found set_individual_steering function @ +{:x}", memory::as_offset( address ) );

        set_individual_steering_hook = CCore::g_instance->get_hooks_manager()->register_function_hook(
            "set_individual_steering",
            address,
            reinterpret_cast< uint64_t >( &hk_set_individual_steering ) );

        if ( set_individual_steering_hook->hook() != CHook::HOOKED )
        {
            CCore::g_instance->error( "Could not hook 'set_individual_steering'" );
            return false;
        }

        this->valid_ = true;
        return this->valid_;
    }


    // The current trailer chain, refreshed by update() every frame on the render thread.
    void* trailers[ max_trailers ] = {};
    uint32_t trailer_count = 0;

    bool held[ settings::action_count ] = {};

    prism::vehicle_wheel_steering_data_t* steering_data( void* trailer )
    {
        return prism::offsets::field< prism::vehicle_wheel_steering_data_t* >( trailer, prism::offsets::trailer_steering_data );
    }

    void set_locked( const uint32_t i, const bool lock )
    {
        if ( locked_ui[ i ] == lock ) return;
        locked_ui[ i ] = lock;
        CCore::g_instance->info( "{} steering for {}", lock ? "Locking" : "Unlocking", i );
        if ( lock )
        {
            // start from where the game had the wheels so they do not jump
            angle_ui[ i ] = prism::offsets::field< float >( trailers[ i ], prism::offsets::trailer_steering );
            locked_steering[ i ].angle.store( angle_ui[ i ] );
        }
    }

    bool game_has_focus()
    {
        DWORD pid = 0;
        GetWindowThreadProcessId( GetForegroundWindow(), &pid );
        return pid == GetCurrentProcessId();
    }

    // true while the bound key is down; first_press only on the frame it went down
    bool key_down( const settings::Action action, bool* first_press = nullptr )
    {
        const auto vk = settings::key( action );
        const bool down = vk != 0 && ( GetAsyncKeyState( static_cast< int >( vk ) ) & 0x8000 ) != 0;
        bool& was = held[ static_cast< int >( action ) ];
        if ( first_press != nullptr ) *first_press = down && !was;
        was = down;
        return down;
    }

    void CTrailerManipulation::update( const float dt )
    {
        if ( !this->valid_ ) return;

        auto* game_actor = CCore::g_instance->get_game_actor();
        void* trailer = game_actor == nullptr ? nullptr : prism::offsets::field< void* >( game_actor, prism::offsets::game_actor_trailer );

        trailer_count = 0;
        for ( ; trailer != nullptr && trailer_count < max_trailers; ++trailer_count )
        {
            trailers[ trailer_count ] = trailer;
            trailer = prism::offsets::field< void* >( trailer, prism::offsets::trailer_slave );
        }

        // nothing to steer on a trailer without steerable wheels, or one that is gone
        for ( uint32_t i = 0; i < max_trailers; ++i )
        {
            if ( i >= trailer_count || steering_data( trailers[ i ] ) == nullptr )
            {
                locked_ui[ i ] = false;
            }
        }

        // hotkeys, ignored while the game is in the background or a key is being rebound
        if ( game_has_focus() && !settings::is_capturing() )
        {
            bool lock_pressed = false, center_pressed = false;
            key_down( settings::Action::TOGGLE_STEERING_LOCK, &lock_pressed );
            key_down( settings::Action::STEER_CENTER, &center_pressed );
            const bool left = key_down( settings::Action::STEER_LEFT );
            const bool right = key_down( settings::Action::STEER_RIGHT );

            bool any_locked = false;
            for ( uint32_t i = 0; i < trailer_count; ++i ) any_locked |= locked_ui[ i ];

            for ( uint32_t i = 0; i < trailer_count; ++i )
            {
                if ( steering_data( trailers[ i ] ) == nullptr ) continue;

                if ( lock_pressed )
                {
                    set_locked( i, !any_locked );
                }
                else if ( left || right || center_pressed )
                {
                    set_locked( i, true ); // steering a trailer means taking control of it
                }

                if ( !locked_ui[ i ] ) continue;

                if ( center_pressed ) angle_ui[ i ] = 0.f;
                const float step = settings::g_settings.steering_speed * dt;
                if ( left ) angle_ui[ i ] = ( std::max )( angle_ui[ i ] - step, -1.f );
                if ( right ) angle_ui[ i ] = ( std::min )( angle_ui[ i ] + step, 1.f );
                locked_steering[ i ].angle.store( angle_ui[ i ] );
            }
        }

        // Published every frame: the data pointer changes when trailers are swapped.
        for ( uint32_t i = 0; i < max_trailers; ++i )
        {
            locked_steering[ i ].data.store( locked_ui[ i ] ? steering_data( trailers[ i ] ) : nullptr );
            if ( i < trailer_count && !locked_ui[ i ] )
            {
                angle_ui[ i ] = prism::offsets::field< float >( trailers[ i ], prism::offsets::trailer_steering );
            }
        }
    }

    void CTrailerManipulation::render_trailer_steering( const uint32_t i ) const
    {
        bool lock = locked_ui[ i ];
        if ( ImGui::Checkbox( "Locked##steering", &lock ) )
        {
            set_locked( i, lock );
        }

        ImGui::BeginDisabled( !locked_ui[ i ] );
        bool changed = ImGui::SliderFloat( "Angle", &angle_ui[ i ], -1.0f, 1.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp );

        ImGui::PushItemFlag( ImGuiItemFlags_ButtonRepeat, true );

        if ( ImGui::ArrowButton( "rotate_left", ImGuiDir_Left ) )
        {
            angle_ui[ i ] = ( std::max )( angle_ui[ i ] - 0.02f, -1.f );
            changed = true;
        }
        ImGui::SameLine();
        if ( ImGui::Button( "center" ) )
        {
            angle_ui[ i ] = 0.f;
            changed = true;
        }
        ImGui::SameLine();
        if ( ImGui::ArrowButton( "rotate_right", ImGuiDir_Right ) )
        {
            angle_ui[ i ] = ( std::min )( angle_ui[ i ] + 0.02f, 1.f );
            changed = true;
        }

        ImGui::PopItemFlag();
        ImGui::EndDisabled();

        if ( changed )
        {
            // applied by the game's next steering advance, on its own thread
            locked_steering[ i ].angle.store( angle_ui[ i ] );
        }
    }

    void CTrailerManipulation::render()
    {
        ImGui::Begin( "Trailer Manipulation" );

        if ( !this->valid_ )
        {
            ImGui::TextWrapped( "Trailer steering is unavailable for this game version, see the game log." );
        }
        else if ( trailer_count == 0 )
        {
            ImGui::Text( "No trailers found" );
        }
        else
        {
            for ( uint32_t i = 0; i < trailer_count; ++i )
            {
                const auto trailer_name = fmt::format( "Trailer {}", i );
                ImGui::PushID( trailer_name.c_str() );
                if ( ImGui::CollapsingHeader( trailer_name.c_str(), ImGuiTreeNodeFlags_DefaultOpen ) )
                {
                    if ( steering_data( trailers[ i ] ) != nullptr )
                    {
                        ImGui::SeparatorText( "Steering" );
                        this->render_trailer_steering( i );
                    }
                    else
                    {
                        ImGui::TextWrapped( "No steerable wheels" );
                    }
                }
                ImGui::PopID();
            }
        }

        ImGui::End();
    }
}
