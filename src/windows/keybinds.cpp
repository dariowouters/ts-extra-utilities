#include "keybinds.hpp"

#include "imgui.h"
#include "fmt/core.h"

#include "settings/settings.hpp"

namespace ts_extra_utilities
{
    bool CKeybinds::init()
    {
        return true;
    }

    void CKeybinds::render()
    {
        ImGui::Begin( "Keybinds" );

        if ( ImGui::BeginTable( "keybinds", 3, ImGuiTableFlags_SizingStretchProp ) )
        {
            for ( int i = 0; i < settings::action_count; ++i )
            {
                const auto action = static_cast< settings::Action >( i );
                const bool fixed = action == settings::Action::TOGGLE_UI || action == settings::Action::TOGGLE_CURSOR;
                ImGui::PushID( i );
                ImGui::TableNextRow();

                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                ImGui::TextUnformatted( settings::action_name( action ) );

                ImGui::TableNextColumn();
                if ( fixed )
                {
                    // rebindable in settings.ini only — keeping them off the
                    // capture path makes the plugin unreachable harder
                    ImGui::TextDisabled( "%s (settings.ini)", settings::key_name( settings::key( action ) ).c_str() );
                }
                else
                {
                    const auto label = settings::is_capturing( action )
                                           ? std::string( "Press a key... (Esc cancels)" )
                                           : settings::key_name( settings::key( action ) );
                    if ( ImGui::Button( fmt::format( "{}##bind", label ).c_str(), ImVec2( -FLT_MIN, 0 ) ) )
                    {
                        settings::begin_capture( action );
                    }
                }

                ImGui::TableNextColumn();
                if ( !fixed )
                {
                    ImGui::BeginDisabled( settings::key( action ) == 0 );
                    if ( ImGui::SmallButton( "Clear" ) )
                    {
                        settings::g_settings.keys[ i ] = 0;
                        settings::save();
                    }
                    ImGui::EndDisabled();
                }

                ImGui::PopID();
            }
            ImGui::EndTable();
        }

        ImGui::Separator();
        ImGui::SliderFloat( "Steering speed", &settings::g_settings.steering_speed, 0.1f, 2.f, "%.2f / s", ImGuiSliderFlags_AlwaysClamp );
        if ( ImGui::IsItemDeactivatedAfterEdit() ) // once on release, not every drag frame
        {
            settings::save();
        }
        ImGui::TextDisabled( "Steering keys act on every trailer with steerable wheels." );

        ImGui::End();
    }
}
