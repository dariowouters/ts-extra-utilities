#include "core.hpp"

#include <MinHook.h>

#include "consts.hpp"

#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_win32.h"
#include "memory/memory_utils.hpp"

#include "managers/window_manager.hpp"
#include "windows/trailer_manipulation.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND, UINT, WPARAM, LPARAM );

namespace ts_extra_utilities
{
    CCore* CCore::g_instance = nullptr;

    CCore::CCore( const scs_telemetry_init_params_v101_t* init_params ) : init_params_( init_params )
    {
        this->hooks_manager_ = new CHooksManager();
        this->window_manager_ = new CWindowManager();
        scs_log_ = init_params->common.log;
        g_instance = this;
    }

    CCore::~CCore()
    {
        this->destroy();
        MH_RemoveHook( nullptr );
    }

    bool CCore::init()
    {
        MH_Initialize();
        this->info( "Initializing {}", VERSION );
        truckersmp_ = GetModuleHandle( L"core_ets2mp.dll" ) != nullptr || GetModuleHandle( L"core_atsmp.dll" ) != nullptr;

        this->dx11_hook = new CDirectX11Hook();
        if ( !this->dx11_hook->hook_present() )
        {
            return false;
        }
        this->di8_hook = new CDirectInput8Hook();
        if ( !this->di8_hook->hook() )
        {
            return false;
        }

        const auto trailer_manipulation = this->window_manager_->register_window( std::make_shared< CTrailerManipulation >() );

        if ( !trailer_manipulation->init() )
        {
            g_instance->error( "Could not initialize the trailer manipulation module" );
        }

        return true;
    }

    void CCore::destroy()
    {
        delete this->dx11_hook;
        delete this->di8_hook;
        delete this->hooks_manager_;
        delete this->window_manager_;
    }

    bool CCore::on_mouse_input( LPDIDEVICEOBJECTDATA rgdod )
    {
        if ( !this->disable_in_game_mouse ) return false;

        auto& io = ImGui::GetIO();
        if ( rgdod->dwOfs == DIMOFS_X )
        {
            io.MousePos.x += static_cast< float >( static_cast< int >( rgdod->dwData ) );
        }
        else if ( rgdod->dwOfs == DIMOFS_Y )
        {
            io.MousePos.y += static_cast< float >( static_cast< int >( rgdod->dwData ) );
        }

        return true;
    }

    bool CCore::render()
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        if ( this->render_ui )
        {
#ifdef _DEBUG
            bool s = true;
            ImGui::ShowDemoWindow( &s );
#endif
            this->window_manager_->render();
        }
        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData() );
        return true;
    }

    void CCore::toggle_input_hook()
    {
        this->disable_in_game_mouse = !this->disable_in_game_mouse;
        auto& io = ImGui::GetIO();
        if ( this->disable_in_game_mouse )
        {
            io.MousePos.x = last_mouse_pos_x_;
            io.MousePos.y = last_mouse_pos_y_;
        }
        else
        {
            last_mouse_pos_x_ = io.MousePos.x;
            last_mouse_pos_y_ = io.MousePos.y;
        }

        io.MouseDrawCursor = this->disable_in_game_mouse;

        this->debug( "Mouse hook is now {}", this->disable_in_game_mouse ? "active" : "disabled" );
    }

    void CCore::toggle_ui()
    {
        this->render_ui = !this->render_ui;
    }

    // TODO: add keybind settings
    bool CCore::on_wnd_proc( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam )
    {
        if ( umsg == WM_KEYDOWN )
        {
            if ( wparam == VK_INSERT )
            {
                toggle_input_hook();
                return true;
            }
            if ( wparam == VK_DELETE )
            {
                toggle_ui();
                return true;
            }
        }
        else if ( umsg == WM_MOUSEMOVE )
        {
            if ( this->disable_in_game_mouse )
            {
                return true;
            }
        }

        if ( ImGui_ImplWin32_WndProcHandler( hwnd, umsg, wparam, lparam ) )
        {
            return true;
        }
        return false;
    }

    prism::base_ctrl_u* CCore::get_base_ctrl_instance()
    {
        if ( this->base_ctrl_instance_ptr_address != 0 ) return *reinterpret_cast< prism::base_ctrl_u** >( this->base_ctrl_instance_ptr_address );

        const auto addr = memory::get_address_for_pattern( "48 8b 05 ? ? ? ? 48 8b 4b ? 48 8b 80 ? ? ? ? 48 8b b9" );

        if ( addr == 0 ) return nullptr;
        this->base_ctrl_instance_ptr_address = addr + *reinterpret_cast< int32_t* >( addr + 3 ) + 7;
        this->game_actor_offset_in_base_ctrl = *reinterpret_cast< int32_t* >( addr + 14 );

        this->info( "Found base_ctrl @ +{:x}", memory::as_offset( this->base_ctrl_instance_ptr_address ) );

        return *reinterpret_cast< prism::base_ctrl_u** >( this->base_ctrl_instance_ptr_address );
    }

    prism::game_actor_u* CCore::get_game_actor()
    {
        auto* base_ctrl = this->get_base_ctrl_instance();

        if ( base_ctrl == nullptr || this->game_actor_offset_in_base_ctrl == 0 )
        {
            return nullptr;
        }

        return *reinterpret_cast< prism::game_actor_u** >( reinterpret_cast< uint64_t >( base_ctrl ) + this->game_actor_offset_in_base_ctrl );
    }
}
