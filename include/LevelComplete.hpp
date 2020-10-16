#ifndef MOON_BUGGY_2D_LEVEL_COMPLETE_HPP
#define MOON_BUGGY_2D_LEVEL_COMPLETE_HPP

#include "TypedNodeCastMixin.hpp"

#include <Control.hpp>
#include <Godot.hpp>

namespace moon_buggy
{

  struct LevelComplete
      : godot::Control
      , TypedNodeCastMixin<LevelComplete>
  {
    auto static _register_methods() -> void;

    auto _init() -> void;
    auto _ready() -> void;

    GODOT_CLASS(LevelComplete, godot::Control)  // NOLINT
  };

}  // namespace moon_buggy

#endif