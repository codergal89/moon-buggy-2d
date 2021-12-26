#ifndef MOON_BUGGY_2D_MAIN_MENU
#define MOON_BUGGY_2D_MAIN_MENU

#include "support/SilentGodotClass.hpp"
#include "support/TypedNodeCastMixin.hpp"

#include <Defs.hpp>
#include <Godot.hpp>
#include <Node.hpp>
#include <RandomNumberGenerator.hpp>
#include <Ref.hpp>

namespace moon_buggy
{

  struct MainMenu
      : godot::Node
      , TypedNodeCastMixin<MainMenu>
  {
    auto static _register_methods() -> void;

    auto _init() -> void;

  private:
    SILENT_GODOT_CLASS(MainMenu, godot::Node)

    /// Exported Methods
    auto _ready() -> void;

    auto on_start_button_clicked() -> void;
  };

}  // namespace moon_buggy

#endif