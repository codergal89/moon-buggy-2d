#ifndef MOON_BUGGY_2D_CORE_METEOR_HPP
#define MOON_BUGGY_2D_CORE_METEOR_HPP

#include "support/TypedNodeCastMixin.hpp"

#include <AnimatedSprite.hpp>
#include <Defs.hpp>
#include <Godot.hpp>
#include <Node.hpp>
#include <Physics2DDirectBodyState.hpp>
#include <RigidBody2D.hpp>

namespace moon_buggy
{

  struct Meteor
      : godot::RigidBody2D
      , TypedNodeCastMixin<Meteor>
  {
    auto static _register_methods() -> void;

    auto _init() -> void;
    auto _ready() -> void;

  private:
    GODOT_CLASS(Meteor, godot::RigidBody2D)  // NOLINT

    auto _integrate_forces(godot::Physics2DDirectBodyState * body_state) -> void;
    auto on_screen_exited() -> void;
    auto on_body_entered(godot::Node * body) -> void;

    real_t entry_angle{};
    real_t entry_speed{};
    godot::AnimatedSprite * sprite;
  };

}  // namespace moon_buggy

#endif