#pragma once
#include <string>
#include <map>
#include <memory>
#include "hooks/hook.hpp"

namespace ts_extra_utilities
{
    class CVirtualFunctionHook;
    class CFunctionHook;

    class CHooksManager
    {
    private:
        std::map< std::string, std::shared_ptr< CHook > > hooks_ = {};

    public:
        CHooksManager();
        ~CHooksManager();

        bool init();

        std::shared_ptr< CFunctionHook > register_function_hook( const std::string& name, uint64_t original_address, uint64_t hk_address );
        std::shared_ptr< CVirtualFunctionHook > register_virtual_function_hook( const std::string& name, uint64_t original_address, uint64_t hk_address );
        std::shared_ptr< CHook > get_hook( const std::string& name );
        std::map< std::string, CHook::Enum > get_hooks() const;
    };
}
