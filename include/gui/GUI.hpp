#ifndef MOON_BUGGY_2D_GUI_GUI_HPP
#define MOON_BUGGY_2D_GUI_GUI_HPP

#include "gui/Fwd.hpp"
#include "support/SilentGodotClass.hpp"
#include "support/TypedNodeCastMixin.hpp"

#include <Camera2D.hpp>
#include <CanvasLayer.hpp>
#include <Defs.hpp>
#include <Godot.hpp>
#include <Ref.hpp>
#include <PackedScene.hpp>

namespace moon_buggy
{

  struct GUI
      : godot::CanvasLayer
      , TypedNodeCastMixin<GUI>
  {
    auto static _register_methods() -> void;

    auto _init() -> void;

  private:
    SILENT_GODOT_CLASS(GUI, godot::CanvasLayer)

    /// Exported Methods
    auto _ready() -> void;

    auto show_main_menu() -> void;

    /// Internal Functions
    auto hide_all_layers() -> void;

    /// Exported Properties
    godot::Ref<godot::PackedScene> main_menu{};
  };

}  // namespace moon_buggy

#endif