#pragma once

namespace ts_extra_utilities
{
    class CDirectInput8Hook
    {
    public:
        ~CDirectInput8Hook();

        bool hook();
        bool unhook();
    };
}
