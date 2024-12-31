#pragma once
#include "hook.hpp"

#include <utility>

namespace ts_extra_utilities
{
    class CFunctionHook : public CHook
    {
    public:
        CFunctionHook( std::string name, const uint64_t original_address, const uint64_t hk_address )
            : CHook( std::move( name ), original_address, hk_address )
        {
        }

        ~CFunctionHook() override;

        Enum hook() override;
        Enum create();
        Enum remove();
        Enum unhook() override;
    };
}
