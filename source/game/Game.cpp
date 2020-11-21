#include "game/Game.hpp"

#include "game/Buggy.hpp"
#include "game/LevelGenerator.hpp"
#include "game/Map.hpp"
#include "game/ScrollCamera.hpp"
#include "gui/HUD.hpp"
#include "gui/LevelComplete.hpp"
#include "gui/MainMenu.hpp"

#include <Godot.hpp>
#include <ProjectSettings.hpp>
#include <Timer.hpp>

namespace moon_buggy
{

  auto Game::_register_methods() -> void
  {
    godot::register_method("_ready", &Game::_ready);
    godot::register_method("buggy_crashed", &Game::buggy_crashed);
    godot::register_method("goal_reached", &Game::goal_reached);
    godot::register_method("start_game", &Game::start_game);

    godot::register_property("buggy_scene", &Game::buggy_scene, {});
    godot::register_property("explosion_scene", &Game::explosion_scene, {});
    godot::register_property("fireworks_scene", &Game::fireworks_scene, {});

    godot::register_property("moon_tiles_texture", &Game::moon_tiles_texure, {});
    godot::register_property("moon_tiles_images", &Game::moon_tiles_images, {});

    godot::register_property("space_texture", &Game::space_texture, {});
    godot::register_property("space_images", &Game::space_images, {});

    godot::register_property("stars_texture", &Game::stars_texture, {});
    godot::register_property("stars_images", &Game::stars_images, {});
  }

  auto Game::_init() -> void
  {
    auto settings = godot::ProjectSettings::get_singleton();
    window_height = settings->get_setting("display/window/size/height");
    window_width = settings->get_setting("display/window/size/width");

    level_start_reason = start_reason::start_next;

    theme_rng.instance();
    theme_rng->randomize();
  }

  auto Game::_ready() -> void
  {
    check_properties();

    randomize_theme();

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

}  // namespace moon_buggy