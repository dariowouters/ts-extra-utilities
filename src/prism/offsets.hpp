#pragma once
#include <cstdint>

// Field offsets for the trailer steering feature, re-derived for game version 1.61.1.9
// (ETS2 and ATS share them). The structs in prism/ still describe the 1.53-era layout
// and are NOT valid for these fields anymore, so read them through here instead.
//
// 1.61.1.9 added 0x28 bytes to the trailer object ahead of the steering fields, so every
// offset past that point moved from the 1.61.1.1 values (steering 0x690, data 0x698,
// slave 0x1060). game_actor itself is unchanged.
//
// How each was found (offline, from the exe):
//  - steering / steering data: physics_trailer_u::steering_advance stores the angle to
//    +0x6B8 and passes the pointer at +0x6C0 to set_individual_steering
//  - slave trailer: prism::physics_trailer_u::connect_slave tests +0x1088 against null
//  - first trailer: the `cheat repair` handler walks game_actor +0x18 (truck) and +0xC8
namespace ts_extra_utilities::prism::offsets
{
    constexpr uint32_t game_actor_truck = 0x0018; // game_physics_vehicle_u*, the player's truck
    constexpr uint32_t game_actor_trailer = 0x00C8; // game_trailer_actor_u*, first trailer
    constexpr uint32_t trailer_parent = 0x1080; // game_trailer_actor_u*, previous trailer in the chain
    constexpr uint32_t trailer_slave = 0x1088; // game_trailer_actor_u*, next trailer in the chain
    constexpr uint32_t trailer_steering = 0x06B8; // float, -1..1, written by the game every advance
    constexpr uint32_t trailer_steering_data = 0x06C0; // vehicle_wheel_steering_data_t*, null when not steerable

    // Trailer connect/disconnect (1.61.1.9). physics_joint is null while a trailer is
    // detached; the game's own detach path checks it before calling disconnect and then
    // lowering the brace. hook_locator is the vehicle's own coupling point in local
    // space; chassis+0x740 is the rear hook position on the chassis data object
    // (truck+0x200). All verified in the native attach/detach code.
    constexpr uint32_t trailer_physics_joint = 0x0FE0; // physics_joint_physx_t*, null while detached
    constexpr uint32_t vehicle_hook_locator = 0x0464; // float3_t
    constexpr uint32_t vehicle_chassis_data = 0x0200; // chassis data object
    constexpr uint32_t chassis_hook_position = 0x0740; // float3_t

    // Parked-trailer state: the game sets trailer_parked once a jointless trailer
    // settles, which keeps its wheels locked like a spring brake. The native attach
    // path clears the byte when trailer_parked_state exists; manual connects must do
    // the same or the trailer drags along braked.
    constexpr uint32_t trailer_parked_state = 0x1188; // object*, non-null while the parked state is tracked
    constexpr uint32_t trailer_parked = 0x1198; // byte, 1 = parked/locked, cleared by native attach

    // More fields from the native attach/detach code: connect_slave copies the
    // coupling byte parent->slave and passes the parent's chassis-type check to
    // connect(); trailer_total_mass feeds the towing vehicle's wheel load.
    constexpr uint32_t vehicle_coupling_flag = 0x0FDB; // byte, propagated parent -> slave on connect
    constexpr uint32_t chassis_slave_hook_type = 0x0628; // dword on chassis data, == 2 marks a slave/dolly hook
    constexpr uint32_t trailer_total_mass = 0x04B8; // float, passed to the parent's wheel-load distribution

    // Trailer brake channels (1.61.1.9): the game's per-frame trailer update writes
    // trailer_brake = 1.0 and trailer_service_brake = 0 while the physics joint is
    // missing (the spring brake a detached trailer sits on), and the byte at +0x764
    // forces full brake regardless of the float. Nothing zeroes them on a manual
    // connect, so do_connect must release them itself or the wheels stay locked.
    // Verified at 0x1409fb34f / 0x140637e84 (writers) and 0x14090c562 / 0x140644181
    // (per-wheel brake consumers).
    constexpr uint32_t trailer_brake = 0x0760; // float, 0..1 brake factor applied per wheel
    constexpr uint32_t trailer_service_brake = 0x0730; // float, zeroed alongside on detach
    constexpr uint32_t trailer_brake_forced = 0x0764; // byte, != 0 forces full brake

    // Truck coupling state on the truck physics object (game_actor+0x18): the
    // per-frame trailer loop applies spring brake to every jointless trailer, and
    // to jointed ones too while this state is not 5 (fully coupled). The truck's
    // brake/electric broadcast down the towed chain is likewise gated on 5.
    // The native attach state machine ends at 5; a manual connect must write it
    // or the trailer stays braked and dark.
    // Verified at 0x1405d8c94 (spring-brake loop) and 0x14064b760 (broadcast gate).
    constexpr uint32_t truck_coupling_state = 0x14E0; // dword, 5 = trailer coupled

    // vfunc slots on the vehicle/trailer vtable: connect_slave calls [slave+0x310] on
    // the next trailer, the detach path calls [+0x318] then [+0x328](true), and the
    // re-attach path calls [+0x310] then [+0x328](false).
    constexpr uint32_t vfunc_connect = 98; // +0x310, connect(connect_to, hook_vec, a4, a5, a6)
    constexpr uint32_t vfunc_disconnect = 99; // +0x318, disconnect()
    constexpr uint32_t vfunc_set_brace = 101; // +0x328, set_trailer_brace(bool lowered)

    template < typename T >
    T& field( void* object, const uint32_t offset )
    {
        return *reinterpret_cast< T* >( reinterpret_cast< uint8_t* >( object ) + offset );
    }
}
