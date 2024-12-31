#pragma once
#include "token.hpp"
#include "./unit/unit.hpp"
#include "./vehicles/game_trailer_actor.hpp"
#include "memory/memory_utils.hpp"

#pragma pack(push, 1)

namespace ts_extra_utilities::prism
{
    class game_trailer_actor_u;

    class game_physics_vehicle_u : public vehicle_shared_u
    {
    };

    class game_actor_u : public unit_t // Size: 0x10D8
    {
    public:
        char pad_0010[ 8 ]; //0x0010 (0x08)
        game_physics_vehicle_u* game_physics_vehicle; //0x0018 (0x08)
        class visual_interior_u* visual_interior; //0x0020 (0x08)
        class game_actor_hud_u* game_actor_hud; //0x0028 (0x08)
        char pad_0030[ 104 ]; //0x0030 (0x68)
        class history_stream_set_u* history_stream_set; //0x0098 (0x08)
        char pad_00A0[ 16 ]; //0x00A0 (0x10)
        class history_manager_u* history_manager; //0x00B0 (0x08)
        game_trailer_actor_u* game_trailer_actor; //0x00B8 (0x08)
        char pad_00C0[ 88 ]; //0x00C0 (0x58)
        uint32_t current_camera; //0x0118 (0x04) 2 = int, 1 = chase, 7 = top, 5 = roof, 4 = side_back, 3 = hood, 6 = side_front, 9 = cinematic, 0 = dev
        char pad_011C[ 8 ]; //0x011C (0x08)
        uint32_t engine_state; //0x0124 (0x04) 0 = engine off, 1 = starting, 2 = running, 3 = shutting down
        char pad_0128[ 8 ]; //0x0128 (0x08)
        uint8_t parking_brake; //0x0130 (0x01)
        char pad_0131[ 63 ]; //0x0131 (0x3f)
        float speed_limiter; //0x0170 (0x04)
        float road_speed_limit; //0x0174 (0x04)
        char pad_0178[ 52 ]; //0x0178 (0x34)
        uint16_t light_state; //0x01AC (0x02) (flags) 6 = parking, 8 = low, 7 = high, 5 = beacons
        char pad_01AE[ 2 ]; //0x01AE (0x02)
        uint16_t additional_illumination; //0x01B0 (0x02) (flags) 3 = front, 4 = roof
        char pad_01B2[ 14 ]; //0x01B2 (0x0e)
        float wipers_intermittent_time; //0x01C0 (0x04) Countdown before wipers move again when in intermittent setting
        float wipers_position; //0x01C4 (0x04) 0 = down start, 1 = down end ???
        float rpm; //0x01C8 (0x04)
        float air_pressure; //0x01CC (0x04)
        float brake_pressure; //0x01D0 (0x04)
        float max_air_pressure; //0x01D4 (0x04)
        char pad_01D8[ 8 ]; //0x01D8 (0x08)
        float oil_temp; //0x01E0 (0x04)
        char pad_01E4[ 4 ]; //0x01E4 (0x04)
        float water_temp; //0x01E8 (0x04)
        char pad_01EC[ 4 ]; //0x01EC (0x04)
        float battery_voltage; //0x01F0 (0x04)
        char pad_01F4[ 8 ]; //0x01F4 (0x08)
        float turbo_pressure; //0x01FC (0x04) ?
        char pad_0200[ 16 ]; //0x0200 (0x10)
        float light_switch; //0x0210 (0x04) 0 = off, 0.5 = parking, 1 = low beam
        float light_switch_state; //0x0214 (0x04) 0 = off, 0.5 = parking, 1 = low beam (instant)
        float high_beam_stick; //0x0218 (0x04) 0 = off, 1 = on
        float high_beam_state; //0x021C (0x04) 0 = off, 1 = on (instant)
        float light_horn_stick; //0x0220 (0x04) 0 = off, 1 = on
        float light_horn_state; //0x0224 (0x04) 0 = off, 1 = on (instant)
        float indicator_stick; //0x0228 (0x04) 0 = left indicator, 0.5 = none, 1 = right
        float indicator_state; //0x022C (0x04) 0 = left indicator, 0.5 = none, 1 = right (instant)
        float hazard_warning_btn; //0x0230 (0x04) 0 = off, 1 = on
        float hazard_warning_state; //0x0234 (0x04) 0 = off, 1 = on (instant)
        float beacon_switch; //0x0238 (0x04) 0 = off, 1 = on
        float beacon_state; //0x023C (0x04) 0 = off, 1 = on (instant)
        char pad_0240[ 8 ]; //0x0240 (0x08)
        float handbrake_handle; //0x0248 (0x04) 0 = off, 1 = on
        float handbrake_state; //0x024C (0x04) 0 = off, 1 = on (instant)
        char pad_0250[ 16 ]; //0x0250 (0x10)
        float engine_brake_stick; //0x0260 (0x04) 0 = off, 1 = full; steps dependant on engine data
        float engine_brake_state; //0x0264 (0x04) 0 = off, 1 = full (instant); steps dependant on engine data
        char pad_0268[ 20 ]; //0x0268 (0x14)
        float wipers_stick; //0x027C (0x04) 0 = off, 0.333 = intermittent, 0.666 = slow, 1 = fast
        float wipers_state; //0x0280 (0x04) 0 = off, 0.333 = intermittent, 0.666 = slow, 1 = fast (instant)
        char pad_0284[ 20 ]; //0x0284 (0x14)
        uint8_t left_window_moving_direction; //0x0298 (0x01) 1 = got to end down; 2 = got to end up; 16 = down; 32 = up
        uint8_t is_left_window_moving; //0x0299 (0x01) 1 = down auto; 16 = down holding btn; 2 = auto up; 32 = up holding button
        char pad_029A[ 2 ]; //0x029A (0x02)
        float left_window_state; //0x029C (0x04) 0 = up; 1 = down
        float left_window_btn; //0x02A0 (0x04) 0 = up; 0.5 off; 1 = down
        float left_window_btn_state; //0x02A4 (0x04) 0 = up; 0.5 off; 1 = down (instant)
        float N00002B28; //0x02A8 (0x04)
        char pad_02AC[ 4 ]; //0x02AC (0x04)
        uint8_t right_window_moving_direction; //0x02B0 (0x01) 1 = got to end down; 2 = got to end up; 16 = down; 32 = up
        uint8_t is_right_window_moving; //0x02B1 (0x01) 1 = down auto; 16 = down holding btn; 2 = auto up; 32 = up holding button
        char pad_02B2[ 2 ]; //0x02B2 (0x02)
        float right_window_state; //0x02B4 (0x04) 0 = up; 1 = down
        float right_window_btn; //0x02B8 (0x04) 0 = up; 0.5 off; 1 = down
        float right_window_btn_state; //0x02BC (0x04) 0 = up; 0.5 off; 1 = down (instant)
        char pad_02C0[ 1880 ]; //0x02C0 (0x758)
        class game_actor_sounds_t* sound_events; //0x0A18 (0x08)
        char pad_0A20[ 136 ]; //0x0A20 (0x88)
        class model_object_u* driver_model; //0x0AA8 (0x08)
        char pad_0AB0[ 16 ]; //0x0AB0 (0x10)
        class physics_gearbox_sequential_u* physics_gearbox_sequential; //0x0AC0 (0x08)
        class physics_gearbox_automatic_u* physics_gearbox_automatic; //0x0AC8 (0x08)
        class physics_gearbox_direct_u* physics_gearbox_direct; //0x0AD0 (0x08)
        char pad_0AD8[ 8 ]; //0x0AD8 (0x08)
        class accessory_head_lights_data_u* accessory_head_lights_data; //0x0AE0 (0x08)
        char pad_0AE8[ 1232 ]; //0x0AE8 (0x4d0)
        token_t N00002CB4; //0x0FB8 (0x08)
        char pad_0FC0[ 280 ]; //0x0FC0 (0x118)

    public:
        game_trailer_actor_u* get_actual_slave_trailer( const vehicle_shared_u* parent_vehicle ) const
        {
            if ( this->game_trailer_actor == nullptr )
            {
                return nullptr;
            }

            auto* wanted_trailer = this->game_trailer_actor;

            while ( wanted_trailer != nullptr )
            {
                if ( wanted_trailer->parent_vehicle == parent_vehicle )
                {
                    return wanted_trailer;
                }

                wanted_trailer = wanted_trailer->slave_trailer;
            }
            return nullptr;
        }

        game_trailer_actor_u* get_last_trailer_connected_to_truck() const
        {
            vehicle_shared_u* last_vehicle = this->game_physics_vehicle;
            do
            {
                auto* slave = this->get_actual_slave_trailer( last_vehicle );
                if ( slave == nullptr ) break;
                last_vehicle = slave;
            }
            while ( true );

            if ( last_vehicle == this->game_physics_vehicle ) return nullptr;
            return reinterpret_cast< game_trailer_actor_u* >( last_vehicle );
        }
    };

    static_assert(sizeof( game_actor_u ) == 0x10D8);
}

#pragma pack(pop)
