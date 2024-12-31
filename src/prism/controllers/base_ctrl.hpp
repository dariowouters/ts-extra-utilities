#pragma once
#include "prism/string.hpp"
#include "prism/collections/array_dyn.hpp"
#include "prism/unit/unit.hpp"

#pragma pack(push, 1)

namespace ts_extra_utilities::prism
{
    class token_t;
    class trailer_u;
    class trailer_def_u;

    class base_ctrl_u : public unit_t // Size: 0x2980
    {
    public:
        class array_dyn_t< class kdop_item_t* > map_kdop_items; //0x0010 (0x20)
        class array_dyn_t< class node_item_t* > map_nodes; //0x0030 (0x20)
        char pad_0050[ 16 ]; //0x0050 (0x10)
        class game_data_u* game_data; //0x0060 (0x08)
        char pad_0068[ 24 ]; //0x0068 (0x18)
        class map_data_u* map_data; //0x0080 (0x08)
        char pad_0088[ 48 ]; //0x0088 (0x30)
        class string_dyn_t current_time_zone; //0x00B8 (0x18)
        char pad_00D0[ 344 ]; //0x00D0 (0x158)
        class array_dyn_t< trailer_u* > unk_trailers; //0x0228 (0x20)
        class array_dyn_t< trailer_def_u* > unk_trailer_defs; //0x0248 (0x20)
        class array_dyn_t< class traffic_vehicle_type_u* > traffic_vehicle_types; //0x0268 (0x20)
        class array_dyn_t< class traffic_trailer_type_u* > traffic_trailer_types; //0x0288 (0x20)
        class array_dyn_t< class accessory_hookup_data_u* > accessory_hookup_datas; //0x02A8 (0x20)
        class license_plate_manager_u* license_plate_manager; //0x02C8 (0x08)
        class driver_plate_manager_u* driver_plate_manager; //0x02D0 (0x08)
        class player_tag_manager_u* player_tag_manager; //0x02D8 (0x08)
        class traffic_sign_manager_u* traffic_sign_manager; //0x02E0 (0x08)
        char pad_02E8[ 720 ]; //0x02E8 (0x2d0)
        class array_dyn_t< token_t > N000004AC; //0x05B8 (0x20)
        class array_dyn_t< token_t > N000004B0; //0x05D8 (0x20)
        char pad_05F8[ 56 ]; //0x05F8 (0x38)
        class trigger_manager_u* trigger_manager; //0x0630 (0x08)
        class camera_manager_u* camera_manager; //0x0638 (0x08)
        class cutscene_manager_u* cutscene_manager; //0x0640 (0x08)
        class journey_events_manager_u* journey_events_manager; //0x0648 (0x08)
        char pad_0650[ 16 ]; //0x0650 (0x10)
        class core_camera_u* core_camera; //0x0660 (0x08)
        class environment_u* environment; //0x0668 (0x08)
        class oversize_manager_u* oversize_manager; //0x0670 (0x08)
        char pad_0678[ 72 ]; //0x0678 (0x48)
        class eye_state_u* eye_state; //0x06C0 (0x08)
        char pad_06C8[ 7192 ]; //0x06C8 (0x1c18)
        class local_gameplay_mode_u* local_gameplay_mode; //0x22E0 (0x08)
        char pad_22E8[ 528 ]; //0x22E8 (0x210)
        uint64_t selected_physics_engine; //0x24F8 (0x08) 1 = physx
        class physics_server_physx_t* physics_server; //0x2500 (0x08)
        char pad_2508[ 24 ]; //0x2508 (0x18)
        class physics_data_u* physics_data; //0x2520 (0x08)
        class physics_interior_camera_data_u* physics_interior_camera_data; //0x2528 (0x08)
        class force_feedback_data_u* force_feedback_data; //0x2530 (0x08)
        class vehicle_damage_data_u* vehicle_damage_data; //0x2538 (0x08)
        class vehicle_service_data_u* vehicle_service_data; //0x2540 (0x08)
        class sector_manager_u* sector_manager; //0x2548 (0x08)
        char pad_2550[ 360 ]; //0x2550 (0x168)
        class asset_catalog_u* asset_catalog; //0x26B8 (0x08)
        char pad_26C0[ 432 ]; //0x26C0 (0x1b0)
        class game_actor_u* game_actor__dont_use; //0x2870 (0x08)
        char pad_2878[ 272 ]; //0x2878 (0x110)
    };

    static_assert(sizeof( base_ctrl_u ) == 0x2988);
}
#pragma pack(pop)
