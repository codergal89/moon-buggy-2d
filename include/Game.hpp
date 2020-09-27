#ifndef MOON_BUGGY_2D_GAME_HPP
#define MOON_BUGGY_2D_GAME_HPP

#include "LevelGenerator.hpp"
#include "Map.hpp"
#include "TypedNodeCastMixin.hpp"

#include <Godot.hpp>
#include <Node.hpp>

namespace moon_buggy
{

  struct Game
      : godot::Node
      , TypedNodeCastMixin<Game>
  {
    auto static _register_methods() -> void;

    auto _init() -> void;
    auto _ready() -> void;

  private:
    GODOT_CLASS(Game, godot::Node)

    auto kill_zone_entered(godot::Node * node) -> void;

    Map * map;
    LevelGenerator * level_generator;
  };

}  // namespace moon_buggy

#endif