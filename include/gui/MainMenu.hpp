#ifndef MOON_BUGGY_2D_MAIN_MENU
#define MOON_BUGGY_2D_MAIN_MENU

#include "support/TypedNodeCastMixin.hpp"

#include <Control.hpp>
#include <Godot.hpp>
#include <Defs.hpp>
#include <ParallaxBackground.hpp>

namespace moon_buggy
{

  struct MainMenu
      : godot::Control
      , TypedNodeCastMixin<MainMenu>
  {
    auto static _register_methods() -> void;

    auto _init() -> void;

  private:
    GODOT_CLASS(MainMenu, godot::Control)  // NOLINT (modernize-trailing-return-type)

    /// Exported Methods
    auto _process(real_t delta) -> void;
    auto _ready() -> void;

    auto on_start_button_clicked() -> void;

    /// Exported Properties
    real_t background_scroll_speed;

    /// Internal Variables
    godot::ParallaxBackground *  background;
  };

}  // namespace moon_buggy

#endif