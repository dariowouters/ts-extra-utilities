#pragma once

#include <memory>
#include <vector>

#include "windows/window.hpp"

namespace ts_extra_utilities
{
    class CWindowManager
    {
    private:
        std::vector< std::shared_ptr< CWindow > > windows_ = {};

    public:
        CWindowManager() = default;
        ~CWindowManager() = default;

        bool init();
        void render() const;

        template < typename T, std::enable_if_t< std::is_base_of_v< CWindow, T > && !std::is_same_v< CWindow, T >, int >  = 0 >
        std::shared_ptr< T > register_window( std::shared_ptr< T > window )
        {
            this->windows_.push_back( window );
            return window;
        }
    };
}
