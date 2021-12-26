#ifndef MOON_BUGGY_2D_GAME_GAME_HPP
#define MOON_BUGGY_2D_GAME_GAME_HPP

#include "game/Fwd.hpp"
#include "support/SilentGodotClass.hpp"
#include "support/TypedNodeCastMixin.hpp"

#include <Godot.hpp>
#include <Node2D.hpp>
#include <Timer.hpp>

#include <cstdint>
#include <memory>

namespace moon_buggy
{

  struct Game
      : godot::Node2D
      , TypedNodeCastMixin<Game>
  {
    auto static _register_methods() -> void;

    auto _init() -> void;

  private:
    SILENT_GODOT_CLASS(Game, godot::Node2D)

    /// Exported Methods
    auto _ready() -> void;
    
    auto start() -> void;

    /// Internal Variables
    LevelGenerator * level_generator{};
    Map * map{};
    godot::Timer * restart_timer{};
  };

}  // namespace moon_buggy

#endif