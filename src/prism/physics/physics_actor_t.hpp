#pragma once

#include "prism/collections/array_dyn.hpp"
#include "./physics_joint_t.hpp"

#pragma pack(push, 1)

namespace ts_extra_utilities::prism
{
    class physics_actor_physx_t // Size: 0x0140
    {
    public:
        array_dyn_t< class physics_shape_physx_t* > shapes; //0x0008 (0x20)
        array_dyn_t< physics_joint_physx_t* > joints; //0x0028 (0x20)
        char pad_0048[ 48 ]; //0x0048 (0x30)
        char pad_0078[ 16 ]; //0x0078 (0x10)
        float N00005F1C; //0x0088 (0x04)
        float N00011C3A; //0x008C (0x04)
        float N00005F1D; //0x0090 (0x04)
        float N00011C3C; //0x0094 (0x04)
        char pad_0098[ 56 ]; //0x0098 (0x38)
        class PxRigidStatic* px_actor; //0x00D0 (0x08)
        char pad_00D8[ 104 ]; //0x00D8 (0x68)

        virtual void destructor();
    };

    static_assert(sizeof( physics_actor_physx_t ) == 0x140);
}
#pragma pack(pop)
