#ifndef MOON_BUGGY_2D_GUI_GUI_HPP
#define MOON_BUGGY_2D_GUI_GUI_HPP

#include "gui/HUD.hpp"
#include "gui/LevelComplete.hpp"
#include "gui/MainMenu.hpp"
#include "support/TypedNodeCastMixin.hpp"

#include <CanvasLayer.hpp>
#include <Godot.hpp>

namespace moon_buggy
{

  struct GUI
      : godot::CanvasLayer
      , TypedNodeCastMixin<GUI>
  {
    auto static _register_methods() -> void;

    auto _init() -> void;

  private:
    GODOT_CLASS(GUI, godot::CanvasLayer)

    /// Exported Methods
    auto _ready() -> void;

    auto set_level_number(int number) -> void;

    auto show_hud() -> void;
    auto show_level_complete_screen() -> void;
    auto show_main_menu() -> void;

    /// Internal Functions
    auto hide_all_layers() -> void;

    /// Internal Variables
    HUD * hud;
    LevelComplete * level_complete_screen;
    MainMenu * main_menu;
  };

}  // namespace moon_buggy

#endif