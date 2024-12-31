#include "window_manager.hpp"

#include "imgui.h"

namespace ts_extra_utilities
{
    bool CWindowManager::init()
    {
        return true;
    }

    void CWindowManager::render() const
    {
        // TODO: Add taskbar
        // ImGui::Begin( "##window_manager", nullptr, ImGuiWindowFlags_NoTitleBar );
        //
        // for (const auto& window : this->windows_)
        // {
        //
        // }
        //
        // ImGui::End();

        for ( const auto& window : this->windows_ )
        {
            window->render();
        }
    }
}
