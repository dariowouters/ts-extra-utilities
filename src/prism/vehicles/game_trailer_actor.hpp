#pragma once
#include "vehicle_shared.hpp"

#pragma pack(push, 1)
namespace ts_extra_utilities::prism
{
    class physics_joint_physx_t;

    class physics_trailer_u : public vehicle_shared_u // Size: 0x0EC0
    {
    public:
        char pad_0D58[ 8 ]; //0x0D58 (0x08)
        physics_joint_physx_t* physics_joint; //0x0D60 (0x08) nullptr when trailer not attached
        char pad_0D68[ 128 ]; //0x0D68 (0x80)
        vehicle_shared_u* parent_vehicle; //0x0DE8 (0x08) truck for 1st trailer
        game_trailer_actor_u* slave_trailer; //0x0DF0 (0x08)
        class accessory_trailer_data_u* accessory_trailer_data; //0x0DF8 (0x08)
        class vehicle_accessory_u* transmission_accessory; //0x0E00 (0x08)
        class vehicle_accessory_u* engine_accessory; //0x0E08 (0x08)
        char pad_0E10[ 64 ]; //0x0E10 (0x40)
        class accessory_trailer_body_data_u* accessory_trailer_body_data; //0x0E50 (0x08)
        char pad_0E58[ 104 ]; //0x0E58 (0x68)

        virtual void vfunc_86();
        virtual void vfunc_87();
        virtual void connect( vehicle_shared_u* connect_to, float3_t hook_vec, int a4, bool a5, bool a6 );
        virtual void disconnect();
        virtual void vfunc_90();
        virtual void set_trailer_brace( bool lowered );
    };

    static_assert(sizeof( physics_trailer_u ) == 0xEC0);

    class physics_trailer_actor_u : public physics_trailer_u // Size: 0x0EC0
    {
    public:
    };

    static_assert(sizeof( physics_trailer_actor_u ) == 0xEC0);

    class game_trailer_actor_u : public physics_trailer_actor_u // Size: 0x0FE8
    {
    public:
        class model_object_u* loading_model_object; //0x0EC0 (0x08)
        class eutpp_paint_u* paint; //0x0EC8 (0x08)
        char pad_0ED0[ 96 ]; //0x0ED0 (0x60)
        class eutpp_paint_u* paint_2; //0x0F30 (0x08)
        class eut2pp_truckpaint_u* truck_paint; //0x0F38 (0x08)
        char pad_0F40[ 56 ]; //0x0F40 (0x38)
        class pp_lamp_colors_u* lamp_colors; //0x0F78 (0x08)
        char pad_0F80[ 80 ]; //0x0F80 (0x50)
        class history_stream_set_u* history_stream; //0x0FD0 (0x08)
        char pad_0FD8[ 16 ]; //0x0FD8 (0x10)
    };

    static_assert(sizeof( game_trailer_actor_u ) == 0xFE8);
}

#pragma pack(pop)
