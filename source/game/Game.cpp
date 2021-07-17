#include "game/Game.hpp"

#include "game/Buggy.hpp"
#include "game/LevelGenerator.hpp"
#include "game/Map.hpp"
#include <Godot.hpp>
#include <Timer.hpp>

namespace moon_buggy
{

  auto Game::_register_methods() -> void
  {
    godot::register_method("_ready", &Game::_ready);
  }

  auto Game::_init() -> void
  {
  }

  auto Game::_ready() -> void
  {
    level_generator = get_typed_node<LevelGenerator>("LevelGenerator");
    map = get_typed_node<Map>("Map");
    restart_timer = get_typed_node<godot::Timer>("RestartTimer");
  }

}  // namespace moon_buggy