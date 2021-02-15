#ifndef MOON_BUGGY_2D_CORE_SCROLL_CAMERA_HPP
#define MOON_BUGGY_2D_CORE_SCROLL_CAMERA_HPP

#include "support/TypedNodeCastMixin.hpp"

#include <Camera2D.hpp>
#include <Defs.hpp>
#include <Godot.hpp>

namespace moon_buggy
{

  struct ScrollCamera
      : godot::Camera2D
      , TypedNodeCastMixin<ScrollCamera>
  {
    auto static _register_methods() -> void;

    auto _init() -> void;

  private:
    GODOT_CLASS(ScrollCamera, godot::Camera2D)  // NOLINT

    // Exported Methods
    auto _physics_process(real_t delta) -> void;

    // Exported Properties
    real_t speed{};
    bool should_scroll{};
  };

}  // namespace moon_buggy

#endif