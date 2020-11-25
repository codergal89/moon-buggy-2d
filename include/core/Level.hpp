#ifndef MOON_BUGGY_2D_LEVEL_HPP
#define MOON_BUGGY_2D_LEVEL_HPP

#include <Array.hpp>
#include <Godot.hpp>
#include <Object.hpp>

namespace moon_buggy
{

  struct Level : godot::Object
  {
    enum struct Tile
    {
      ground_layer1_border,
      ground_layer1,
      ground_layer2_border,
      ground_layer2,
      ground_layer3_border,
      ground_layer3,
      ground_surface,
      hole,
      left_shoulder,
      long_stone1,
      right_shoulder,
      small_stone1,
    };

    auto static _register_methods() -> void;

    auto _init() -> void;

    godot::Array surface_tiles{};

    GODOT_CLASS(Level, godot::Object)  // NOLINT
  };

}  // namespace moon_buggy

#endif