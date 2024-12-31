#pragma once
#include <string>

namespace ts_extra_utilities
{
    class CHook
    {
    public:
        enum Enum
        {
            UNHOOKED,
            CREATED,
            HOOKED,
            FAILED,
        };

    protected:
        std::string name_;
        Enum status_ = UNHOOKED;

        uint64_t original_address_ = 0;
        uint64_t original_fn_ = 0;
        uint64_t hk_address_ = 0;

    public:
        CHook( std::string name, uint64_t original_address, uint64_t hk_address );

        virtual ~CHook() = default;

        virtual Enum hook() = 0;
        virtual Enum unhook() = 0;

        template < typename T >
        T* get_original()
        {
            return reinterpret_cast< T* >( this->original_fn_ );
        }

        const std::string& get_name() const { return name_; }
        Enum get_status() const { return status_; }
    };
}
