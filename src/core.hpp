#pragma once
#include <dinput.h>

#include "scssdk_telemetry.h"
#include "fmt/core.h"

#include "graphics/dx11_hook.hpp"
#include "input/di8_hook.hpp"
#include "managers/hooks_manager.hpp"

namespace ts_extra_utilities
{
    class CWindowManager;

    namespace prism
    {
        class base_ctrl_u;
        class game_actor_u;
    };

    class CCore
    {
    private:
        const scs_telemetry_init_params_v101_t* init_params_;
        scs_log_t scs_log_;

        CDirectX11Hook* dx11_hook = nullptr;
        CDirectInput8Hook* di8_hook = nullptr;

        CWindowManager* window_manager_;
        CHooksManager* hooks_manager_;

        float last_mouse_pos_x_ = 500;
        float last_mouse_pos_y_ = 500;

        uint64_t base_ctrl_instance_ptr_address = 0;
        uint32_t game_actor_offset_in_base_ctrl = 0;

        bool truckersmp_ = false;

    public:
        static CCore* g_instance;

        bool disable_in_game_mouse = false;
        bool render_ui = false;

        CCore( const scs_telemetry_init_params_v101_t* init_params );
        ~CCore();

        bool init();
        void destroy();

        void tick();
        void telemetry_config( const scs_telemetry_configuration_t* );

        bool on_mouse_input( LPDIDEVICEOBJECTDATA );
        bool render();
        void toggle_input_hook();
        void toggle_ui();
        bool on_wnd_proc( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam );

        bool is_truckersmp() const
        {
            return this->truckersmp_;
        }

        CHooksManager* get_hooks_manager() const { return this->hooks_manager_; }

        prism::base_ctrl_u* get_base_ctrl_instance();
        prism::game_actor_u* get_game_actor();

        // TODO: change to file only or something
        template < class... T >
        void debug( const char* fmt_s, T&&... args ) const
        {
#ifdef _DEBUG
            scs_log_( 0, fmt::vformat( std::string( "[extra_utils] " ) + fmt_s, fmt::make_format_args( args... ) ).c_str() );
#endif
        }

        template < class... T >
        void info( const char* fmt_s, T&&... args ) const
        {
            scs_log_( 0, fmt::vformat( std::string( "[extra_utils] " ) + fmt_s, fmt::make_format_args( args... ) ).c_str() );
        }

        template < class... T >
        void warning( const char* fmt_s, T&&... args ) const
        {
            scs_log_( 1, fmt::vformat( std::string( "[extra_utils] " ) + fmt_s, fmt::make_format_args( args... ) ).c_str() );
        }

        template < class... T >
        void error( const char* fmt_s, T&&... args ) const
        {
            scs_log_( 2, fmt::vformat( std::string( "[extra_utils] " ) + fmt_s, fmt::make_format_args( args... ) ).c_str() );
        }
    };
}
