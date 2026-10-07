#include "settings.hpp"

#include <ShlObj.h>
#include <filesystem>

#include "fmt/core.h"

namespace ts_extra_utilities::settings
{
    Settings g_settings;

    namespace
    {
        constexpr UINT default_keys[ action_count ] = {
            VK_DELETE, // TOGGLE_UI
            VK_INSERT, // TOGGLE_CURSOR
            0, // STEER_LEFT
            0, // STEER_RIGHT
            0, // STEER_CENTER
            0, // TOGGLE_STEERING_LOCK
        };

        // ini keys, kept stable so saved files keep working if display names change
        constexpr const wchar_t* ini_keys[ action_count ] = {
            L"toggle_ui",
            L"toggle_cursor",
            L"steer_left",
            L"steer_right",
            L"steer_center",
            L"toggle_steering_lock",
        };

        constexpr const char* names[ action_count ] = {
            "Toggle window",
            "Toggle cursor",
            "Steer left",
            "Steer right",
            "Center steering",
            "Lock / unlock steering",
        };

        int capturing = -1;

        std::filesystem::path settings_path()
        {
            PWSTR appdata = nullptr;
            std::filesystem::path path;
            if ( SUCCEEDED( SHGetKnownFolderPath( FOLDERID_RoamingAppData, 0, nullptr, &appdata ) ) )
            {
                path = std::filesystem::path( appdata ) / L"ts-extra-utilities" / L"settings.ini";
            }
            CoTaskMemFree( appdata );
            return path;
        }
    }

    void load()
    {
        for ( int i = 0; i < action_count; ++i )
        {
            g_settings.keys[ i ] = default_keys[ i ];
        }

        const auto path = settings_path();
        if ( path.empty() ) return;

        for ( int i = 0; i < action_count; ++i )
        {
            g_settings.keys[ i ] = GetPrivateProfileIntW( L"keybinds", ini_keys[ i ], static_cast< INT >( default_keys[ i ] ), path.c_str() );
        }

        wchar_t speed[ 32 ] = {};
        GetPrivateProfileStringW( L"steering", L"speed", L"", speed, 32, path.c_str() );
        if ( speed[ 0 ] != 0 )
        {
            const float value = static_cast< float >( _wtof( speed ) );
            if ( value > 0.f && value <= 5.f ) g_settings.steering_speed = value;
        }
    }

    void save()
    {
        const auto path = settings_path();
        if ( path.empty() ) return;

        std::error_code ec;
        std::filesystem::create_directories( path.parent_path(), ec );

        for ( int i = 0; i < action_count; ++i )
        {
            WritePrivateProfileStringW( L"keybinds", ini_keys[ i ], std::to_wstring( g_settings.keys[ i ] ).c_str(), path.c_str() );
        }
        WritePrivateProfileStringW( L"steering", L"speed", std::to_wstring( g_settings.steering_speed ).c_str(), path.c_str() );
    }

    UINT key( const Action action )
    {
        return g_settings.keys[ static_cast< int >( action ) ];
    }

    const char* action_name( const Action action )
    {
        return names[ static_cast< int >( action ) ];
    }

    std::string key_name( const UINT vk )
    {
        if ( vk == 0 ) return "(unbound)";

        LONG scan = static_cast< LONG >( MapVirtualKeyW( vk, MAPVK_VK_TO_VSC ) );
        switch ( vk )
        {
            // these share scan codes with the numpad and need the extended bit to get their own name
            case VK_INSERT: case VK_DELETE: case VK_HOME: case VK_END: case VK_PRIOR: case VK_NEXT:
            case VK_LEFT: case VK_RIGHT: case VK_UP: case VK_DOWN: case VK_DIVIDE: case VK_NUMLOCK:
            case VK_RCONTROL: case VK_RMENU:
                scan |= 0x100;
                break;
            default:
                break;
        }

        char name[ 64 ] = {};
        if ( scan != 0 && GetKeyNameTextA( scan << 16, name, sizeof( name ) ) > 0 )
        {
            return name;
        }
        return fmt::format( "Key 0x{:02X}", vk );
    }

    void begin_capture( const Action action )
    {
        capturing = static_cast< int >( action );
    }

    bool is_capturing()
    {
        return capturing >= 0;
    }

    bool is_capturing( const Action action )
    {
        return capturing == static_cast< int >( action );
    }

    bool capture_key( const UINT vk )
    {
        if ( capturing < 0 ) return false;

        // modifiers alone are not useful bindings; wait for a real key
        if ( vk == VK_SHIFT || vk == VK_CONTROL || vk == VK_MENU ) return true;

        if ( vk != VK_ESCAPE )
        {
            // a key can only do one thing: take it away from any other action
            for ( auto& bound : g_settings.keys )
            {
                if ( bound == vk ) bound = 0;
            }
            g_settings.keys[ capturing ] = vk;
            save();
        }
        capturing = -1;
        return true;
    }
}
