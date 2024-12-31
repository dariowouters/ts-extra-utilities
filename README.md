# ts-extra-utilities

> [!WARNING]
> This is more of a PoC(Proof of Concept) and very much WIP(Work In Progress), it is not extensively tested or in any way a stable finished product.
> If you decide to use this, expect possible issues / crashes (especially after game updates)

A plugin for ATS/ETS2 to add some experimental extra functionality.

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

## How to use

Currently only works with DirectX11.

Made for singleplayer, **NOT** recommended in multiplayer.


Download the [latest release](https://github.com/dariowouters/ts-extra-utilities/releases/latest), copy the `ts-extra-utilities.dll` to `<game_install_location>/bin/win_x64/plugins`  
(if the plugins folder does not exists, you can create one)

Then in-game you can toggle the UI with `delete` and `insert` to toggle the cursor.  
(Keybinds are currently hardcoded, will make it changable at some point.)
