#include "Game.hpp"

#include "Buggy.hpp"
#include "ScrollCamera.hpp"

#include <Camera2D.hpp>
#include <Godot.hpp>
#include <KinematicBody2D.hpp>
#include <PackedScene.hpp>
#include <Ref.hpp>
#include <TileMap.hpp>
#include <Viewport.hpp>

namespace moon_buggy
{

  auto Game::_register_methods() -> void
  {
    godot::register_method("_ready", &Game::_ready);
    godot::register_method("buggy_crashed", &Game::buggy_crashed);

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

    scroll_camera = get_typed_node<ScrollCamera>("ScrollCamera");
    scroll_camera->make_current();
    scroll_camera->set("should_scroll", true);

    auto buggy = cast_to<Buggy>(buggy_scene->instance());
    auto ground = map->get_typed_node<godot::TileMap>("Ground");
    auto viewport_size = get_viewport()->get_size();
    buggy->set_position({.0f, viewport_size.y / 2 - ground->get_cell_size().y});
    buggy->connect("crashed", this, "buggy_crashed");
    scroll_camera->add_child(buggy);

    auto kill_zone = scroll_camera->get_kill_zone();
    kill_zone->connect("body_entered", buggy, "kill_zone_entered");
  }

  auto Game::buggy_crashed(Buggy * buggy) -> void
  {
    godot::Godot::print("buggy {0} crashed", buggy->get_instance_id());
    scroll_camera->set_deferred("should_scroll", false);
  }

}  // namespace moon_buggy