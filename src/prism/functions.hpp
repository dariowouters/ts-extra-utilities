#pragma once
#include <cstdint>

namespace ts_extra_utilities
{
    struct float3_t;

    namespace prism
    {
        class vehicle_shared_u;
        class game_trailer_actor_u;
        class physics_trailer_u;
        class vehicle_wheel_steering_data_t;

        using set_individual_steering_fn = void __fastcall( vehicle_wheel_steering_data_t*, float );
        using physics_trailer_u_steering_advance_fn = uint64_t __fastcall( physics_trailer_u* );

        using crashes_when_disconnected_fn = void __fastcall( physics_trailer_u*, game_trailer_actor_u* );
        using physics_trailer_u_connect_slave_fn = uint64_t __fastcall( physics_trailer_u* );

        using physics_trailer_u_get_slave_hook_position_fn = void __fastcall( physics_trailer_u*, float3_t* );

        // called through the vtable (see prism::offsets::vfunc_*). this/arg types in
        // reality are physics_trailer_u* / vehicle_shared_u*; float3 is passed by value
        // like the original code, MSVC lowers it to a hidden pointer
        using physics_trailer_u_connect_fn = void __fastcall( void*, void*, float3_t, int, bool, bool );
        using physics_trailer_u_disconnect_fn = void __fastcall( void* );
        using physics_trailer_u_set_trailer_brace_fn = void __fastcall( void*, bool );

        // Free functions the native attach/detach flow (T key) calls around the
        // connect/disconnect vfuncs. Replayed by manual connects so a trailer gets
        // fully registered: towed-trailer bookkeeping on the parent, the detached-
        // state flag, air/electric line rebuild, towed-mass distribution onto the
        // parent's wheels and the game_actor axle-lift refresh.
        using vehicle_set_towed_trailer_fn = void __fastcall( void* /*vehicle*/, bool, void* /*trailer*/ );
        using vehicle_clear_detached_flag_fn = void __fastcall( void* /*vehicle*/, uint8_t, uint8_t );
        using vehicle_rebuild_lines_fn = void __fastcall( void* /*front vehicle*/, void* /*trailer*/ );
        using vehicle_distribute_towed_mass_fn = void __fastcall( void* /*vehicle*/, float, float );
        using game_actor_refresh_axle_lift_fn = void __fastcall( void* /*game_actor*/ );

        // Component-notification pair the native T attach/detach calls on BOTH
        // vehicles around the connect/disconnect vfuncs: every component on the
        // vehicle's +0xE98 list (cargo constraints, cable accessories, joints)
        // gets told the coupling state changed. Without these calls a loaded
        // trailer's cargo constraints keep stale coupling state and the physics
        // sweep aborts on them.
        using vehicle_notify_coupled_fn = void __fastcall( void* /*vehicle*/, uint8_t, uint8_t );
        using vehicle_notify_decoupled_fn = void __fastcall( void* /*vehicle*/ );
        // truck-side joint release the native detach runs BEFORE the disconnect
        // vfunc while the truck's coupling state is still nonzero
        using vehicle_pre_detach_fn = void __fastcall( void* /*truck*/, void* /*trailer*/ );

        // accessory event refresh the native attach AND detach run on the trailer
        // right after the brace setter: fires coupling events at every accessory
        // on the trailer's +0x158/+0x10A8 lists (cargo, traffic-obstacle anchors).
        // Skipping it leaves a parked trailer's traffic lane modifier unanchored
        using vehicle_refresh_accessories_fn = void __fastcall( void* /*parent vehicle*/, void* /*trailer*/ );

        // the native detach binds the parked trailer's traffic-obstacle anchor:
        // the trailer actor's provider (actor+0x1F0) vfunc+0x28 hands over the
        // anchor object which then gets registered at the vehicle's transform.
        // Without it the lane modifier keeps a null owner and the traffic sweep
        // dereferences it whenever AI traffic comes near the parked trailer
        using vehicle_get_transform_fn = void* __fastcall( void* /*vehicle*/, void* /*out32*/ );
        using provider_get_anchor_fn = void* __fastcall( void* /*provider*/ );
        using traffic_register_anchor_fn = void __fastcall( void* /*anchor*/, const void* /*transform32*/, void* /*ctx*/ );

        // console line executor: rcx is a char** holding the command text and the
        // game passes -1 as the queue context for direct execution
        using console_execute_line_fn = void __fastcall( char** /*command text*/, int /*queue ctx*/ );
        // reads a cvar's effective float value (walks the override chain at +0x120)
        using cvar_get_float_fn = float __fastcall( void* /*cvar object*/ );
    }
}
