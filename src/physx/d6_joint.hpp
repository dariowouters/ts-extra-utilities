#pragma once

#include <cstdint>
#include "common.hpp"

#pragma pack(push, 1)
namespace ts_extra_utilities::physx
{
    class D6JointData // Size: 0x01B8
    {
    public:
        PxConstraintInvMassScale mInvScale;
        PxTransform c2b_1; //0x0010 (0x1c)
        PxTransform c2b_2; //0x002C (0x1c)
        char pad_0048[ 8 ]; //0x0048 (0x08)
        uint32_t motion_eX; //0x0050 (0x04)
        uint32_t motion_eY; //0x0054 (0x04)
        uint32_t motion_eZ; //0x0058 (0x04)
        uint32_t motion_eTWIST; //0x005C (0x04)
        uint32_t motion_eSWING1; //0x0060 (0x04)
        uint32_t motion_eSWING2; //0x0064 (0x04)
        char pad_0068[ 336 ]; //0x0068 (0x150)
    };

    static_assert(sizeof( D6JointData ) == 0x1B8);

    struct PxD6Axis
    {
        enum Enum
        {
            eX = 0,	//!< motion along the X axix
            eY = 1,	//!< motion along the Y axis
            eZ = 2,	//!< motion along the Z axis
            eTWIST = 3,	//!< motion around the X axis
            eSWING1 = 4,	//!< motion around the Y axis
            eSWING2 = 5,	//!< motion around the Z axis
            eCOUNT = 6
        };
    };

    struct PxD6Motion
    {
        enum Enum
        {
            eLOCKED,	//!< The DOF is locked, it does not allow relative motion.
            eLIMITED,	//!< The DOF is limited, it only allows motion within a specific range.
            eFREE		//!< The DOF is free and has its full range of motion.
        };
    };

    class PxD6Joint // Size: 0x0088
    {
    public:
        char pad_0008[ 24 ]; //0x0008 (0x18)
        char* name; //0x0020 (0x08)
        PxTransform mLocalPose_1; //0x0028 (0x1c)
        PxTransform mLocalPose_2; //0x0044 (0x1c)
        PxConstraint* mPxConstraint; //0x0060 (0x08)
        D6JointData* mData; //0x0068 (0x08)
        bool mRecomputeMotion; //0x0070 (0x01)
        bool mRecomputeLimits; //0x0071 (0x01)
        char pad_0072[ 22 ]; //0x0072 (0x16)

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
        virtual void Function10();
        virtual void Function11();
        virtual void Function12();
        virtual void Function13();
        virtual void Function14();
        virtual void Function15();
        virtual void Function16();
        virtual void Function17();
        virtual void Function18();
        virtual void Function19();
        virtual void Function20();
        virtual void Function21();
        virtual void Function22();
        virtual void Function23();
        virtual void Function24();
        virtual void Function25();
        virtual void Function26();
        virtual void Function27();
        virtual void Function28();
        virtual void setMotion( PxD6Axis::Enum index, PxD6Motion::Enum t );
        virtual void Function30();
        virtual void Function31();
        virtual void Function32();
        virtual void Function33();
        virtual void Function34();
        virtual void Function35();
        virtual void Function36();
        virtual void Function37();
        virtual void Function38();
        virtual void Function39();
        virtual void Function40();
        virtual void Function41();
        virtual void Function42();
        virtual void Function43();
        virtual void Function44();
        virtual void Function45();
        virtual void Function46();
        virtual void Function47();
        virtual void Function48();
        virtual void Function49();
        virtual void Function50();
        virtual void Function51();
        virtual void Function52();
    };

    static_assert(sizeof( PxD6Joint ) == 0x88);
}

#pragma pack(pop)
