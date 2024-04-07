#include "movement.h"

using namespace godot;

void Movement::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_speed"), &Movement::get_speed);
    ClassDB::bind_method(D_METHOD("set_speed", "val"), &Movement::set_speed);
    ClassDB::add_property("Movement", PropertyInfo(Variant::FLOAT, "speed"),
                          "set_speed", "get_speed");
    ClassDB::bind_method(D_METHOD("get_max_velocity"), &Movement::get_max_velocity);
    ClassDB::bind_method(D_METHOD("set_max_velocity", "val"), &Movement::set_max_velocity);
    ClassDB::add_property("Movement", PropertyInfo(Variant::FLOAT, "max_velocity"),
                          "set_max_velocity", "get_max_velocity");
    ClassDB::bind_method(D_METHOD("get_jump_height"), &Movement::get_jump_height);
    ClassDB::bind_method(D_METHOD("set_jump_height", "height"), &Movement::set_jump_height);
    ClassDB::add_property("Movement", PropertyInfo(Variant::FLOAT, "jump_height"),
                          "set_jump_height", "get_jump_height");
    ClassDB::bind_method(D_METHOD("get_jump_time_to_peak"), &Movement::get_jump_time_to_peak);
    ClassDB::bind_method(D_METHOD("set_jump_time_to_peak", "val"), &Movement::set_jump_time_to_peak);
    ClassDB::add_property("Movement", PropertyInfo(Variant::FLOAT, "jump_time_to_peak"),
                          "set_jump_time_to_peak", "get_jump_time_to_peak");
    ClassDB::bind_method(D_METHOD("get_jump_time_to_descent"), &Movement::get_jump_time_to_descent);
    ClassDB::bind_method(D_METHOD("set_jump_time_to_descent", "val"), &Movement::set_jump_time_to_descent);
    ClassDB::add_property("Movement", PropertyInfo(Variant::FLOAT, "jump_time_to_descent"),
                          "set_jump_time_to_descent", "get_jump_time_to_descent");
    ClassDB::bind_method(D_METHOD("get_jump_buffer_time"), &Movement::get_jump_buffer_time);
    ClassDB::bind_method(D_METHOD("set_jump_buffer_time", "val"), &Movement::set_jump_buffer_time);
    ClassDB::add_property("Movement", PropertyInfo(Variant::FLOAT, "jump_buffer_time"),
                          "set_jump_buffer_time", "get_jump_buffer_time");
    ClassDB::bind_method(D_METHOD("get_jump_coyote_time"), &Movement::get_jump_coyote_time);
    ClassDB::bind_method(D_METHOD("set_jump_coyote_time", "val"), &Movement::set_jump_coyote_time);
    ClassDB::add_property("Movement", PropertyInfo(Variant::FLOAT, "jump_coyote_time"),
                          "set_jump_coyote_time", "get_jump_coyote_time");
    ClassDB::bind_method(D_METHOD("get_jump_gravity"), &Movement::get_jump_gravity);
    ClassDB::bind_method(D_METHOD("get_fall_gravity"), &Movement::get_fall_gravity);
    ClassDB::bind_method(D_METHOD("get_current_gravity"), &Movement::get_current_gravity);
    ClassDB::bind_method(D_METHOD("get_interval_since_jump_pressed"), &Movement::get_interval_since_jump_pressed);
    ClassDB::bind_method(D_METHOD("get_interval_since_on_floor"), &Movement::get_interval_since_on_floor);
    ClassDB::bind_method(D_METHOD("get_direction"), &Movement::get_direction);
}

Movement::Movement() {
    if (Engine::get_singleton()->is_editor_hint())
        set_process_mode(ProcessMode::PROCESS_MODE_DISABLED);

    _body = nullptr;

    _speed = 10.0;
    
    _jump_height = 49.0;
    _jump_time_to_peak = 1.0;
    _jump_time_to_descent = 1.0;
    _jump_buffer_time = 0.1;
    _jump_coyote_time = 0.1;

    _max_velocity = 4200;
    _jump_gravity = 2.0f * _jump_height / powf(_jump_time_to_peak, 2.0);
    _fall_gravity = 2.0f * _jump_height / powf(_jump_time_to_descent, 2.0);

    _interval_since_jump_pressed = 0.0;
    _interval_since_on_floor = 0.0;
    _default_delta = 1.0f / static_cast<float>(Engine::get_singleton()->get_physics_ticks_per_second());
    _direction = 0;
}

Movement::~Movement() = default;

void Movement::_ready() {
    _body = cast_to<CharacterBody2D>(get_parent());
}

void Movement::_physics_process(double delta) {
    auto delta_f = static_cast<float>(delta);

    if (_body->is_on_floor()) {
        if (_interval_since_jump_pressed < _jump_buffer_time)
            _try_jump(false);
        _interval_since_on_floor = 0.0;
    }
    else {
        _interval_since_on_floor += delta_f;
        _interval_since_jump_pressed += delta_f;
    }

    _calculate_velocity();
}

void Movement::move(int direction) {
    if (-2 < direction && direction < 2) {
        if (direction * -1 == _direction)
            _flip();
        _direction = direction;
    }
}

void Movement::_try_jump(bool pressed) {
    if (_body->is_on_floor() || _interval_since_on_floor < _jump_coyote_time) {
        auto velocity = Vector2(_body->get_velocity().x,
                                -1.0f * _jump_gravity * _jump_time_to_peak);
        _body->set_velocity(velocity);
    }

    if (pressed)
        _interval_since_jump_pressed = 0.0;
}

void Movement::_calculate_velocity() {
    auto delta = static_cast<float>(get_physics_process_delta_time());
    auto velocity = Vector2(_speed * static_cast<float>(_direction),
                            _body->get_velocity().y + get_current_gravity() * delta);
    velocity *= delta / _default_delta;

    if(velocity.length() > _max_velocity)
        velocity = velocity.normalized() * _max_velocity;

    _body->set_velocity(velocity);
    _body->move_and_slide();
}

void Movement::_flip() {
    auto new_scale = get_scale();
    new_scale.x *= -1;
    set_scale(new_scale);
}

#pragma region getset

float Movement::get_speed() const { return _speed; }
void Movement::set_speed(float val) {
    if (val >= 0)
        _speed = val;
}

float Movement::get_max_velocity() const { return _max_velocity; }
void Movement::set_max_velocity(float val) {
    if (val >= 0)
        _max_velocity = val;
}

float Movement::get_jump_height() const { return _jump_height; }
void Movement::set_jump_height(float height) {
    if (height >= 0)
        _jump_height = height;
}

float Movement::get_jump_time_to_peak() const { return _jump_time_to_peak; }
void Movement::set_jump_time_to_peak(float val) {
    if (val >= 0)
        _jump_time_to_peak = val;
}

float Movement::get_jump_time_to_descent() const { return _jump_time_to_descent; }
void Movement::set_jump_time_to_descent(float val) {
    if (val >= 0)
        _jump_time_to_descent = val;
}

float Movement::get_jump_buffer_time() const { return _jump_buffer_time; }
void Movement::set_jump_buffer_time(float val) {
    if (val >= 0)
        _jump_buffer_time = val;
}

float Movement::get_jump_coyote_time() const { return _jump_coyote_time; }
void Movement::set_jump_coyote_time(float val) {
    if (val >= 0)
        _jump_coyote_time = val;
}

float Movement::get_jump_gravity() const { return _jump_gravity; }

float Movement::get_fall_gravity() const { return _fall_gravity; }

float Movement::get_current_gravity() const {
    return _body->get_velocity().y < 0.0 ? _jump_gravity : _fall_gravity;
}

float Movement::get_interval_since_jump_pressed() const { return _interval_since_jump_pressed; }

float Movement::get_interval_since_on_floor() const { return _interval_since_on_floor; }

int Movement::get_direction() const {return _direction; }

#pragma endregion getset