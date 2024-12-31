#pragma once
#include "accessory_data.hpp"
#include "prism/string.hpp"
#include "prism/token.hpp"
#include "prism/collections/array_dyn.hpp"

#pragma pack(push, 1)

namespace ts_extra_utilities::prism
{
    class accessory_data_u : public unit_t // Size: 0x0118
    {
    public:
        class string_dyn_t name; //0x0010 (0x18)
        class string_dyn_t icon; //0x0028 (0x18)
        class array_dyn_t< string_dyn_t > info; //0x0040 (0x20)
        int64_t price; //0x0060 (0x08)
        uint32_t unlock; //0x0068 (0x04)
        uint32_t part_type; //0x006C (0x04)
        class array_dyn_t< string_dyn_t > suitable_for; //0x0070 (0x20)
        class array_dyn_t< string_dyn_t > conflict_with; //0x0090 (0x20)
        class array_dyn_t< string_dyn_t > defaults; //0x00B0 (0x20)
        class array_dyn_t< string_dyn_t > overrides; //0x00D0 (0x20)
        class array_dyn_t< token_t > require; //0x00F0 (0x20)
        bool sync_over_network; //0x0110 (0x01)
        char pad_0111[ 3 ]; //0x0111 (0x03)
        uint32_t steam_inventory_id; //0x0114 (0x04)
    };

    static_assert(sizeof( accessory_data_u ) == 0x118);
}

#pragma pack(pop)
