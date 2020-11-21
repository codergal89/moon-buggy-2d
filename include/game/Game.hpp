#ifndef MOON_BUGGY_2D_GAME_HPP
#define MOON_BUGGY_2D_GAME_HPP

#include "core/Level.hpp"
#include "game/Buggy.hpp"
#include "game/LevelGenerator.hpp"
#include "game/Map.hpp"
#include "game/ScrollCamera.hpp"
#include "gui/HUD.hpp"
#include "gui/LevelComplete.hpp"
#include "gui/MainMenu.hpp"
#include "support/ObjectPointer.hpp"
#include "support/TypedNodeCastMixin.hpp"

#include <Area2D.hpp>
#include <Control.hpp>
#include <Godot.hpp>
#include <Image.hpp>
#include <ImageTexture.hpp>
#include <Node.hpp>
#include <PackedScene.hpp>
#include <RandomNumberGenerator.hpp>
#include <Ref.hpp>
#include <String.hpp>
#include <Timer.hpp>

#include <cstddef>
#include <cstdint>

namespace moon_buggy
{

  struct Game
      : godot::Node
      , TypedNodeCastMixin<Game>
  {
    enum struct start_reason
    {
      start_next,
      retry,
    };

    auto static _register_methods() -> void;

    auto _init() -> void;
    auto _ready() -> void;

  private:
    GODOT_CLASS(Game, godot::Node)  // NOLINT

    auto check_properties() -> void;

    auto buggy_crashed(Buggy * buggy) -> void;
    auto goal_reached() -> void;
    auto start_game() -> void;
    auto start_level() -> void;

    std::uint64_t window_width{};
    std::uint64_t window_height{};
    int number_of_levels{};
    int current_level_number{};
    object_ptr<Level> current_level{};
    start_reason level_start_reason{};

    godot::Timer * restart_timer;
    LevelGenerator * level_generator;
    LevelComplete * level_complete_screen;
    godot::Control * active_start_screen;
    Map * map;
    MainMenu * main_menu;
    ScrollCamera * scroll_camera;
    HUD * hud;
    godot::Area2D * kill_zone;

    godot::Ref<godot::RandomNumberGenerator> moon_tiles_image_rng{};

    godot::Ref<godot::PackedScene> buggy_scene{};
    godot::Ref<godot::PackedScene> explosion_scene{};
    godot::Ref<godot::PackedScene> fireworks_scene{};

    godot::Ref<godot::ImageTexture> moon_tiles_texure{};
    godot::Array moon_tiles_images{};
  };

  auto to_string(Game::start_reason reason) -> godot::String;
  auto to_reason(godot::String string) -> Game::start_reason;

}  // namespace moon_buggy

#endif