#ifndef MOON_BUGGY_2D_LEVEL_COMPLETE_HPP
#define MOON_BUGGY_2D_LEVEL_COMPLETE_HPP

#include "support/TypedNodeCastMixin.hpp"

#include <Control.hpp>
#include <Godot.hpp>
#include <Label.hpp>

namespace moon_buggy
{

  struct LevelComplete
      : godot::Control
      , TypedNodeCastMixin<LevelComplete>
  {
    auto static _register_methods() -> void;

    auto _init() -> void;

  private:
    GODOT_CLASS(LevelComplete, godot::Control)

    // Exported Methods
    auto start_button_clicked() -> void;
    auto set_level_number(int number) -> void;
    auto _ready() -> void;

    // Internal Variables
    godot::Label * level_number;
  };

}  // namespace moon_buggy

#endif