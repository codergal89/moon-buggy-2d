#ifndef MOON_BUGGY_2D_SCROLL_CAMERA_HPP
#define MOON_BUGGY_2D_SCROLL_CAMERA_HPP

#include "TypedNodeCastMixin.hpp"

#include <Area2D.hpp>
#include <Camera2D.hpp>
#include <Defs.hpp>
#include <Godot.hpp>

namespace moon_buggy
{

  struct ScrollCamera
      : godot::Camera2D
      , TypedNodeCastMixin<ScrollCamera>
  {
    auto static constexpr default_speed{100.f};

    auto static _register_methods() -> void;

    auto _init() -> void;
    auto _ready() -> void;
    auto _physics_process(real_t delta) -> void;

    auto get_kill_zone() -> godot::Area2D *;

  private:
    GODOT_CLASS(ScrollCamera, godot::Camera2D)

    real_t speed{default_speed};
    bool should_scroll{};

    godot::Area2D * kill_zone;
  };

}  // namespace moon_buggy

#endif