#include "game/Game.hpp"

#include "game/Buggy.hpp"
#include "game/LevelGenerator.hpp"
#include "game/Map.hpp"
#include "game/ScrollCamera.hpp"
#include "gui/HUD.hpp"
#include "gui/LevelComplete.hpp"
#include "gui/MainMenu.hpp"

#include <AnimatedSprite.hpp>
#include <Defs.hpp>
#include <Particles2D.hpp>
#include <Vector2.hpp>

namespace moon_buggy
{

  auto Game::check_properties() -> void
  {
    CRASH_COND(!buggy_scene.is_valid());
    CRASH_COND(!explosion_scene.is_valid());
    CRASH_COND(!fireworks_scene.is_valid());

    CRASH_COND(moon_tiles_images.empty());
    CRASH_COND(!moon_tiles_texure.is_valid());
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
        auto theme_id = theme_rng->randi_range(0, moon_tiles_images.size() - 1);
        moon_tiles_texure->set_data(moon_tiles_images[theme_id]);
        space_texture->set_data(space_images[theme_id]);
        stars_texture->set_data(stars_images[theme_id]);
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
    auto surface_level = map->get_surface_level();

    scroll_camera->set_position(godot::Vector2{0.f, 0.f});
    scroll_camera->set("limit_left", map->get_world_end());
    auto scroll_speed = scroll_camera->get("speed");

    auto buggy = cast_to<Buggy>(buggy_scene->instance());
    buggy->set_position({window_width / 2.f, static_cast<real_t>(surface_level)});
    buggy->set("base_speed", scroll_speed);
    buggy->connect("crashed", this, "buggy_crashed");
    scroll_camera->add_child(buggy);
    kill_zone->connect("body_entered", buggy, "kill_zone_entered");

    scroll_camera->set("should_scroll", true);
  }

}  // namespace moon_buggy