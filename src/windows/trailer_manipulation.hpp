#pragma once
#include <cstdint>

#include "window.hpp"
#include "prism/functions.hpp"

namespace prism
{
    class game_trailer_actor_u;
}

namespace ts_extra_utilities
{
    // TODO: get dynamic offsets for game_trailer_actor, slave_trailer, wheel_steering_stuff
    // TODO: When disconnected: figure out 3rd person camera / trailer cables / disable lights / etc...
    class CTrailerManipulation : public CWindow
    {
    private:
        bool valid_ = false;
        prism::set_individual_steering_fn* set_individual_steering_fn_ = nullptr;
        prism::physics_trailer_u_get_slave_hook_position_fn* get_slave_hook_position_fn_ = nullptr;

        void render_trailer_steering( prism::game_trailer_actor_u* current_trailer, uint32_t i ) const;
        void connect_trailer( prism::game_trailer_actor_u* current_trailer, uint32_t i ) const;
        void render_trailer_joint( prism::game_trailer_actor_u* current_trailer, uint32_t i ) const;
        void render_trailers() const;

    public:
        CTrailerManipulation();
        ~CTrailerManipulation() override;

        bool init() override;
        void render() override;
    };
}
