#include "ScrollCamera.hpp"

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
  }

  auto ScrollCamera::_init() -> void
  {
  }

  auto ScrollCamera::_ready() -> void
  {
    auto viewport = get_viewport();
    set("limit_top", 0);
    set("limit_bottom", viewport->get_size().y);
  }

  auto ScrollCamera::_physics_process(real_t delta) -> void
  {
    auto current_position = get_position();
    set_position(current_position + godot::Vector2{-1, 0} * speed * delta);
  }

}  // namespace moon_buggy