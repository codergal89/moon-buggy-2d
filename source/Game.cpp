#include "Game.hpp"

#include "Buggy.hpp"
#include "ScrollCamera.hpp"

#include <Camera2D.hpp>
#include <Godot.hpp>
#include <PackedScene.hpp>
#include <Ref.hpp>
#include <TileMap.hpp>
#include <Viewport.hpp>

namespace moon_buggy
{

  auto Game::_register_methods() -> void
  {
    godot::register_method("_ready", &Game::_ready);
    godot::register_method("kill_zone_entered", &Game::kill_zone_entered);

    godot::register_property("buggy_scene", &Game::buggy_scene, godot::Ref<godot::PackedScene>{});
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

    auto buggy = static_cast<Buggy *>(buggy_scene->instance());
    auto ground = map->get_typed_node<godot::TileMap>("Ground");
    auto viewport_size = get_viewport()->get_size();
    buggy->set_position({.0f, viewport_size.y / 2 - ground->get_cell_size().y});

    auto scroll_camera = get_typed_node<ScrollCamera>("ScrollCamera");
    scroll_camera->make_current();
    scroll_camera->set("should_scroll", true);
    scroll_camera->add_child(buggy);
  }

  auto Game::kill_zone_entered(godot::Node * node) -> void
  {
    static_cast<void>(node);
  }

}  // namespace moon_buggy