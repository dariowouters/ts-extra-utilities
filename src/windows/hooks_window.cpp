#include "hooks_window.hpp"

#include "core.hpp"
#include "imgui.h"
#include "managers/hooks_manager.hpp"

namespace ts_extra_utilities
{
    void CHooksWindow::render()
    {
        ImGui::Begin( "Hooks" );
        const auto hooks = CCore::g_instance->get_hooks_manager()->get_hooks();
        ImGui::Text( "Hooks: %u", hooks.size() );

        for ( const auto& [ name, status ] : hooks )
        {
            ImGui::Text( "%s", name.c_str() );
            ImGui::SameLine();
            ImVec4 color = { 0.8f, 0, 0, 1.0f };
            auto status_text = "Failed";
            if ( status == CHook::CREATED )
            {
                color = { 0.6f, 0.6f, 0.6f, 1.0f };
                status_text = "Created";
            }
            else if ( status == CHook::UNHOOKED )
            {
                color = { 0.2f, 0.2f, 0.2f, 0.6f };
                status_text = "Removed";
            }
            else if ( status == CHook::HOOKED )
            {
                color = { 0, 0.8f, 0.0f, 1.0f };
                status_text = "Enabled";
            }
            ImGui::TextColored( color, "%s", status_text );
        }

        ImGui::End();
    }
}
