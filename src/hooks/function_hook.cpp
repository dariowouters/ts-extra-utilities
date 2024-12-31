#include "function_hook.hpp"

#include <MinHook.h>

#include "core.hpp"

namespace ts_extra_utilities
{
    CFunctionHook::~CFunctionHook()
    {
        this->remove();
    }

    CFunctionHook::Enum CFunctionHook::create()
    {
        if (
            const auto res = MH_CreateHook( reinterpret_cast< LPVOID >( this->original_address_ ),
                                            reinterpret_cast< LPVOID >( this->hk_address_ ),
                                            reinterpret_cast< LPVOID* >( this->original_fn_ ) );
            res != MH_OK && res != MH_ERROR_ALREADY_CREATED )
        {
            CCore::g_instance->error( "Could not create '{}' hook: {}", this->name_, static_cast< int32_t >( res ) );
            this->status_ = FAILED;
            return this->status_;
        }
        this->status_ = CREATED;
        return this->status_;
    }

    CHook::Enum CFunctionHook::hook()
    {
        CCore::g_instance->debug( "Hooking '{}' @ {:x}", this->name_, this->original_address_ );

        if ( this->status_ == UNHOOKED )
        {
            this->create();
        }

        if ( this->status_ == FAILED )
        {
            CCore::g_instance->error( "Status is FAILED, could not enable '{}' hook", this->name_ );
            return this->status_;
        }

        if (
            const auto res = MH_EnableHook( reinterpret_cast< LPVOID >( this->original_address_ ) );
            res != MH_OK
        )
        {
            CCore::g_instance->error( "Could not enable '{}' hook: {}", this->name_, static_cast< int32_t >( res ) );
            this->status_ = FAILED;
            return this->status_;
        }
        this->status_ = HOOKED;
        return this->status_;
    }

    CHook::Enum CFunctionHook::unhook()
    {
        CCore::g_instance->debug( "Unhooking '{}' @ {:x}", this->name_, this->original_address_ );
        if (
            const auto res = MH_DisableHook( reinterpret_cast< LPVOID >( this->original_address_ ) );
            res != MH_OK && res != MH_ERROR_NOT_CREATED
        )
        {
            CCore::g_instance->error( "Could not disable '{}' hook: {}", this->name_, static_cast< int32_t >( res ) );
            this->status_ = FAILED;
            return this->status_;
        }

        this->status_ = CREATED;
        return this->status_;
    }

    CFunctionHook::Enum CFunctionHook::remove()
    {
        if (
            const auto res = MH_RemoveHook( reinterpret_cast< LPVOID >( this->original_address_ ) );
            res != MH_OK && res != MH_ERROR_NOT_CREATED
        )
        {
            CCore::g_instance->error( "Could not remove '{}' hook: {}", this->name_, static_cast< int32_t >( res ) );
            this->status_ = FAILED;
            return this->status_;
        }
        this->status_ = UNHOOKED;
        return this->status_;
    }
}
