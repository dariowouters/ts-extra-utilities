#pragma once
#include <cstdint>

#include "window.hpp"
#include "prism/functions.hpp"

namespace ts_extra_utilities
{
    // Only manual trailer steering is ported to 1.61. Individually detachable trailers and
    // lockable joints depend on many more struct offsets and vfunc slots that have not been
    // re-verified, so they are disabled rather than left in a state that writes to stale offsets.
    class CTrailerManipulation : public CWindow
    {
    private:
        bool valid_ = false;

        void render_trailer_steering( uint32_t i ) const;

    public:
        CTrailerManipulation();
        ~CTrailerManipulation() override;

        bool init() override;
        // every frame, also while the window is hidden: refreshes the trailer chain and applies steering hotkeys
        void update( float dt );
        void render() override;
    };
}
