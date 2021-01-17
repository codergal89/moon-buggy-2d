#ifndef MOON_BUGGY_2D_GAME_METEORS_HPP
#define MOON_BUGGY_2D_GAME_METEORS_HPP

#include "support/TypedNodeCastMixin.hpp"

#include <Godot.hpp>
#include <Node2D.hpp>
#include <Timer.hpp>

namespace moon_buggy
{

  struct Meteors
      : godot::Node2D
      , TypedNodeCastMixin<Meteors>
  {
    auto static _register_methods() -> void;

    auto _init() -> void;

  private:
    GODOT_CLASS(Meteors, godot::Node2D)  // NOLINT

    /// Exported Methods
    auto _ready() -> void;

    /// Internal Variables
    godot::Timer * spawn_timer;
  };

}  // namespace moon_buggy

#endif