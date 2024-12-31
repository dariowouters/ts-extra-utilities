#pragma once

namespace ts_extra_utilities
{
    class CWindow
    {
    protected:
        bool open_ = true;

    public:
        CWindow() = default;
        virtual ~CWindow() = default;

        virtual bool init() = 0;
        virtual void render() = 0;

        void show();
        void hide();

        bool is_open() const
        {
            return this->open_;
        }
    };
}
