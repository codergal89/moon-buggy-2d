#include "core/LevelDescriptor.hpp"

#include <Godot.hpp>

namespace moon_buggy
{
  auto LevelDescriptor::_register_methods() -> void
  {
    godot::register_property("segments", &LevelDescriptor::segments, 0);
    godot::register_property("minimum_hole_length", &LevelDescriptor::minimum_hole_length, 0);
    godot::register_property("maximum_hole_length", &LevelDescriptor::maximum_hole_length, 0);
    godot::register_property("minimum_platform_length", &LevelDescriptor::minimum_platform_length, 0);
    godot::register_property("maximum_platform_length", &LevelDescriptor::maximum_platform_length, 0);
  }

  auto LevelDescriptor::_new(int segments,
                             int minimum_hole_length,
                             int maximum_hole_length,
                             int minimum_platform_length,
                             int maximum_platform_length) -> LevelDescriptor *
  {
    auto instance = _new();
    instance->segments = segments;
    instance->minimum_hole_length = minimum_hole_length;
    instance->maximum_hole_length = maximum_hole_length;
    instance->minimum_platform_length = minimum_platform_length;
    instance->maximum_platform_length = maximum_platform_length;
    return instance;
  }

  auto LevelDescriptor::_init() -> void
  {
  }
}  // namespace moon_buggy