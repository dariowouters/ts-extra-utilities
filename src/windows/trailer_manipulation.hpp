#pragma once
#include <cstdint>

#include "window.hpp"
#include "prism/functions.hpp"

namespace ts_extra_utilities
{
    // Manual trailer steering and individually detachable trailers are ported to 1.61
    // (connect/disconnect go through vfunc slots verified against the native attach path).
    // Lockable PhysX joints are still disabled; their offsets were not re-verified.
    class CTrailerManipulation : public CWindow
    {
    private:
        bool valid_ = false;
        bool connect_valid_ = false; // connect/disconnect needs two extra hooks; tracked separately

        void refresh_trailer_chain();
        void update_convoy_state();
        void gc_detached_marks();
        void update_steering( float dt );
        void render_trailer_steering( uint32_t i ) const;
        void render_trailer_connect( uint32_t i ) const;
        void connect_trailer( uint32_t i ) const;
        void disconnect_trailer( uint32_t i ) const;
        void toggle_attach( uint32_t i ) const;
        void connect_to_truck( uint32_t i ) const;

    public:
        CTrailerManipulation();
        ~CTrailerManipulation() override;

        bool init() override;
        // every frame, also while the window is hidden: refreshes the trailer chain and applies steering hotkeys
        void update( float dt );
        void render() override;
    };
}
