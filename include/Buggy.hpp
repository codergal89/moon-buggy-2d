#ifndef MOON_BUGGY_2D_BUGGY_HPP
#define MOON_BUGGY_2D_BUGGY_HPP

#include "TypedNodeCastMixin.hpp"

#include <Godot.hpp>
#include <RigidBody2D.hpp>

namespace moon_buggy
{

  struct Buggy
      : godot::RigidBody2D
      , TypedNodeCastMixin<Buggy>
  {
    auto static _register_methods() -> void;

    auto _init() -> void;

  private:
    GODOT_CLASS(Buggy, godot::RigidBody2D)
  };

}  // namespace moon_buggy

#endif