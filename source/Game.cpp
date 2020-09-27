#include "Game.hpp"

#include <Camera2D.hpp>
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

    auto scroll_camera = get_typed_node<godot::Camera2D>("ScrollCamera");
    scroll_camera->make_current();
    scroll_camera->set("should_scroll", true);
  }

  auto Game::kill_zone_entered(godot::Node * node) -> void
  {
    static_cast<void>(node);
  }

}  // namespace moon_buggy