#pragma once
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
    }
}
