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
      ground,
      hole,
      left_shoulder,
      right_shoulder,
    };

    auto static _register_methods() -> void;

    auto _init() -> void;

    godot::Array tiles{};

    GODOT_CLASS(Level, godot::Object)  // NOLINT
  };

}  // namespace moon_buggy

#endif