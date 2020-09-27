#ifndef MOON_BUGGY_2D_GAME_BOUNDARY_HPP
#define MOON_BUGGY_2D_GAME_BOUNDARY_HPP

#include "TypedNodeCastMixin.hpp"

#include <CollisionShape2D.hpp>
#include <Godot.hpp>
#include <StaticBody2D.hpp>

namespace moon_buggy
{

  struct GameBoundary
      : godot::StaticBody2D
      , TypedNodeCastMixin<GameBoundary>
  {
    auto static _register_methods() -> void;

    auto _init() -> void;
    auto _ready() -> void;

  private:
    GODOT_CLASS(GameBoundary, godot::StaticBody2D);

    godot::CollisionShape2D * left_edge;
    godot::CollisionShape2D * right_edge;
  };

}  // namespace moon_buggy

#endif