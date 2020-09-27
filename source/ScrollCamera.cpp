#include "ScrollCamera.hpp"

#include "Buggy.hpp"

#include <Area2D.hpp>
#include <Defs.hpp>
#include <Godot.hpp>
#include <Vector2.hpp>
#include <Viewport.hpp>

namespace moon_buggy
{

  auto ScrollCamera::_register_methods() -> void
  {
    godot::register_method("_ready", &ScrollCamera::_ready);
    godot::register_method("_physics_process", &ScrollCamera::_physics_process);
    godot::register_property("speed", &ScrollCamera::speed, default_speed);
    godot::register_property("should_scroll", &ScrollCamera::should_scroll, false);
  }

  auto ScrollCamera::_init() -> void
  {
  }

  auto ScrollCamera::_ready() -> void
  {
    auto viewport_size = get_viewport()->get_size();
    set("limit_top", 0);
    set("limit_bottom", viewport_size.y);
    set_position({viewport_size.x / 2, viewport_size.y / 2});

    kill_zone = get_typed_node<godot::Area2D>("KillZone");
    kill_zone->set_position({.0f, viewport_size.y / 2});
  }

  auto ScrollCamera::_physics_process(real_t delta) -> void
  {
    if (should_scroll)
    {
      auto current_position = get_position();
      set_position(current_position + godot::Vector2{-1, 0} * speed * delta);
    }
  }

  auto ScrollCamera::get_kill_zone() -> godot::Area2D *
  {
    return kill_zone;
  }

}  // namespace moon_buggy