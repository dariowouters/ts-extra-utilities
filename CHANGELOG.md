2.0
    - Individually detachable trailers, ported to 1.61.1.9: per-trailer
      Connect/Disconnect buttons and keybinds for trailers 2-3 (the first trailer
      stays on the game's own T key). Disconnect drops the landing gear and the
      joint; Connect re-attaches the trailer to whatever is ahead of it in the
      chain. Singleplayer only
    - "Attach trailer to truck": connect a rear trailer straight to the fifth wheel
      — park a trailer at a dock, haul the rest, come back and pick it up. Refused
      while the fifth wheel is occupied
    - Manual connect/disconnect replays the full native sequence: towed-trailer
      bookkeeping, parked flag, brace, air line rebuild and wheel-load
      distribution; the truck's coupling state (+0x14E0) is marked coupled so the
      game releases the trailer's brakes
    - The parked set can no longer re-brake a trailer sitting on the truck: the
      brake setter is hooked and positive writes aimed at a truck-towed trailer
      are swallowed, while the pedal brake still works
    - connect_slave stays permanently hooked but only refuses trailers detached
      through this plugin — the game no longer rejoins them when the head trailer
      is detached with T, and normal attach/spawn keeps working
    - g_trailer_cables_mode follows the coupled convoy automatically: 1 while the
      first trailer is coupled, 2 once the last trailer is coupled, 0 with nothing
    - Overlay font slightly enlarged for 4K readability; Insert releases a
      centered cursor
    - Diagnostic dump per trailer (joint, brake channels, connector, truck towed
      pointer) and brake setter call tracing in game.log.txt

1.2.2
    - Trailer steering offsets updated for game version 1.61.1.9 (the trailer object grew by
      0x28 bytes: steering angle 0x690 -> 0x6B8, steering data 0x698 -> 0x6C0, slave trailer
      0x1060 -> 0x1088; first trailer in game_actor is still 0xC8)

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
