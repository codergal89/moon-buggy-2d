#ifndef MOON_BUGGY_2D_GAME_METEORS_HPP
#define MOON_BUGGY_2D_GAME_METEORS_HPP

#include "support/TypedNodeCastMixin.hpp"

#include <Godot.hpp>
#include <Node2D.hpp>
#include <PackedScene.hpp>
#include <PathFollow2D.hpp>
#include <RandomNumberGenerator.hpp>
#include <Ref.hpp>
#include <Timer.hpp>

namespace moon_buggy
{

  struct Meteors
      : godot::Node2D
      , TypedNodeCastMixin<Meteors>
  {
    auto static _register_methods() -> void;

    auto _init() -> void;

  private:
    GODOT_CLASS(Meteors, godot::Node2D)  // NOLINT

    /// Exported Methods
    auto _ready() -> void;

    auto on_spawn_timer_expired() -> void;

    auto get_live_meteor_limit() const -> unsigned;
    auto set_live_meteor_limit(unsigned limit) -> void;

    auto start() -> void;
    auto stop(bool clear = true) -> void;

    /// Internal Functions
    auto spawn_meteor() -> void;

    /// Exported Properties
    godot::Ref<godot::PackedScene> meteor_scene{};

    /// Internal Variables
    godot::Timer * spawn_timer{};
    godot::PathFollow2D * spawn_location{};
    godot::Ref<godot::RandomNumberGenerator> rng{};
    unsigned live_meteor_limit{};
  };

}  // namespace moon_buggy

#endif