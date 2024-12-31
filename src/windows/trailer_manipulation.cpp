#include "trailer_manipulation.hpp"

#include "imgui.h"

#include "core.hpp"
#include "hooks/function_hook.hpp"
#include "hooks/vtable_hook.hpp"
#include "prism/controllers/base_ctrl.hpp"
#include "prism/game_actor.hpp"
#include "prism/physics/physics_actor_t.hpp"
#include "prism/vehicles/accessories/data/accessory_chassis_data.hpp"

namespace ts_extra_utilities
{
    struct TrailerJointState
    {
        enum Enum
        {
            NORMAL,
            LOCKED,
            DISCONNECTED,
        };
    };

    thread_local bool locked_trailers[ 20 ] = {};

    thread_local TrailerJointState::Enum trailer_joints[ 20 ] = {};

    thread_local std::shared_ptr< CVirtualFunctionHook > steering_advance_hook = nullptr;
    thread_local std::shared_ptr< CFunctionHook > crashes_when_disconnected_hook = nullptr;
    thread_local std::shared_ptr< CFunctionHook > connect_slave_hook = nullptr;

    /**
     * \brief Hook for prism::physics_trailer_u::steering_advance so we can control which trailer can be steered by the game
     * \param self /
     * \return /
     */
    uint64_t hk_steering_advance( prism::physics_trailer_u* self )
    {
        int trailer_index = 0;
        const auto* check_trailer = CCore::g_instance->get_game_actor()->game_trailer_actor;
        while ( check_trailer != nullptr && self != check_trailer )
        {
            check_trailer = check_trailer->slave_trailer;
            ++trailer_index;
        }

        if ( check_trailer == nullptr || locked_trailers[ trailer_index ] == false )
        {
            return steering_advance_hook->get_original< prism::physics_trailer_u_steering_advance_fn >()( self );
        }
        return 0;
    }

    // TODO: Check what this original function actually did
    // original function crashes when a slave trailer is disconnected due to it expecting the physics joint to exist when there is a slave trailer
    // and the joint is not there when we have the trailer disconnected
    void hk_crashes_when_disconnected( prism::physics_trailer_u* self, prism::game_trailer_actor_u* trailer_actor )
    {
        // original_crash_fn(self, trailer_actor);
    }

    /**
     * \brief Hook to stop slave trailers from getting automatically reconnected when we attach their parent
     * \param _ unused
     */
    void hk_connect_slave( prism::physics_trailer_u* _ )
    {
        // original_connect_slave(self);
    }

    CTrailerManipulation::CTrailerManipulation() = default;

    CTrailerManipulation::~CTrailerManipulation()
    {
        steering_advance_hook.reset();
        crashes_when_disconnected_hook.reset();
        connect_slave_hook.reset();
    }

    bool CTrailerManipulation::init()
    {
        this->set_individual_steering_fn_ = memory::get_function_from_pattern< prism::set_individual_steering_fn >( "48 89 5c 24 08 48 89 74 24 10 57 48 83 ec ? 8b 41 ? 48 8b d9 0f 29 74", 0 );

        if ( this->set_individual_steering_fn_ == nullptr )
        {
            CCore::g_instance->error( "Could not find 'set_individual_steering' function" );
        }
        else
        {
            CCore::g_instance->debug( "Found set_individual_steering function @ +{:x}", memory::as_offset( reinterpret_cast< uint64_t >( this->set_individual_steering_fn_ ) ) );
        }

        const auto crash_fn_address = memory::get_address_for_pattern( "48 85 d2 0f 84 ? ? ? ? 48 89 74 24 18 57 48 83 ec 40" );

        crashes_when_disconnected_hook = CCore::g_instance->get_hooks_manager()->register_function_hook(
            "crashes_when_disconnected",
            crash_fn_address,
            reinterpret_cast< uint64_t >( &hk_crashes_when_disconnected ) );

        if ( !CCore::g_instance->is_truckersmp() )
        {
            crashes_when_disconnected_hook->hook();
        }

        const auto connect_slave_address = memory::get_address_for_pattern( "40 53 48 83 ec 60 48 83 b9 ? ? ? ? 00 48 8b d9 0f 84 ? ? ? ? 48 8d 54 24 ? e8" );

        connect_slave_hook = CCore::g_instance->get_hooks_manager()->register_function_hook(
            "prism::physics_trailer_u::connect_slave",
            connect_slave_address,
            reinterpret_cast< uint64_t >( &hk_connect_slave ) );
        if ( !CCore::g_instance->is_truckersmp() )
        {
            connect_slave_hook->create();
        }

        this->get_slave_hook_position_fn_ = reinterpret_cast< prism::physics_trailer_u_get_slave_hook_position_fn* >(
            connect_slave_address + 29 + *reinterpret_cast< int32_t* >( connect_slave_address + 29 ) + 4 );

        this->valid_ = true;
        return this->valid_;
    }

    void CTrailerManipulation::render_trailer_steering( prism::game_trailer_actor_u* current_trailer, uint32_t i ) const
    {
        if ( ImGui::Checkbox( "Locked##steering", &locked_trailers[ i ] ) )
        {
            CCore::g_instance->info( "{} steering for {}", locked_trailers[ i ] ? "Locking" : "Unlocking", i );
        }
        ImGui::BeginDisabled( !locked_trailers[ i ] );
        if ( ImGui::SliderFloat( "Angle", &current_trailer->steering, -1.0f, 1.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp ) )
        {
            CCore::g_instance->info( "Changed steering angle for trailer {} to {}", i, current_trailer->steering );
            this->set_individual_steering_fn_( current_trailer->wheel_steering_stuff, current_trailer->steering );
        }

        ImGui::PushItemFlag( ImGuiItemFlags_ButtonRepeat, true );

        if ( ImGui::ArrowButton( "rotate_left", ImGuiDir_Left ) )
        {
            current_trailer->steering -= 0.02f;
            if ( current_trailer->steering < -1.f )
            {
                current_trailer->steering = -1.f;
            }
            this->set_individual_steering_fn_( current_trailer->wheel_steering_stuff, current_trailer->steering );
        }
        ImGui::SameLine();
        if ( ImGui::Button( "center" ) )
        {
            current_trailer->steering = 0.f;
            this->set_individual_steering_fn_( current_trailer->wheel_steering_stuff, current_trailer->steering );
        }
        ImGui::SameLine();

        if ( ImGui::ArrowButton( "rotate_right", ImGuiDir_Right ) )
        {
            current_trailer->steering += 0.02f;
            if ( current_trailer->steering > 1.f )
            {
                current_trailer->steering = 1.f;
            }
            this->set_individual_steering_fn_( current_trailer->wheel_steering_stuff, current_trailer->steering );
        }

        ImGui::PopItemFlag();

        ImGui::EndDisabled();
    }

    void CTrailerManipulation::connect_trailer( prism::game_trailer_actor_u* current_trailer, const uint32_t i ) const
    {
        // disable the function that automatically connects slave trailers
        if ( connect_slave_hook->hook() != CHook::HOOKED )
        {
            CCore::g_instance->error( "Could not enable 'connect_slave' hook in 'connect_trailer'" );
            return;
        }
        auto* last_connected_trailer = CCore::g_instance->get_game_actor()->get_last_trailer_connected_to_truck();

        if ( last_connected_trailer == nullptr )
        {
            auto* truck = CCore::g_instance->get_game_actor()->game_physics_vehicle;
            const float3_t vec{
                truck->accessory_chassis_data->hook_position.x - truck->hook_locator.x,
                truck->accessory_chassis_data->hook_position.y - truck->hook_locator.y,
                truck->accessory_chassis_data->hook_position.z - truck->hook_locator.z
            };
            current_trailer->connect( truck, vec, 0, true, false );
            current_trailer->set_trailer_brace( false );
        }
        else
        {
            float3_t slave_hook_position{};
            get_slave_hook_position_fn_( last_connected_trailer, &slave_hook_position );
            const float3_t vec{
                slave_hook_position.x - last_connected_trailer->hook_locator.x,
                slave_hook_position.y - last_connected_trailer->hook_locator.y,
                slave_hook_position.z - last_connected_trailer->hook_locator.z
            };
            current_trailer->connect( last_connected_trailer, vec, 0, true, false );
            current_trailer->set_trailer_brace( false );
        }

        if ( connect_slave_hook->unhook() != CHook::CREATED )
        {
            CCore::g_instance->error( "Could not disable 'connect_slave' hook in 'connect_trailer'" );
        }
    }

    void CTrailerManipulation::render_trailer_joint( prism::game_trailer_actor_u* current_trailer, const uint32_t i ) const
    {
        ImGui::SeparatorText( "Joint" );
        if ( CCore::g_instance->get_base_ctrl_instance()->selected_physics_engine == 1 ) // PhysX
        {
            if ( current_trailer->physics_joint != nullptr && current_trailer->physics_joint->px_joint != nullptr )
            {
                if ( ImGui::RadioButton( "Unlocked##joint", trailer_joints[ i ] == TrailerJointState::NORMAL ) )
                {
                    if ( trailer_joints[ i ] == TrailerJointState::DISCONNECTED )
                    {
                        this->connect_trailer( current_trailer, i );
                    }

                    trailer_joints[ i ] = TrailerJointState::NORMAL;
                    current_trailer->physics_joint->px_joint->setMotion( physx::PxD6Axis::eTWIST, physx::PxD6Motion::eFREE );
                }
                ImGui::SameLine();
                if ( ImGui::RadioButton( "Locked##joint", trailer_joints[ i ] == TrailerJointState::LOCKED ) )
                {
                    if ( trailer_joints[ i ] == TrailerJointState::DISCONNECTED )
                    {
                        this->connect_trailer( current_trailer, i );
                    }

                    trailer_joints[ i ] = TrailerJointState::LOCKED;
                    current_trailer->physics_joint->px_joint->setMotion( physx::PxD6Axis::eTWIST, physx::PxD6Motion::eLOCKED );
                }
            }
        }
        else
        {
            ImGui::TextWrapped( "Ability to lock joints is only available with PhysX" );
        }

        ImGui::SeparatorText( "Connect/Disconnect" );
        // nothing in this plugin is recommended to be used in TruckersMP but this is completely broken when used in TruckersMP and WILL get you banned, so I've explicitly disabled it.
        if ( !CCore::g_instance->is_truckersmp() )
        {
            ImGui::BeginDisabled( current_trailer->physics_joint != nullptr );
            if ( ImGui::Button( "Connect##trailer" ) )
            {
                this->connect_trailer( current_trailer, i );
                trailer_joints[ i ] = TrailerJointState::NORMAL;
            }
            ImGui::EndDisabled();
            ImGui::SameLine();
            ImGui::BeginDisabled( current_trailer->physics_joint == nullptr );
            if ( ImGui::Button( "Disconnect##trailer" ) )
            {
                current_trailer->set_trailer_brace( true );
                current_trailer->disconnect();
                trailer_joints[ i ] = TrailerJointState::DISCONNECTED;
            }
            ImGui::EndDisabled();
        }
        else
        {
            ImGui::TextWrapped( "Individually detachable trailers does not work in TruckersMP" );
        }
    }

    void CTrailerManipulation::render_trailers() const
    {
        if (
            CCore::g_instance->get_game_actor() == nullptr ||
            CCore::g_instance->get_game_actor()->game_trailer_actor == nullptr )
        {
            ImGui::Text( "No trailers found" );
            return;
        }

        auto* current_trailer = CCore::g_instance->get_game_actor()->game_trailer_actor;

        if ( steering_advance_hook == nullptr )
        {
            const auto steering_advance_address = *reinterpret_cast< uint64_t* >( current_trailer ) + 0x08 * 73;
            steering_advance_hook = CCore::g_instance->get_hooks_manager()->register_virtual_function_hook(
                "physics_trailer_u::steering_advance",
                steering_advance_address,
                reinterpret_cast< uint64_t >( &hk_steering_advance )
            );


            if ( steering_advance_hook->hook() != CHook::HOOKED )
            {
                CCore::g_instance->error( "Could not hook the physics_trailer_u::steering_advance virtual function" );
            }
        }

        int i = 0;
        do
        {
            const auto trailer_name = fmt::format( "Trailer {}", i );
            ImGui::PushID( trailer_name.c_str() );
            if ( ImGui::CollapsingHeader( trailer_name.c_str(), ImGuiTreeNodeFlags_DefaultOpen ) )
            {
                if ( current_trailer->wheel_steering_stuff != nullptr && set_individual_steering_fn_ != nullptr )
                {
                    ImGui::SeparatorText( "Steering" );
                    this->render_trailer_steering( current_trailer, i );
                }
                this->render_trailer_joint( current_trailer, i );
            }
            ImGui::PopID();

            current_trailer = current_trailer->slave_trailer;
            ++i;
        }
        while ( current_trailer != nullptr );
    }

    void CTrailerManipulation::render()
    {
        ImGui::Begin( "Trailer Manipulation"/*, &this->open_ */ );

        this->render_trailers();

        ImGui::End();
    }
}
