#include "vtable_hook.hpp"

#include "core.hpp"

namespace ts_extra_utilities
{
    CVirtualFunctionHook::~CVirtualFunctionHook()
    {
        this->unhook();
    }

    CHook::Enum CVirtualFunctionHook::hook()
    {
        CCore::g_instance->debug( "Hooking vfunc '{}' @ {:x}", this->name_, this->original_address_ );

        this->original_fn_ = *reinterpret_cast< uint64_t* >( this->original_address_ );

        DWORD old_protect;
        if ( !VirtualProtect( reinterpret_cast< LPVOID >( this->original_address_ ), 8, PAGE_EXECUTE_READWRITE, &old_protect ) )
        {
            CCore::g_instance->error( "Could not change protection for the '{}' virtual function", this->name_ );
            return this->status_;
        }
        *reinterpret_cast< uint64_t* >( this->original_address_ ) = this->hk_address_;
        VirtualProtect( reinterpret_cast< LPVOID >( this->original_address_ ), 8, old_protect, nullptr );

        this->status_ = HOOKED;
        return this->status_;
    }

    CHook::Enum CVirtualFunctionHook::unhook()
    {
        if ( this->status_ != HOOKED )
        {
            CCore::g_instance->debug( "Attempted to unhook unhooked vfunc '{}' @ {:x}", this->name_, this->original_address_ );
            return this->status_;
        }

        CCore::g_instance->debug( "Unhooking vfunc '{}' @ {:x}", this->name_, this->original_address_ );

        DWORD old_protect;
        if ( !VirtualProtect( reinterpret_cast< LPVOID >( this->original_address_ ), 8, PAGE_EXECUTE_READWRITE, &old_protect ) )
        {
            CCore::g_instance->error( "Could not change protection for unhooking the '{}' virtual function", this->name_ );
            return this->status_;
        }

        *reinterpret_cast< uint64_t* >( this->original_address_ ) = this->original_fn_;
        VirtualProtect( reinterpret_cast< LPVOID >( this->original_address_ ), 8, old_protect, nullptr );

        this->status_ = UNHOOKED;
        return this->status_;
    }
}
