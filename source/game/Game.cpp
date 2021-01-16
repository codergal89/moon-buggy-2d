#include "game/Game.hpp"

#include "core/Constants.hpp"
#include "game/Buggy.hpp"
#include "game/GameMachine.hpp"
#include "game/LevelGenerator.hpp"
#include "game/Map.hpp"
#include "game/Meteor.hpp"
#include "game/ScrollCamera.hpp"
#include "gui/GUI.hpp"
#include "support/ArrayIterator.hpp"

#include <AnimatedSprite.hpp>
#include <CanvasLayer.hpp>
#include <Control.hpp>
#include <Defs.hpp>
#include <Godot.hpp>
#include <Particles2D.hpp>
#include <ProjectSettings.hpp>
#include <SceneTree.hpp>
#include <Timer.hpp>
#include <Vector2.hpp>

namespace moon_buggy
{

  namespace
  {
    auto constexpr absolute_meteor_angle_limit = pi / 2 - (15.0 * pi) / 180.0;
    auto constexpr minimum_meteor_angle = static_cast<real_t>(-absolute_meteor_angle_limit);
    auto constexpr maximum_meteor_angle = static_cast<real_t>(absolute_meteor_angle_limit);
    auto constexpr minimum_meteor_speed = static_cast<real_t>(300.0);
    auto constexpr maximum_meteor_speed = static_cast<real_t>(500.0);
  }  // namespace

  auto Game::_register_methods() -> void
  {
    godot::register_method("_ready", &Game::_ready);
    godot::register_method("has_next_level", &Game::has_next_level);
    godot::register_method("on_buggy_crashed", &Game::on_buggy_crashed);
    godot::register_method("on_game_start_requested", &Game::on_game_start_requested);
    godot::register_method("on_next_level_requested", &Game::on_next_level_requested);
    godot::register_method("on_goal_reached", &Game::on_goal_reached);
    godot::register_method("on_meteor_spawn_timer_expired", &Game::on_meteor_spawn_timer_expired);
    godot::register_method("on_success_animation_played", &Game::on_success_animation_played);
    godot::register_method("play_success_animation", &Game::play_success_animation);
    godot::register_method("show_buggy_crashed_screen", &Game::show_buggy_crashed_screen);
    godot::register_method("show_level_complete_screen", &Game::show_level_complete_screen);
    godot::register_method("show_main_menu", &Game::show_main_menu);
    godot::register_method("start_next_level", &Game::start_next_level);

    godot::register_property("buggy_scene", &Game::buggy_scene, {});
    godot::register_property("explosion_scene", &Game::explosion_scene, {});
    godot::register_property("fireworks_scene", &Game::fireworks_scene, {});
    godot::register_property("meteor_scene", &Game::meteor_scene, {});
    godot::register_property("moon_tiles_images", &Game::moon_tiles_images, {});
    godot::register_property("moon_tiles_texture", &Game::moon_tiles_texure, {});
    godot::register_property("space_images", &Game::space_images, {});
    godot::register_property("space_texture", &Game::space_texture, {});
    godot::register_property("stars_images", &Game::stars_images, {});
    godot::register_property("stars_texture", &Game::stars_texture, {});
  }

  auto Game::_init() -> void
  {
    state_machine = std::make_shared<boost::sml::sm<GameMachine>>(*this);

    auto settings = godot::ProjectSettings::get_singleton();
    window_height = settings->get_setting("display/window/size/height");
    window_width = settings->get_setting("display/window/size/width");

    theme_rng.instance();
    theme_rng->randomize();

    meteor_rng.instance();
    meteor_rng->randomize();
  }

  auto Game::_ready() -> void
  {
    check_properties();

    gui = get_typed_node<GUI>("GUI");

    level_generator = get_typed_node<LevelGenerator>("LevelGenerator");

    map = get_typed_node<Map>("Map");
    restart_timer = get_typed_node<godot::Timer>("RestartTimer");
    scroll_camera = get_typed_node<ScrollCamera>("ScrollCamera");
    kill_zone = scroll_camera->get_kill_zone();

    randomize_theme();

    state_machine->process_event(readied_up{});

    start_meteors();
  }

  auto Game::show_buggy_crashed_screen() -> void
  {
  }

  auto Game::show_hud() -> void
  {
    gui->call("show_hud");
  }

  auto Game::show_level_complete_screen() -> void
  {
    gui->call("show_level_complete_screen");
  }

  auto Game::show_main_menu() -> void
  {
    gui->call("show_main_menu");
    scroll_camera->set("should_scroll", true);
  }

  auto Game::has_next_level() const -> bool
  {
    return level_generator->has_remaining_levels();
  }

  auto Game::start_next_level() -> void
  {
    ++current_level_number;
    current_level.reset(level_generator->generate_next());
    randomize_theme();
    start_level();
  }

  auto Game::play_success_animation() -> void
  {
    auto buggy = scroll_camera->get_typed_node<Buggy>("Buggy");
    auto fireworks = cast_to<godot::Particles2D>(fireworks_scene->instance());

    fireworks->set_deferred("emitting", true);
    fireworks->set_position(buggy->get_position());
    restart_timer->set_wait_time(fireworks->get_lifetime());
    restart_timer->start();
    restart_timer->connect("timeout", this, "on_success_animation_played");

    scroll_camera->add_child(fireworks);

    buggy->queue_free();
  }

  auto Game::on_buggy_crashed(Buggy * buggy) -> void
  {
    state_machine->process_event(crash_occured{buggy});
  }

  auto Game::on_goal_reached() -> void
  {
    state_machine->process_event(reached_goal{});
  }

  auto Game::on_game_start_requested() -> void
  {
    stop_meteors();
    state_machine->process_event(game_start_requested{});
  }

  auto Game::on_meteor_spawn_timer_expired() -> void
  {
    spawn_meteor();
    auto timer = get_typed_node<godot::Timer>("MeteorSpawnTimer");
    timer->start(meteor_rng->randf_range(0.5f, 1.5f));
  }

  auto Game::on_next_level_requested() -> void
  {
    state_machine->process_event(next_level_requested{});
  }

  auto Game::on_success_animation_played() -> void
  {
    scroll_camera->get_node("Fireworks")->queue_free();
    restart_timer->disconnect("timeout", this, "on_success_animation_played");
    state_machine->process_event(animation_finished{});
  }

  auto Game::check_properties() -> void
  {
    CRASH_COND(!buggy_scene.is_valid());
    CRASH_COND(!explosion_scene.is_valid());
    CRASH_COND(!fireworks_scene.is_valid());
    CRASH_COND(!meteor_scene.is_valid());

    CRASH_COND(moon_tiles_images.empty());
    CRASH_COND(!moon_tiles_texure.is_valid());
  }

  auto Game::randomize_theme() -> void
  {
    auto theme_id = static_cast<int>(theme_rng->randi_range(0, moon_tiles_images.size() - 1));
    moon_tiles_texure->set_data(moon_tiles_images[theme_id]);
    space_texture->set_data(space_images[theme_id]);
    stars_texture->set_data(stars_images[theme_id]);
  }

  auto Game::start_level() -> void
  {
    show_hud();
    gui->call("set_level_number", current_level_number);

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

  auto Game::start_meteors() -> void
  {
    auto timer = godot::Timer::_new();
    add_child(timer);
    timer->set_name("MeteorSpawnTimer");
    timer->set_one_shot(true);
    timer->connect("timeout", this, "on_meteor_spawn_timer_expired");
    timer->start(0);
  }

  auto Game::stop_meteors() -> void
  {
    auto timer = get_typed_node<godot::Timer>("MeteorSpawnTimer");
    timer->stop();
    remove_child(timer);

    get_tree()->call_group("meteors", "queue_free");
  }

  auto Game::spawn_meteor() -> void
  {
    auto live_meteors = get_tree()->get_nodes_in_group("meteors");
    if (live_meteors.size() >= 3)
    {
      return;
    }

    auto meteor = cast_to<Meteor>(meteor_scene->instance());
    meteor->set_position(godot::Vector2{meteor_rng->randf_range(-128.f, window_width + 128.f), 0.f});

    meteor->set("entry_angle", meteor_rng->randf_range(minimum_meteor_angle, maximum_meteor_angle));
    meteor->set("entry_speed", meteor_rng->randf_range(minimum_meteor_speed, maximum_meteor_speed));
    scroll_camera->add_child(meteor);
  }

}  // namespace moon_buggy