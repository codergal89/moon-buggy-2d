#ifndef MOON_BUGGY_2D_MAIN_MENU
#define MOON_BUGGY_2D_MAIN_MENU

#include "TypedNodeCastMixin.hpp"

#include <Button.hpp>
#include <Control.hpp>
#include <Godot.hpp>
#include <Label.hpp>

namespace moon_buggy
{

  struct MainMenu
      : godot::Control
      , TypedNodeCastMixin<MainMenu>
  {
    auto static _register_methods() -> void;

    auto _init() -> void;
    auto _ready() -> void;

  private:
    GODOT_CLASS(MainMenu, godot::Control)  // NOLINT

    auto start_button_clicked() -> void;

    godot::Label * title;
    godot::Button * start_button;
  };

}  // namespace moon_buggy

#endif