#pragma once
#include <string>
#include <Windows.h>

namespace ts_extra_utilities::settings
{
    enum class Action
    {
        TOGGLE_UI,
        TOGGLE_CURSOR,
        STEER_LEFT,
        STEER_RIGHT,
        STEER_CENTER,
        TOGGLE_STEERING_LOCK,
        COUNT,
    };

    constexpr int action_count = static_cast< int >( Action::COUNT );

    struct Settings
    {
        UINT keys[ action_count ] = {}; // virtual-key codes, 0 = unbound
        float steering_speed = 0.5f; // angle units per second while a steer key is held (full lock = 1)
    };

    extern Settings g_settings;

    // %APPDATA%\ts-extra-utilities\settings.ini, shared by ETS2 and ATS
    void load();
    void save();

    UINT key( Action action );
    const char* action_name( Action action );
    std::string key_name( UINT vk );

    // Rebinding: while capturing, the next key press is taken by capture_key() instead of the game.
    void begin_capture( Action action );
    bool is_capturing();
    bool is_capturing( Action action );
    // Returns true when the key was consumed. Escape cancels, anything else becomes the binding.
    bool capture_key( UINT vk );
}
