1.2.0
    - Configurable keybinds (Keybinds window), saved to %APPDATA%\ts-extra-utilities\settings.ini
    - New hotkeys: steer left / right, center, lock / unlock trailer steering (unbound by default)
    - Adjustable steering speed for the hotkeys

1.1.1
    - DirectX 11 Present is now inline-hooked (plus Present1) instead of vtable-swapped, so the overlay also shows on flip-model swapchains
    - Logs "Present hook is live, overlay initialized" once the overlay is up

1.1.0
    - Updated manual trailer steering for game version 1.61 (ETS2 + ATS)
    - Trailer steering now only overrides the steering angle instead of skipping the whole steering update
    - Detachable trailers and lockable joints are disabled until their offsets are re-verified

1.0.1
    - Fixed DirectX hook not working on some devices

1.0.0
    - Initial Version
