#pragma once

#pragma pack(push, 1)

namespace ts_extra_utilities
{
    struct float2_t // size: 0x0008
    {
        float x; // 0x0000 (0x04)
        float y; // 0x0004 (0x04)
    };

    static_assert(sizeof( float2_t ) == 0x08);

    struct float3_t // size: 0x000c
    {
        float x; // 0x0000 (0x04)
        float y; // 0x0004 (0x04)
        float z; // 0x0008 (0x04)
    };

    static_assert(sizeof( float3_t ) == 0x0c);

    struct quat_t // size: 0x0010
    {
        float w; // 0x0000 (0x04)
        float x; // 0x0004 (0x04)
        float y; // 0x0008 (0x04)
        float z; // 0x000c (0x04)
    };

    static_assert(sizeof( quat_t ) == 0x10);
}
#pragma pack(pop)
