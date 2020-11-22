#include "core/Level.hpp"

#include <Godot.hpp>

namespace moon_buggy
{

  auto Level::_register_methods() -> void
  {
    godot::register_property("surface_tiles", &Level::surface_tiles, {});
  }

  auto Level::_init() -> void
  {
  }

}  // namespace moon_buggy