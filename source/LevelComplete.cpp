#include "LevelComplete.hpp"

#include <Godot.hpp>

namespace moon_buggy
{

  auto LevelComplete::_register_methods() -> void
  {
    godot::register_method("_ready", &LevelComplete::_ready);
  }

  auto LevelComplete::_init() -> void
  {
  }

  auto LevelComplete::_ready() -> void
  {
  }

}  // namespace moon_buggy