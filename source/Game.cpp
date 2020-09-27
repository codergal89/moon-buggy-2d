#include "Game.hpp"

#include <Godot.hpp>

namespace moon_buggy
{

  auto Game::_register_methods() -> void
  {
    godot::register_method("_ready", &Game::_ready);
    godot::register_method("kill_zone_entered", &Game::kill_zone_entered);
  }

  auto Game::_init() -> void
  {
  }

  auto Game::_ready() -> void
  {
    map = get_typed_node<Map>("Map");
    level_generator = get_typed_node<LevelGenerator>("LevelGenerator");

    auto level = level_generator->generate(0);
    map->level(level);
  }

  auto Game::kill_zone_entered(godot::Node * node) -> void
  {
    static_cast<void>(node);
  }

}  // namespace moon_buggy