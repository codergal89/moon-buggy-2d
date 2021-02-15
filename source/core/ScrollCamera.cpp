#include "core/ScrollCamera.hpp"

#include <Defs.hpp>
#include <Godot.hpp>
#include <Vector2.hpp>

namespace moon_buggy
{

  namespace
  {
    auto static constexpr default_speed{100.f};
  }

  auto ScrollCamera::_register_methods() -> void
  {
    godot::register_method("_physics_process", &ScrollCamera::_physics_process);

    godot::register_property("speed", &ScrollCamera::speed, default_speed);
    godot::register_property("should_scroll", &ScrollCamera::should_scroll, false);
  }

  auto ScrollCamera::_init() -> void
  {
    speed = default_speed;
  }

  auto ScrollCamera::_physics_process(real_t delta) -> void
  {
    if (should_scroll)
    {
      set_position(get_position() + godot::Vector2{-1, 0} * speed * delta);
    }
  }

}  // namespace moon_buggy