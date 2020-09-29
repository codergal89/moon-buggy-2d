#include "Game.hpp"

#include "Buggy.hpp"
#include "ScrollCamera.hpp"

#include <AnimatedSprite.hpp>
#include <Camera2D.hpp>
#include <Godot.hpp>
#include <KinematicBody2D.hpp>
#include <PackedScene.hpp>
#include <Particles2D.hpp>
#include <ProjectSettings.hpp>
#include <Ref.hpp>
#include <TileMap.hpp>
#include <Timer.hpp>

namespace moon_buggy
{

  auto Game::_register_methods() -> void
  {
    godot::register_method("_ready", &Game::_ready);
    godot::register_method("buggy_crashed", &Game::buggy_crashed);
    godot::register_method("goal_reached", &Game::goal_reached);
    godot::register_method("start_game", &Game::start_game);

    godot::register_property("buggy_scene", &Game::buggy_scene, godot::Ref<godot::PackedScene>{});
    godot::register_property("explosion_scene", &Game::explosion_scene, godot::Ref<godot::PackedScene>{});
    godot::register_property("fireworks_scene", &Game::fireworks_scene, godot::Ref<godot::PackedScene>{});
  }

  auto Game::_init() -> void
  {
    auto settings = godot::ProjectSettings::get_singleton();
    window_height = settings->get_setting("display/window/size/height");
    window_width = settings->get_setting("display/window/size/width");
  }

  auto Game::_ready() -> void
  {
    restart_timer = get_typed_node<godot::Timer>("RestartTimer");

    level_generator = get_typed_node<LevelGenerator>("LevelGenerator");
    map = get_typed_node<Map>("Map");

    main_menu = get_typed_node<MainMenu>("MainMenu");
    main_menu->show();

    scroll_camera = get_typed_node<ScrollCamera>("ScrollCamera");
    scroll_camera->set("should_scroll", true);
    kill_zone = scroll_camera->get_kill_zone();
  }

  auto Game::buggy_crashed(Buggy * buggy) -> void
  {
    scroll_camera->set_deferred("should_scroll", false);

    auto explosion = cast_to<godot::AnimatedSprite>(explosion_scene->instance());
    auto explosion_position = buggy->get_position();
    explosion_position.y -= 16.f;
    scroll_camera->add_child(explosion);
    explosion->set_position(explosion_position);
    explosion->connect("animation_finished", explosion, "queue_free");
    explosion->connect("animation_finished", main_menu, "show");
    explosion->call_deferred("play");

    buggy->queue_free();
  }

  auto Game::goal_reached() -> void
  {
    auto buggy = scroll_camera->get_typed_node<Buggy>("Buggy");
    auto fireworks = cast_to<godot::Particles2D>(fireworks_scene->instance());

    fireworks->set_deferred("emitting", true);
    fireworks->set_position(buggy->get_position());
    restart_timer->connect("timeout", main_menu, "show");
    restart_timer->start();

    scroll_camera->add_child(fireworks);

    buggy->queue_free();
  }

  auto Game::start_game() -> void
  {
    main_menu->hide();

    auto level = level_generator->generate(0);
    map->level(level, window_width, window_height);

    godot::Godot::print("end of the world is at: {0}", map->world_end());

    scroll_camera->set_position(godot::Vector2{0.f, 0.f});
    scroll_camera->set("limit_left", map->world_end());

    auto buggy = cast_to<Buggy>(buggy_scene->instance());
    auto ground = map->get_typed_node<godot::TileMap>("Ground");
    buggy->set_position({window_width / 2.f, window_height - ground->get_cell_size().y});
    buggy->connect("crashed", this, "buggy_crashed");
    scroll_camera->add_child(buggy);
    kill_zone->connect("body_entered", buggy, "kill_zone_entered");

    scroll_camera->set("should_scroll", true);
  }

}  // namespace moon_buggy