#include "hook.hpp"

namespace ts_extra_utilities
{
    CHook::CHook( std::string name, const uint64_t original_address, const uint64_t hk_address ) : name_( std::move( name ) ), original_address_( original_address ), hk_address_( hk_address )
    {
    }
}
