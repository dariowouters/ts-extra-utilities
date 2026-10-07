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
                ImGui::PushID( i );
                ImGui::TableNextRow();

                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                ImGui::TextUnformatted( settings::action_name( action ) );

                ImGui::TableNextColumn();
                const auto label = settings::is_capturing( action )
                                       ? std::string( "Press a key... (Esc cancels)" )
                                       : settings::key_name( settings::key( action ) );
                if ( ImGui::Button( fmt::format( "{}##bind", label ).c_str(), ImVec2( -FLT_MIN, 0 ) ) )
                {
                    settings::begin_capture( action );
                }

                ImGui::TableNextColumn();
                ImGui::BeginDisabled( settings::key( action ) == 0 );
                if ( ImGui::SmallButton( "Clear" ) )
                {
                    settings::g_settings.keys[ i ] = 0;
                    settings::save();
                }
                ImGui::EndDisabled();

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
