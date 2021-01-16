#ifndef MOON_BUGGY_2D_GAME_METEOR_HPP
#define MOON_BUGGY_2D_GAME_METEOR_HPP

#include "support/TypedNodeCastMixin.hpp"

#include <AnimatedSprite.hpp>
#include <Godot.hpp>
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

    godot::AnimatedSprite * sprite;
  };

}  // namespace moon_buggy

#endif