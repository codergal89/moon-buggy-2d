#ifndef MOON_BUGGY_2D_MAIN_MENU
#define MOON_BUGGY_2D_MAIN_MENU

#include "core/Fwd.hpp"
#include "support/TypedNodeCastMixin.hpp"

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

  private:
    GODOT_CLASS(MainMenu, godot::Control)  // NOLINT

    /// Exported Methods
    auto _ready() -> void;

    auto on_start_button_clicked() -> void;
    auto on_visibility_changed() -> void;

    /// Internal Variables
    MeteorSpawner * meteor_spawner{};
  };

}  // namespace moon_buggy

#endif