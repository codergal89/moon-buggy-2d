#ifndef MOON_BUGGY_2D_BUGGY_HPP
#define MOON_BUGGY_2D_BUGGY_HPP

#include "TypedNodeCastMixin.hpp"

#include <Defs.hpp>
#include <Godot.hpp>
#include <KinematicBody2D.hpp>
#include <Vector2.hpp>

namespace moon_buggy
{

  struct Buggy
      : godot::KinematicBody2D
      , TypedNodeCastMixin<Buggy>
  {
    auto static constexpr default_speed_limit{100.f};

    auto static _register_methods() -> void;

    auto _init() -> void;
    auto _ready() -> void;
    auto _physics_process(float delta) -> void;

  private:
    GODOT_CLASS(Buggy, godot::KinematicBody2D)

    auto handle_gravity(real_t delta) -> void;
    auto handle_input() -> void;
    auto accelerate(real_t difference) -> void;
    auto decelerate(real_t difference) -> void;

    real_t speed_limit{};

    godot::Vector2 velocity{};
    godot::Vector2 gravity{};
  };

}  // namespace moon_buggy

#endif