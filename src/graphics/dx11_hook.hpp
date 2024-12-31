#pragma once

namespace ts_extra_utilities
{
    class CDirectX11Hook
    {
    public:
        ~CDirectX11Hook();

        bool hook_present();
        bool unhook_present();
    };
}
