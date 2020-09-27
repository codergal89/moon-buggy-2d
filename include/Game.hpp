#ifndef MOON_BUGGY_2D_GAME_HPP
#define MOON_BUGGY_2D_GAME_HPP

#include "Buggy.hpp"
#include "LevelGenerator.hpp"
#include "MainMenu.hpp"
#include "Map.hpp"
#include "ScrollCamera.hpp"
#include "TypedNodeCastMixin.hpp"

#include <Area2D.hpp>
#include <Godot.hpp>
#include <Node.hpp>
#include <PackedScene.hpp>
#include <Ref.hpp>

namespace moon_buggy
{

  struct Game
      : godot::Node
      , TypedNodeCastMixin<Game>
  {
    auto static _register_methods() -> void;

    auto _init() -> void;
    auto _ready() -> void;

  private:
    GODOT_CLASS(Game, godot::Node)

    auto buggy_crashed(Buggy * buggy) -> void;
    auto start_game() -> void;

    Map * map;
    LevelGenerator * level_generator;
    ScrollCamera * scroll_camera;
    godot::Area2D * kill_zone;
    MainMenu * main_menu;
    godot::Ref<godot::PackedScene> buggy_scene;
    godot::Ref<godot::PackedScene> explosion_scene;
  };

}  // namespace moon_buggy

#endif