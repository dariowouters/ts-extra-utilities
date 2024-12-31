#pragma once
#include "accessory_data.hpp"
#include "prism/common.hpp"
#include "prism/string.hpp"
#include "prism/token.hpp"
#include "prism/collections/array_dyn.hpp"

#pragma pack(push, 1)

namespace ts_extra_utilities::prism
{
    class accessory_chassis_data_u : public accessory_data_u // Size: 0x0498
    {
    public:
        class string_dyn_t detail_model; //0x0118 (0x18)
        class string_dyn_t model; //0x0130 (0x18)
        class array_dyn_t< string_dyn_t > axle_model; //0x0148 (0x20)
        class array_dyn_t< string_dyn_t > axle_model_collision; //0x0168 (0x20)
        class array_dyn_t< string_dyn_t > lods; //0x0188 (0x20)
        class string_dyn_t collision; //0x01A8 (0x18)
        class string_dyn_t trailer_brace_model; //0x01C0 (0x18)
        class string_dyn_t trailer_brace_anim; //0x01D8 (0x18)
        class string_dyn_t trailer_brace_up_sound; //0x01F0 (0x18)
        class string_dyn_t trailer_brace_down_sound; //0x0208 (0x18)
        token_t variant; //0x0220 (0x08)
        token_t variant_uk; //0x0228 (0x08)
        token_t look; //0x0230 (0x08)
        class string_dyn_t shadow_texture; //0x0238 (0x18)
        float shadow_intensity; //0x0250 (0x04)
        char pad_0254[ 4 ]; //0x0254 (0x04)
        class string_dyn_t extended_shadow_texture; //0x0258 (0x18)
        float extended_shadow_intensity; //0x0270 (0x04)
        float extended_shadow_fadeout_start; //0x0274 (0x04)
        float extended_shadow_fadeout_length; //0x0278 (0x04)
        char pad_027C[ 4 ]; //0x027C (0x04)
        token_t extended_shadow_shared_model_variant; //0x0280 (0x08)
        class array_dyn_t< string_dyn_t > axle_model_extended_shadow_texture; //0x0288 (0x20)
        class string_dyn_t ui_shadow; //0x02A8 (0x18)
        float tank_size; //0x02C0 (0x04)
        float adblue_tank_size; //0x02C4 (0x04)
        float air_tank_pressure; //0x02C8 (0x04)
        float nominal_voltage; //0x02CC (0x04)
        class array_dyn_t< float > residual_travel; //0x02D0 (0x20)
        float trailer_mass; //0x02F0 (0x04)
        char pad_02F4[ 4 ]; //0x02F4 (0x04)
        class array_dyn_t< char > powered_axle; //0x02F8 (0x20)
        class array_dyn_t< float > kerb_weight; //0x0318 (0x20)
        class array_dyn_t< char > liftable_axle; //0x0338 (0x20)
        class array_dyn_t< token_t > steerable_axle; //0x0358 (0x20)
        bool steerable_lifted_axle; //0x0378 (0x01)
        bool powered_trailer; //0x0379 (0x01)
        char pad_037A[ 14 ]; //0x037A (0x0e)
        uint32_t weight_distribution; //0x0388 (0x04)
        float cog_cargo_mass_min; //0x038C (0x04)
        float cog_cargo_mass_max; //0x0390 (0x04)
        float3_t cog_cargo_offset_min; //0x0394 (0x0c)
        float3_t cog_cargo_offset_max; //0x03A0 (0x0c)
        uint32_t passenger_capacity; //0x03AC (0x04)
        float master_collision_angle; //0x03B0 (0x04)
        char pad_03B4[ 4 ]; //0x03B4 (0x04)
        class array_dyn_t< float3_t > wheel_positions; //0x03B8 (0x20)
        class array_dyn_t< char > wheel_types; //0x03D8 (0x20) f/r
        class array_dyn_t< float3_t > N00001EC5; //0x03F8 (0x20)
        char pad_0418[ 40 ]; //0x0418 (0x28)
        float2_t axle_model_sliding_limits; //0x0440 (0x08)
        float3_t hook_position; //0x0448 (0x0c)
        char pad_0454[ 4 ]; //0x0454 (0x04)
        class array_dyn_t< string_dyn_t > sounds; //0x0458 (0x20)
        class string_dyn_t lamp_setup; //0x0478 (0x18)
        char pad_0490[ 8 ]; //0x0490 (0x08)
    };

    static_assert(sizeof( accessory_chassis_data_u ) == 0x498);
}
#pragma pack(pop)
