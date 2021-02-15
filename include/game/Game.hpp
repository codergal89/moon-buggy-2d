#ifndef MOON_BUGGY_2D_GAME_GAME_HPP
#define MOON_BUGGY_2D_GAME_GAME_HPP

// #include "core/Fwd.hpp"
#include "game/Fwd.hpp"
// #include "gui/Fwd.hpp"
// #include "support/ObjectPointer.hpp"
#include "support/TypedNodeCastMixin.hpp"

// #include <Area2D.hpp>
// #include <Array.hpp>
// #include <CanvasLayer.hpp>
#include <Godot.hpp>
// #include <ImageTexture.hpp>
#include <Node2D.hpp>
// #include <PackedScene.hpp>
// #include <RandomNumberGenerator.hpp>
// #include <Ref.hpp>
#include <Timer.hpp>
// #include <boost/sml.hpp>

#include <cstdint>
#include <memory>

namespace moon_buggy
{

  struct GameMachine;

  struct Game
      : godot::Node2D
      , TypedNodeCastMixin<Game>
  {
    auto static _register_methods() -> void;

    auto _init() -> void;

  private:
    GODOT_CLASS(Game, godot::Node2D)

    // Exported Methods
    auto _ready() -> void;

    // Internal Variables
    Buggy * buggy{};
    LevelGenerator * level_generator{};
    Map * map{};
    Meteors * meteors{};
    godot::Timer * restart_timer{};

    // auto show_buggy_crashed_screen() -> void;
    // auto show_hud() -> void;
    // auto show_level_complete_screen() -> void;
    // auto show_main_menu() -> void;

    // auto has_next_level() const -> bool;
    // auto start_next_level() -> void;

    // auto play_success_animation() -> void;

    // /// Signals
    // auto on_buggy_crashed(Buggy * buggy) -> void;
    // auto on_goal_reached() -> void;
    // auto on_game_start_requested() -> void;
    // auto on_meteor_spawn_timer_expired() -> void;
    // auto on_next_level_requested() -> void;
    // auto on_success_animation_played() -> void;

    // /// Internal Functions
    // auto check_properties() -> void;
    // auto randomize_theme() -> void;
    // auto start_level() -> void;
    // auto start_meteors() -> void;
    // auto stop_meteors() -> void;

    // /// Exported Properties
    // godot::Ref<godot::PackedScene> buggy_scene{};
    // godot::Ref<godot::PackedScene> explosion_scene{};
    // godot::Ref<godot::PackedScene> fireworks_scene{};

    // godot::Array moon_tiles_images{};
    // godot::Ref<godot::ImageTexture> moon_tiles_texure{};
    // godot::Array space_images{};
    // godot::Ref<godot::ImageTexture> space_texture{};
    // godot::Array stars_images{};
    // godot::Ref<godot::ImageTexture> stars_texture{};

    // /// Internal Variables
    // std::uint64_t window_width{};
    // std::uint64_t window_height{};
    // int current_level_number{};
    // object_ptr<Level> current_level{};

    // godot::Timer * restart_timer;
    // LevelGenerator * level_generator;
    // Map * map;
    // ScrollCamera * scroll_camera;
    // godot::Area2D * kill_zone;
    // Meteors * meteors;

    // GUI * gui;

    // godot::Ref<godot::RandomNumberGenerator> theme_rng{};

    // std::shared_ptr<boost::sml::sm<GameMachine>> state_machine{};
  };

}  // namespace moon_buggy

#endif