#ifndef MOON_BUGGY_2D_CORE_METEOR_SPAWNER_HPP
#define MOON_BUGGY_2D_CORE_METEOR_SPAWNER_HPP

#include "support/TypedNodeCastMixin.hpp"

#include <Godot.hpp>
#include <PackedScene.hpp>
#include <PathFollow2D.hpp>
#include <RandomNumberGenerator.hpp>
#include <Ref.hpp>
#include <Timer.hpp>

namespace moon_buggy
{

  struct MeteorSpawner
      : godot::PathFollow2D
      , TypedNodeCastMixin<MeteorSpawner>
  {
    auto static _register_methods() -> void;

    auto _init() -> void;

  private:
    GODOT_CLASS(MeteorSpawner, godot::PathFollow2D)

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
    godot::Ref<godot::RandomNumberGenerator> rng{};
    unsigned live_meteor_limit{};
  };

}  // namespace moon_buggy

#endif