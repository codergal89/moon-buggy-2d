#include "game/Meteors.hpp"

#include <Godot.hpp>
#include <Timer.hpp>

namespace moon_buggy
{

  auto Meteors::_register_methods() -> void
  {
    godot::register_method("_ready", &Meteors::_ready);
  }

  auto Meteors::_init() -> void
  {
  }

  auto Meteors::_ready() -> void
  {
    spawn_timer = get_typed_node<godot::Timer>("SpawnTimer");
    CRASH_COND(!spawn_timer);
  }

}  // namespace moon_buggy