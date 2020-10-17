#ifndef MOON_BUGGY_2D_HUD_HPP
#define MOON_BUGGY_2D_HUD_HPP

#include "support/TypedNodeCastMixin.hpp"

#include <Control.hpp>
#include <Godot.hpp>
#include <Label.hpp>

namespace moon_buggy
{

  struct HUD
      : godot::Control
      , TypedNodeCastMixin<HUD>
  {
    auto static _register_methods() -> void;

    auto _init() -> void;
    auto _ready() -> void;

    auto set_level_number(int number) -> void;

    GODOT_CLASS(HUD, godot::Control)  // NOLINT

  private:
    godot::Label * level_number{};
  };

}  // namespace moon_buggy

#endif