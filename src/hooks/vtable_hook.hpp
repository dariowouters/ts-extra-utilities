#pragma once
#include "hook.hpp"

#include <utility>

namespace ts_extra_utilities
{
    class CVirtualFunctionHook : public CHook
    {
    public:
        CVirtualFunctionHook( std::string name, const uint64_t original_address, const uint64_t hk_address )
            : CHook( std::move( name ), original_address, hk_address )
        {
        }

        ~CVirtualFunctionHook() override;

        Enum hook() override;
        Enum unhook() override;
    };
}
