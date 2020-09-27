#ifndef MOON_BUGGY_2D_BUGGY_HPP
#define MOON_BUGGY_2D_BUGGY_HPP

#include "TypedNodeCastMixin.hpp"

#include <Defs.hpp>
#include <Godot.hpp>
#include <KinematicBody2D.hpp>
#include <Vector2.hpp>

#include <functional>
#include <map>

namespace moon_buggy
{

  struct Buggy
      : godot::KinematicBody2D
      , TypedNodeCastMixin<Buggy>
  {
    auto static constexpr default_acceleration{10.f};
    auto static constexpr default_drag{100.f};
    auto static constexpr default_jump_velocity{98.f};
    auto static constexpr default_speed_limit{100.f};

    auto static _register_methods() -> void;

    auto _init() -> void;
    auto _ready() -> void;
    auto _physics_process(float delta) -> void;

  private:
    GODOT_CLASS(Buggy, godot::KinematicBody2D)

    using input_action = std::function<void()>;

    auto handle_gravity(real_t delta) -> void;
    auto handle_drag(real_t delta) -> void;
    auto handle_input() -> void;

    auto accelerate() -> void;
    auto decelerate() -> void;
    auto jump() -> void;

    auto kill_zone_entered(godot::Node * node) -> void;

    std::map<char const *, input_action> floor_actions{};

    real_t acceleration{default_acceleration};
    real_t drag{default_drag};
    real_t jump_velocity{default_jump_velocity};
    real_t speed_limit{default_speed_limit};

    godot::Vector2 velocity{};
    godot::Vector2 gravity{};
  };

}  // namespace moon_buggy

#endif