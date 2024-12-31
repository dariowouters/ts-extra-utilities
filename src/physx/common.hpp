#pragma once
#include "prism/common.hpp"

#pragma pack(push, 1)

namespace ts_extra_utilities::physx
{
    class PxConstraintInvMassScale // Size: 0x0010
    {
    public:
        float linear0; //0x0000 (0x04)
        float angular0; //0x0004 (0x04)
        float linear1; //0x0008 (0x04)
        float angular1; //0x000C (0x04)
    };

    static_assert(sizeof( PxConstraintInvMassScale ) == 0x10);

    class PxTransform // Size: 0x001C
    {
    public:
        quat_t q; //0x0000 (0x10)
        float3_t p; //0x0010 (0x0c)
    };

    static_assert(sizeof( PxTransform ) == 0x1C);

    class PxConstraint // Size: 0x0180
    {
    public:
        char pad_0008[ 136 ]; //0x0008 (0x88)
        float N00005E93; //0x0090 (0x04)
        float N00005EB3; //0x0094 (0x04)
        float N00005EC6; //0x0098 (0x04)
        char pad_009C[ 20 ]; //0x009C (0x14)
        bool mIsDirty; //0x00B0 (0x01)
        char pad_00B1[ 207 ]; //0x00B1 (0xcf)

        virtual void Function0();
        virtual void Function1();
        virtual void Function2();
        virtual void Function3();
        virtual void Function4();
        virtual void Function5();
        virtual void Function6();
        virtual void Function7();
        virtual void Function8();
        virtual void Function9();
    };

    static_assert(sizeof( PxConstraint ) == 0x180);
}

#pragma pack(pop)
