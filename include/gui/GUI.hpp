#ifndef MOON_BUGGY_2D_GUI_GUI_HPP
#define MOON_BUGGY_2D_GUI_GUI_HPP

#include "support/TypedNodeCastMixin.hpp"

#include <CanvasLayer.hpp>
#include <Godot.hpp>

namespace moon_buggy
{

  struct GUI
      : godot::CanvasLayer
      , TypedNodeCastMixin<godot::CanvasLayer>
  {
    auto static _register_methods() -> void;

    auto _init() -> void;

  private:
    GODOT_CLASS(GUI, godot::CanvasLayer)
  };

}  // namespace moon_buggy

#endif