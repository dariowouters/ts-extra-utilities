#pragma once
#include <cstdint>

#pragma pack(push, 1)
namespace ts_extra_utilities::prism
{
    class unit_t
    {
    public:
        uint32_t unk;
        uint32_t unk2;

    public:
        virtual void destructor();
        virtual void destroy();
        virtual void clone();
        virtual void get_unit_descriptor();
        virtual void vfunc_4();
        virtual void vfunc_5();
        virtual void pre_save();
        virtual void post_save();
        virtual void pre_load();
        virtual void post_load();
    };

    static_assert(sizeof( unit_t ) == 0x10, "unit_t is incorrect in size");
}
#pragma pack(pop)
