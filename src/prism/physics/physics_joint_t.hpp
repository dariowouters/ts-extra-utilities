#pragma once

#include "./physics_actor_t.hpp"
#include "physx/d6_joint.hpp"

#pragma pack(push, 1)

namespace ts_extra_utilities::prism
{
    class physics_joint_t // Size: 0x0018
    {
    public:
        class physics_actor_t* physics_actor; //0x0008 (0x08)
        char pad_0010[ 8 ]; //0x0010 (0x08)

        virtual void destructor();
    };

    class physics_joint_physx_t : public physics_joint_t // Size: 0x0080
    {
    public:
        physx::PxD6Joint* px_joint; //0x0018 (0x08)
        char pad_0020[ 96 ]; //0x0020 (0x60)
    };
}
#pragma pack(pop)
