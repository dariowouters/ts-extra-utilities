#pragma once
#include "window.hpp"

namespace ts_extra_utilities
{
    class CKeybinds : public CWindow
    {
    public:
        bool init() override;
        void render() override;
    };
}
