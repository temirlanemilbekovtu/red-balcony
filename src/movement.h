#pragma once

#ifndef RED_BALCONY_MOVEMENT_H
#define RED_BALCONY_MOVEMENT_H

#include "godot_cpp/classes/node2d.hpp"
#include "godot_cpp/classes/character_body2d.hpp"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/variant/node_path.hpp"
#include "godot_cpp/variant/utility_functions.hpp"

namespace godot {

    class Movement : public Node2D {
        GDCLASS(Movement, Node2D)

    private:
        CharacterBody2D *_body;

        float _speed;
        float _max_velocity;

        float _jump_height;
        float _jump_time_to_peak;
        float _jump_time_to_descent;
        float _jump_buffer_time;
        float _jump_coyote_time;

        float _jump_gravity;
        float _fall_gravity;

        float _interval_since_jump_pressed;
        float _interval_since_on_floor;
        float _default_delta;
        int _direction;

        void _calculate_velocity();
        void _flip();

    protected:
        static void _bind_methods();

    public:
        Movement();
        ~Movement() override;

        void _ready() override;
        void _physics_process(double delta) override;

        void move(int direction);
        void _try_jump(bool pressed = true);

        [[nodiscard]] float get_speed() const;
        void set_speed(float val);
        [[nodiscard]]float get_max_velocity() const;
        void set_max_velocity(float val);

        [[nodiscard]]float get_jump_height() const;
        void set_jump_height(float val);
        [[nodiscard]]float get_jump_time_to_peak() const;
        void set_jump_time_to_peak(float val);
        [[nodiscard]]float get_jump_time_to_descent() const;
        void set_jump_time_to_descent(float val);
        [[nodiscard]]float get_jump_buffer_time() const;
        void set_jump_buffer_time(float val);
        [[nodiscard]]float get_jump_coyote_time() const;
        void set_jump_coyote_time(float val);

        [[nodiscard]]float get_jump_gravity() const;
        [[nodiscard]]float get_fall_gravity() const;
        [[nodiscard]]float get_current_gravity() const;

        [[nodiscard]]float get_interval_since_jump_pressed() const;
        [[nodiscard]]float get_interval_since_on_floor() const;
        [[nodiscard]]int get_direction() const;
    };
    
}

#endif //RED_BALCONY_MOVEMENT_H
