#include "core/Space.hpp"

#include <Defs.hpp>
#include <Godot.hpp>
#include <Vector2.hpp>

namespace moon_buggy
{

  namespace
  {
    auto constexpr default_autoscrolling_speed = real_t{100.f};
    auto constexpr default_enable_autoscrolling = false;
  }  // namespace

  auto Space::_register_methods() -> void
  {
    godot::register_method("_process", &Space::_process);

    godot::register_property("autoscrolling_speed", &Space::autoscrolling_speed, default_autoscrolling_speed);
    godot::register_property("enable_autoscrolling", &Space::enable_autoscrolling, default_enable_autoscrolling);
  }

  auto Space::_init() -> void
  {
    autoscrolling_speed = default_autoscrolling_speed;
    enable_autoscrolling = default_enable_autoscrolling;
  }

  auto Space::_process(real_t delta) -> void
  {
      if(enable_autoscrolling) {
          scroll(delta);
      }
  }

  auto Space::scroll(real_t delta) -> void
  {
      auto current_offset = get_scroll_offset();
      auto offset_delta = godot::Vector2::RIGHT * autoscrolling_speed * delta;
      auto target_offset = current_offset + offset_delta;

      set_scroll_offset(target_offset);
  }

}  // namespace moon_buggy