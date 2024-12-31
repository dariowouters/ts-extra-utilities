#pragma once

#pragma pack(push, 1)
#include "prism/common.hpp"
#include "prism/collections/array_dyn.hpp"

struct float3_t;

namespace ts_extra_utilities::prism
{
    class accessory_chassis_data_u;
    class physics_actor_physx_t;

    class vehicle_wheel_steering_data_t // Size: 0x0060
    {
    public:
        array_dyn_t< float > steps; //0x0000 (0x20)
        char pad_0020[ 8 ]; //0x0020 (0x08)
        array_dyn_t< float > steering; //0x0028 (0x20)
        char pad_0048[ 24 ]; //0x0048 (0x18)
    };

    static_assert(sizeof( vehicle_wheel_steering_data_t ) == 0x60);

    class vehicle_shared_u : public unit_t // Size: 0x0D58
    {
    public:
        char pad_0010[ 32 ]; //0x0010 (0x20)
        physics_actor_physx_t* dynamic_physics_actor; //0x0030 (0x08)
        char pad_0038[ 48 ]; //0x0038 (0x30)
        class game_trailer_actor_u* game_physics_vehicle; //0x0068 (0x08)
        char pad_0070[ 144 ]; //0x0070 (0x90)
        class physics_powertrain_u* physics_powertrain; //0x0100 (0x08)
        class array_dyn_t< void* > model_objects; //0x0108 (0x20)
        char pad_0128[ 32 ]; //0x0128 (0x20)
        class core_vehicle_u* vehicle; //0x0148 (0x08)
        accessory_chassis_data_u* accessory_chassis_data; //0x0150 (0x08)
        char pad_0158[ 72 ]; //0x0158 (0x48)
        class fake_shadow_u* fake_shadow; //0x01A0 (0x08)
        char pad_01A8[ 40 ]; //0x01A8 (0x28)
        class kdop_item_t* current_kdop_item; //0x01D0 (0x08)
        char pad_01D8[ 64 ]; //0x01D8 (0x40)
        float speed; //0x0218 (0x04)
        char pad_021C[ 4 ]; //0x021C (0x04)
        class array_dyn_t< float3_t > wheel_offset; //0x0220 (0x20)
        class array_dyn_t< float3_t > wheel_location; //0x0240 (0x20)
        class array_dyn_t< float > tire_shift; //0x0260 (0x20)
        class array_dyn_t< float > tire_size_idk; //0x0280 (0x20)
        class array_dyn_t< float > N00002B76; //0x02A0 (0x20)
        class array_dyn_t< float > N00002B7A; //0x02C0 (0x20)
        class array_dyn_t< float > N00002B7E; //0x02E0 (0x20)
        char pad_0300[ 44 ]; //0x0300 (0x2c)
        float N000060E9; //0x032C (0x04)
        float N00002B88; //0x0330 (0x04)
        float3_t hook_locator; //0x0334 (0x0c)
        float N0000520A; //0x0340 (0x04)
        float N00006014; //0x0344 (0x04)
        float N00002B8A; //0x0348 (0x04)
        char pad_034C[ 20 ]; //0x034C (0x14)
        class array_dyn_t< float > precalculated_axle_weight; //0x0360 (0x20)
        char pad_0380[ 40 ]; //0x0380 (0x28)
        class N00005D5E* N00002B98; //0x03A8 (0x08)
        char pad_03B0[ 24 ]; //0x03B0 (0x18)
        class N00005CFF* N00002B9C; //0x03C8 (0x08)
        char pad_03D0[ 64 ]; //0x03D0 (0x40)
        class N00005D1C* N00002BA5; //0x0410 (0x08)
        char pad_0418[ 24 ]; //0x0418 (0x18)
        class N00005CFF* N00006465; //0x0430 (0x08)
        char pad_0438[ 16 ]; //0x0438 (0x10)
        class array_dyn_t< float > N00002BB0; //0x0448 (0x20)
        class array_dyn_t< float > wheel_weight_idk; //0x0468 (0x20)
        class array_dyn_t< float > N00002BB4; //0x0488 (0x20)
        class array_dyn_t< float > N00002BB8; //0x04A8 (0x20)
        class array_dyn_t< float > N00002BBE; //0x04C8 (0x20)
        float steering; //0x04E8 (0x04)
        char pad_04EC[ 4 ]; //0x04EC (0x04)
        vehicle_wheel_steering_data_t* wheel_steering_stuff; //0x04F0 (0x08)
        char pad_04F8[ 160 ]; //0x04F8 (0xa0)
        class array_dyn_t< void* > accessory_model_objects; //0x0598 (0x20)
        class array_dyn_t< void* > accessory_hookup_model_objects; //0x05B8 (0x20)
        char pad_05D8[ 1576 ]; //0x05D8 (0x628)
        class array_dyn_t< float > wheels_20_steering; //0x0C00 (0x20)
        char pad_0C20[ 312 ]; //0x0C20 (0x138)

        virtual void vfunc_10();
        virtual void vfunc_11();
        virtual void vfunc_12();
        virtual void vfunc_13();
        virtual void vfunc_14();
        virtual void vfunc_15();
        virtual void vfunc_16();
        virtual void vfunc_17();
        virtual void vfunc_18();
        virtual void vfunc_19();
        virtual void vfunc_20();
        virtual void vfunc_21();
        virtual void vfunc_22();
        virtual void vfunc_23();
        virtual void vfunc_24();
        virtual void vfunc_25();
        virtual void vfunc_26();
        virtual void vfunc_27();
        virtual void vfunc_28();
        virtual void vfunc_29();
        virtual void vfunc_30();
        virtual void vfunc_31();
        virtual void vfunc_32();
        virtual void vfunc_33();
        virtual void vfunc_34();
        virtual void vfunc_35();
        virtual void vfunc_36();
        virtual void vfunc_37();
        virtual void vfunc_38();
        virtual void vfunc_39();
        virtual void vfunc_40();
        virtual void vfunc_41();
        virtual void vfunc_42();
        virtual void vfunc_43();
        virtual void vfunc_44();
        virtual void vfunc_45();
        virtual void vfunc_46();
        virtual void vfunc_47();
        virtual void vfunc_48();
        virtual void vfunc_49();
        virtual void vfunc_50();
        virtual void vfunc_51();
        virtual void vfunc_52();
        virtual void vfunc_53();
        virtual void vfunc_54();
        virtual void vfunc_55();
        virtual void vfunc_56();
        virtual void vfunc_57();
        virtual void vfunc_58();
        virtual void vfunc_59();
        virtual void vfunc_60();
        virtual void vfunc_61();
        virtual void vfunc_62();
        virtual void vfunc_63();
        virtual void vfunc_64();
        virtual void vfunc_65();
        virtual void vfunc_66();
        virtual void vfunc_67();
        virtual void vfunc_68();
        virtual void vfunc_69();
        virtual void vfunc_70();
        virtual void vfunc_71();
        virtual void vfunc_72();
        virtual void steering_advance();
        virtual void vfunc_74();
        virtual void vfunc_75();
        virtual void vfunc_76();
        virtual void vfunc_77();
        virtual void vfunc_78();
        virtual void vfunc_79();
        virtual void vfunc_80();
        virtual void vfunc_81();
        virtual void vfunc_82();
        virtual void vfunc_83();
        virtual void vfunc_84();
        virtual void vfunc_85();
    };

    static_assert(sizeof( vehicle_shared_u ) == 0xD58);
}

#pragma pack(pop)
