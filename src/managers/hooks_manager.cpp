#include "hooks_manager.hpp"

#include "core.hpp"
#include "hooks/hook.hpp"
#include "imgui.h"
#include "hooks/function_hook.hpp"
#include "hooks/vtable_hook.hpp"

namespace ts_extra_utilities
{
    CHooksManager::CHooksManager()
    {
    }

    CHooksManager::~CHooksManager()
    {
        for ( auto& [ _, hook ] : this->hooks_ )
        {
            CCore::g_instance->debug( "[~HooksManager] Removing hook '{}'", hook->get_name() );
            hook.reset();
        }
    }

    bool CHooksManager::init()
    {
        return true;
    }

    std::shared_ptr< CFunctionHook > CHooksManager::register_function_hook( const std::string& name, uint64_t original_address, uint64_t hk_address )
    {
        auto hook = std::make_shared< CFunctionHook >( name, original_address, hk_address );
        CCore::g_instance->debug( "Adding hook '{}'", name );
        this->hooks_.emplace( name, hook );

        return hook;
    }

    std::shared_ptr< CVirtualFunctionHook > CHooksManager::register_virtual_function_hook( const std::string& name, uint64_t original_address, uint64_t hk_address )
    {
        auto hook = std::make_shared< CVirtualFunctionHook >( name, original_address, hk_address );
        CCore::g_instance->debug( "Adding vfunc hook '{}'", name );
        this->hooks_.emplace( name, hook );

        return hook;
    }

    std::shared_ptr< CHook > CHooksManager::get_hook( const std::string& name )
    {
        const auto hook = this->hooks_.find( name );
        if ( hook == this->hooks_.end() ) return nullptr;
        return hook->second;
    }

    std::map< std::string, CHook::Enum > CHooksManager::get_hooks() const
    {
        std::map< std::string, CHook::Enum > result = {};
        for ( const auto& [ name, hook ] : this->hooks_ )
        {
            result.emplace( name, hook->get_status() );
        }
        return result;
    }
}
