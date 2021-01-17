#ifndef MOON_BUGGY_2D_GUI_BUGGY_CRASHED_HPP
#define MOON_BUGGY_2D_GUI_BUGGY_CRASHED_HPP

#include "support/TypedNodeCastMixin.hpp"

#include <Control.hpp>
#include <Godot.hpp>
#include <Label.hpp>
#include <String.hpp>

namespace moon_buggy
{

  struct BuggyCrashed
      : godot::Control
      , TypedNodeCastMixin<BuggyCrashed>
  {
    auto static _register_methods() -> void;

    auto _init() -> void;
    auto _ready() -> void;

    auto set_crash_reason(godot::String reason) -> void;

    GODOT_CLASS(BuggyCrashed, godot::Control)  // NOLINT

  private:
    auto retry_button_clicked() -> void;

    godot::Label * crash_reason;
  };

}  // namespace moon_buggy

#endif