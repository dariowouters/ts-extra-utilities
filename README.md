# ts-extra-utilities

> [!WARNING]
> This is more of a PoC(Proof of Concept) and very much WIP(Work In Progress), it is not extensively tested or in any way a stable finished product.
> If you decide to use this, expect possible issues / crashes (especially after game updates)

A plugin for ATS/ETS2 to add some experimental extra functionality.

**Compatible with ETS2/ATS version 1.59+**

## Current features

 - Manually steerable trailer wheels
    - Abilty to take control of the steerable wheels on a trailer

   [Preview video](https://youtu.be/0kRavShaXy0)

 - Individually detachable trailers
    - Ability to disconnect and connect trailers individually and out of order
    - Still has multiple issues when trailers are disconnected, trailer cables, 3rd person camera, ...

   [Preview video](https://youtu.be/Nu6YvKKSL2g)

 - Lockable trailer joints (PhysX only, `-physx` as a game launch parameter in Steam)
    - Ability to lock the joints between vehicle and trailers so you can stop them from pivoting

   [Preview video](https://youtu.be/zXtlzMVNEXM)

## System Requirements

- **DirectX 11+** (required)
- **Windows x64** (64-bit only)
- **ETS2/ATS Version 1.59 or higher**
- **Visual C++ 2022 Runtime**

## How to use

Currently only works with DirectX11.

Made for singleplayer, **NOT** recommended in multiplayer.

### Installation

1. Download the [latest release](https://github.com/dariowouters/ts-extra-utilities/releases/latest)
2. Copy the `ts-extra-utilities.dll` to your game installation folder:
   ```
   <game_install_location>/bin/win_x64/plugins
   ```
   (If the plugins folder does not exist, create it)
3. Launch the game and verify the plugin loads

### Controls

- `Delete` - Toggle UI visibility
- `Insert` - Toggle cursor capture

*(Keybinds are currently hardcoded, configurable support coming soon)*

## Building from Source

### Requirements

- Visual Studio 2022 or later
- CMake 3.15+
- Windows 10+ SDK

### Build Steps

```bash
# Clone the repository
git clone https://github.com/dariowouters/ts-extra-utilities.git
cd ts-extra-utilities

# Create build directory
mkdir build
cd build

# Generate project files
cmake .. -G "Visual Studio 17 2022" -A x64

# Build the plugin
cmake --build . --config Release
```

The compiled DLL will be in `build/x64-Release/`.

## Troubleshooting

**Plugin not loading?**
- Verify the DLL is in the correct plugins folder
- Check that you're running the game in DirectX11 mode
- Ensure your Windows version has required Visual C++ 2022 Runtime

**Crashes or performance issues?**
- This is a PoC - report issues on [GitHub Issues](https://github.com/dariowouters/ts-extra-utilities/issues)
- Try disabling some features to isolate the problem
- Update to the latest version

## License

MIT License - See [LICENSE.md](LICENSE.md) for details

## Credits

- DirectX11 hooking powered by [MinHook](https://github.com/TsudaKageyu/minhook)
- UI rendering with [ImGui](https://github.com/ocornut/imgui)
- String formatting with [fmt](https://github.com/fmtlib/fmt)
- SCS SDK for ETS2/ATS plugin development
