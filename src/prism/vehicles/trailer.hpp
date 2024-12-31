#pragma once
#include "core_vehicle.hpp"

#pragma pack(push, 1)

namespace ts_extra_utilities::prism
{
    class trailer_u : public core_vehicle_u // size: 0x0188
    {
    public:
        class trailer_def_u* trailer_definition; // 0x0158 (0x08)
        bool oversize; // 0x0160 (0x01)
        char pad_0161[3]; // 0x0161 (0x03)
        float cargo_mass; // 0x0164 (0x04)
        float cargo_damage; // 0x0168 (0x04)
        float virtual_rear_wheels_offset; // 0x016c (0x04)
        trailer_u* slave_trailer; // 0x0170 (0x08)
        bool is_private; // 0x0178 (0x01)
        char pad_0179[3]; // 0x0179 (0x03)
        float trailer_body_wear; // 0x017c (0x04)
        float trailer_body_wear_unfixable; // 0x0180 (0x04)
        char pad_0184[4]; // 0x0184 (0x04)
    };

    static_assert(sizeof(trailer_u) == 0x188, "trailer_u is incorrect in size");
}

#pragma pack(pop)
