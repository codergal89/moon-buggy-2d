#ifndef MOON_BUGGY_2D_LEVEL_DESCRIPTOR_HPP
#define MOON_BUGGY_2D_LEVEL_DESCRIPTOR_HPP

#include <Godot.hpp>
#include <Object.hpp>

#include <queue>

namespace moon_buggy
{

  struct LevelDescriptor : godot::Object
  {
    auto static _register_methods() -> void;
    auto static _new(int segments, int minimum_hole_length, int maximum_hole_length, int minimum_platform_length, int maximum_platform_length)
        -> LevelDescriptor *;

    auto _init() -> void;

    int segments{};
    int minimum_hole_length{};
    int maximum_hole_length{};
    int minimum_platform_length{};
    int maximum_platform_length{};

    GODOT_CLASS(LevelDescriptor, godot::Object)  // NOLINT
  };

}  // namespace moon_buggy

#endif