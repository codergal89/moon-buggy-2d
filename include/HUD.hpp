#ifndef MOON_BUGGY_2D_HUD_HPP
#define MOON_BUGGY_2D_HUD_HPP

#include "TypedNodeCastMixin.hpp"

#include <Control.hpp>
#include <Godot.hpp>

namespace moon_buggy
{

  struct HUD
      : godot::Control
      , TypedNodeCastMixin<HUD>
  {
    auto static _register_methods() -> void;

    auto _init() -> void;

    GODOT_CLASS(HUD, godot::Control)  // NOLINT
  };

}  // namespace moon_buggy

#endif