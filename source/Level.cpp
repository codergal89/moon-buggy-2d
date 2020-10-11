#include "Level.hpp"

#include <Godot.hpp>

namespace moon_buggy
{

  auto Level::_register_methods() -> void
  {
    godot::register_property("tiles", &Level::tiles, {});
  }

  auto Level::_init() -> void
  {
  }

}  // namespace moon_buggy