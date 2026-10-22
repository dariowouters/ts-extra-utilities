#include "settings.hpp"

#include <ShlObj.h>
#include <filesystem>
#include <utility>

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
            0, // ATTACH_DETACH_2
            0, // ATTACH_DETACH_3
            0, // ATTACH_TRUCK_2
            0, // ATTACH_TRUCK_3
        };

        // ini keys, kept stable so saved files keep working if display names change
        constexpr const wchar_t* ini_keys[ action_count ] = {
            L"toggle_ui",
            L"toggle_cursor",
            L"steer_left",
            L"steer_right",
            L"steer_center",
            L"toggle_steering_lock",
            L"attach_detach_2",
            L"attach_detach_3",
            L"attach_truck_2",
            L"attach_truck_3",
        };

        constexpr const char* names[ action_count ] = {
            "Toggle window",
            "Toggle cursor",
            "Steer left",
            "Steer right",
            "Center steering",
            "Lock / unlock steering",
            "Attach / detach trailer 1",
            "Attach / detach trailer 2",
            "Attach trailer 1 to truck",
            "Attach trailer 2 to truck",
        };

        int capturing = -1;

        // friendly key names accepted in settings.ini — a plain number still
        // works and stays the authoritative write format
        UINT key_from_name( const wchar_t* text )
        {
            if ( text == nullptr || text[ 0 ] == 0 ) return UINT_MAX;

            if ( text[ 0 ] >= L'0' && text[ 0 ] <= L'9' )
            {
                const long v = _wtol( text );
                return v > 0 && v <= 0xFE ? static_cast< UINT >( v ) : UINT_MAX;
            }

            wchar_t name[ 32 ] = {};
            size_t i = 0;
            for ( ; text[ i ] != 0 && i < 31; ++i ) name[ i ] = towupper( text[ i ] );
            while ( i > 0 && name[ i - 1 ] == L' ' ) name[ --i ] = 0;

            if ( name[ 0 ] == L'F' && name[ 1 ] >= L'1' && name[ 1 ] <= L'9' )
            {
                const long f = _wtol( name + 1 );
                if ( f >= 1 && f <= 24 ) return VK_F1 + f - 1;
            }
            if ( name[ 0 ] != 0 && name[ 1 ] == 0 &&
                ( ( name[ 0 ] >= L'A' && name[ 0 ] <= L'Z' ) || ( name[ 0 ] >= L'0' && name[ 0 ] <= L'9' ) ) )
            {
                return name[ 0 ]; // letters/digits: VK code is the ASCII char
            }

            static const std::pair< const wchar_t*, UINT > table[] = {
                { L"INSERT", VK_INSERT }, { L"DELETE", VK_DELETE }, { L"HOME", VK_HOME },
                { L"END", VK_END }, { L"PAGEUP", VK_PRIOR }, { L"PAGEDOWN", VK_NEXT },
                { L"UP", VK_UP }, { L"DOWN", VK_DOWN }, { L"LEFT", VK_LEFT }, { L"RIGHT", VK_RIGHT },
                { L"SPACE", VK_SPACE }, { L"TAB", VK_TAB }, { L"ENTER", VK_RETURN },
                { L"RETURN", VK_RETURN }, { L"BACKSPACE", VK_BACK }, { L"ESCAPE", VK_ESCAPE },
                { L"NUMPAD0", VK_NUMPAD0 }, { L"NUMPAD1", VK_NUMPAD1 }, { L"NUMPAD2", VK_NUMPAD2 },
                { L"NUMPAD3", VK_NUMPAD3 }, { L"NUMPAD4", VK_NUMPAD4 }, { L"NUMPAD5", VK_NUMPAD5 },
                { L"NUMPAD6", VK_NUMPAD6 }, { L"NUMPAD7", VK_NUMPAD7 }, { L"NUMPAD8", VK_NUMPAD8 },
                { L"NUMPAD9", VK_NUMPAD9 }, { L"MINUS", VK_OEM_MINUS }, { L"EQUALS", VK_OEM_PLUS },
                { L"COMMA", VK_OEM_COMMA }, { L"PERIOD", VK_OEM_PERIOD },
            };
            for ( const auto& entry : table )
            {
                if ( wcscmp( name, entry.first ) == 0 ) return entry.second;
            }
            return UINT_MAX;
        }

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

        // a ts-settings.ini shipped next to the dll lets a release preset
        // binds out of the box — used only while the user has no own file yet.
        // The ts- prefix keeps it from colliding with other plugins' inis
        // sharing the same plugins folder
        std::filesystem::path plugin_dir_settings_path()
        {
            HMODULE self = nullptr;
            if ( !GetModuleHandleExW( GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                                         GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                                     reinterpret_cast< LPCWSTR >( &plugin_dir_settings_path ),
                                     &self ) )
            {
                return {};
            }
            wchar_t dir[ MAX_PATH ] = {};
            GetModuleFileNameW( self, dir, MAX_PATH );
            return std::filesystem::path( dir ).parent_path() / L"ts-settings.ini";
        }
    }

    void load()
    {
        for ( int i = 0; i < action_count; ++i )
        {
            g_settings.keys[ i ] = default_keys[ i ];
        }

        auto path = settings_path();
        if ( path.empty() ) return;

        const auto bundled = plugin_dir_settings_path();
        const bool has_bundled = !bundled.empty() && std::filesystem::exists( bundled );

        // first run: a shipped ts-settings.ini next to the dll seeds the binds
        if ( has_bundled && !std::filesystem::exists( path ) ) path = bundled;

        for ( int i = 0; i < action_count; ++i )
        {
            wchar_t value[ 32 ] = {};
            GetPrivateProfileStringW( L"keybinds", ini_keys[ i ], L"", value, 32, path.c_str() );
            const UINT parsed = key_from_name( value );
            if ( parsed != UINT_MAX ) g_settings.keys[ i ] = parsed;
        }

        // the window/cursor keys can only be changed through a file, so when a
        // ts-settings.ini sits next to the dll it governs those two even when
        // the user's own ini already exists — everything else still comes
        // from the user file
        if ( has_bundled && bundled != path )
        {
            for ( const int i : { static_cast< int >( Action::TOGGLE_UI ), static_cast< int >( Action::TOGGLE_CURSOR ) } )
            {
                wchar_t value[ 32 ] = {};
                GetPrivateProfileStringW( L"keybinds", ini_keys[ i ], L"", value, 32, bundled.c_str() );
                const UINT parsed = key_from_name( value );
                if ( parsed != UINT_MAX ) g_settings.keys[ i ] = parsed;
            }
        }

        // the two critical keys stay rebindable through the ini, but an
        // unbound/garbage value or a collision would leave the plugin
        // unreachable — fall back to the defaults instead
        const int ui = static_cast< int >( Action::TOGGLE_UI );
        const int cursor = static_cast< int >( Action::TOGGLE_CURSOR );
        if ( g_settings.keys[ ui ] == 0 || g_settings.keys[ cursor ] == 0 ||
            g_settings.keys[ ui ] == g_settings.keys[ cursor ] )
        {
            g_settings.keys[ ui ] = VK_DELETE;
            g_settings.keys[ cursor ] = VK_INSERT;
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

        // whatever currently opens the window / releases the cursor is
        // reserved — stealing it here would leave the plugin unreachable
        if ( vk == key( Action::TOGGLE_UI ) || vk == key( Action::TOGGLE_CURSOR ) )
        {
            capturing = -1;
            return true;
        }

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
