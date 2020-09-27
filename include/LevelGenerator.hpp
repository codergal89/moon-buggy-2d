#ifndef MOON_BUGGY_2D_LEVEL_GENERATOR_HPP
#define MOON_BUGGY_2D_LEVEL_GENERATOR_HPP

#include "Level.hpp"
#include "TypedNodeCastMixin.hpp"

#include <Godot.hpp>
#include <Node.hpp>

namespace moon_buggy
{

  struct LevelGenerator
      : godot::Node
      , TypedNodeCastMixin<LevelGenerator>
  {
    auto static constexpr default_start_area_length{20};

    auto static _register_methods() -> void;

    auto _init() -> void;

    auto generate(unsigned difficulty) -> Level;

  private:
    GODOT_CLASS(LevelGenerator, godot::Node)

    int start_area_length{default_start_area_length};
  };

}  // namespace moon_buggy

#endif