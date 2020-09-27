#ifndef MOON_BUGGY_2D_MAIN_MENU
#define MOON_BUGGY_2D_MAIN_MENU

#include "TypedNodeCastMixin.hpp"

#include <Button.hpp>
#include <CanvasLayer.hpp>
#include <Godot.hpp>
#include <Label.hpp>

namespace moon_buggy
{

  struct MainMenu
      : godot::CanvasLayer
      , TypedNodeCastMixin<MainMenu>
  {
    auto static _register_methods() -> void;

    auto _init() -> void;
    auto _ready() -> void;

    auto show() -> void;
    auto hide() -> void;

  private:
    GODOT_CLASS(MainMenu, godot::CanvasLayer)

    auto start_button_clicked() -> void;

    godot::Label * title;
    godot::Button * start_button;
  };

}  // namespace moon_buggy

#endif