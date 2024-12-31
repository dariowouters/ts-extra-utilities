#pragma once
#include "prism/string.hpp"
#include "prism/collections/array_dyn.hpp"
#include "prism/unit/unit.hpp"

#pragma pack(push, 1)

namespace ts_extra_utilities::prism
{
    class core_vehicle_u : public unit_t // size: 0x0158
    {
    public:
        array_dyn_t<class vehicle_accessory_u*> accessories; // 0x0010 (0x20)
        char pad_0030[8]; // 0x0030 (0x08)
        unsigned int odometer; // 0x0038 (0x04)
        float odometer_float_part; // 0x003c (0x04)
        unsigned int integrity_odometer; // 0x0040 (0x04)
        float integrity_odometer_float_part; // 0x0044 (0x04)
        unsigned int trip_fuel_l; // 0x0048 (0x04)
        float trip_fuel; // 0x004c (0x04)
        unsigned int trip_recuperation_kwh; // 0x0050 (0x04)
        float trip_recuperation; // 0x0054 (0x04)
        unsigned int trip_distance_km; // 0x0058 (0x04)
        float trip_distance; // 0x005c (0x04)
        unsigned int trip_time_min; // 0x0060 (0x04)
        float trip_time; // 0x0064 (0x04)
        string_dyn_t license_plate; // 0x0068 (0x18)
        float chassis_wear; // 0x0080 (0x04)
        float chassis_wear_unfixable; // 0x0084 (0x04)
        array_dyn_t<float> wheels_wear; // 0x0088 (0x20)
        array_dyn_t<float> wheels_wear_unfixable; // 0x00a8 (0x20)
        char pad_00c8[144]; // 0x00c8 (0x90)
    };

    static_assert(sizeof(core_vehicle_u) == 0x158, "core_vehicle_u is incorrect in size");
}

#pragma pack(pop)
