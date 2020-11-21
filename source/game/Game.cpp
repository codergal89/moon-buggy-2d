#include "game/Game.hpp"

#include "core/Level.hpp"
#include "game/Buggy.hpp"
#include "game/ScrollCamera.hpp"

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

#include <algorithm>
#include <array>
#include <iterator>
#include <utility>

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

    godot::register_property("moon_tiles_texture", &Game::moon_tiles_texure, {});
    godot::register_property("moon_tiles_images", &Game::moon_tiles_images, {});
  }

  auto Game::_init() -> void
  {
    auto settings = godot::ProjectSettings::get_singleton();
    window_height = settings->get_setting("display/window/size/height");
    window_width = settings->get_setting("display/window/size/width");

    level_start_reason = start_reason::start_next;
  }

  auto Game::_ready() -> void
  {
    level_generator = get_typed_node<LevelGenerator>("LevelGenerator");
    number_of_levels = level_generator->load("res://config/levels.json");
    map = get_typed_node<Map>("Map");

    main_menu = get_typed_node<MainMenu>("GUI/MainMenu");
    main_menu->show();

    level_complete_screen = get_typed_node<LevelComplete>("GUI/LevelComplete");
    level_complete_screen->hide();

    active_start_screen = main_menu;

    hud = get_typed_node<HUD>("GUI/HUD");
    hud->hide();

    restart_timer = get_typed_node<godot::Timer>("RestartTimer");
    restart_timer->connect("timeout", level_complete_screen, "show");
    restart_timer->connect("timeout", hud, "hide");

    scroll_camera = get_typed_node<ScrollCamera>("ScrollCamera");
    scroll_camera->set("should_scroll", true);
    kill_zone = scroll_camera->get_kill_zone();
  }

  auto Game::buggy_crashed(Buggy * buggy) -> void
  {
    scroll_camera->set_deferred("should_scroll", false);
    level_start_reason = start_reason::retry;
    active_start_screen = main_menu;

    auto explosion = cast_to<godot::AnimatedSprite>(explosion_scene->instance());
    auto explosion_position = buggy->get_position();
    explosion_position.y -= 16.f;
    scroll_camera->add_child(explosion);
    explosion->set_position(explosion_position);
    explosion->connect("animation_finished", explosion, "queue_free");
    explosion->connect("animation_finished", active_start_screen, "show");
    explosion->connect("animation_finished", hud, "hide");
    explosion->call_deferred("play");

    buggy->queue_free();
  }

  auto Game::goal_reached() -> void
  {
    auto buggy = scroll_camera->get_typed_node<Buggy>("Buggy");
    auto fireworks = cast_to<godot::Particles2D>(fireworks_scene->instance());

    level_start_reason = start_reason::start_next;
    level_complete_screen->set_level_number(current_level_number);
    active_start_screen = level_complete_screen;

    fireworks->set_deferred("emitting", true);
    fireworks->set_position(buggy->get_position());
    restart_timer->start();

    scroll_camera->add_child(fireworks);

    buggy->queue_free();
  }

  auto Game::start_game() -> void
  {
    switch (level_start_reason)
    {
    case start_reason::start_next:
      if (level_generator->has_remaining_levels())
      {
        current_level_number++;
        current_level.reset(level_generator->generate_next());
      }
      break;
    default:
      break;
    }

    start_level();
  }

  auto Game::start_level() -> void
  {
    active_start_screen->hide();

    hud->show();
    hud->set_level_number(current_level_number);

    map->set_level(current_level.get(), window_width, window_height);

    scroll_camera->set_position(godot::Vector2{0.f, 0.f});
    scroll_camera->set("limit_left", map->get_world_end());
    auto scroll_speed = scroll_camera->get("speed");

    auto buggy = cast_to<Buggy>(buggy_scene->instance());
    auto ground = map->get_typed_node<godot::TileMap>("Ground");
    buggy->set_position({window_width / 2.f, window_height - 2 * ground->get_cell_size().y});
    buggy->set("base_speed", scroll_speed);
    buggy->connect("crashed", this, "buggy_crashed");
    scroll_camera->add_child(buggy);
    kill_zone->connect("body_entered", buggy, "kill_zone_entered");

    scroll_camera->set("should_scroll", true);
  }

  auto constexpr start_reason_strings = std::array{
      std::pair{Game::start_reason::start_next, "start next"},
      std::pair{Game::start_reason::retry, "retry"},
  };

  auto to_string(Game::start_reason reason) -> godot::String
  {
    auto found = find_if(cbegin(start_reason_strings), cend(start_reason_strings), [=](auto mapping) { return mapping.first == reason; });
    if (found != cend(start_reason_strings))
    {
      return found->second;
    }
    return "INVALID_ENUM_CONSTANT";
  }

  auto to_reason(godot::String string) -> Game::start_reason
  {
    auto found = find_if(cbegin(start_reason_strings), cend(start_reason_strings), [&](auto mapping) {
      return godot::String{mapping.second} == string;
    });
    if (found != cend(start_reason_strings))
    {
      return found->first;
    }
    return static_cast<Game::start_reason>(-1);
  }

}  // namespace moon_buggy