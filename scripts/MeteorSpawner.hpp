#ifndef MB2D_SCRIPTS_METEOR_SPAWNER_HPP
#define MB2D_SCRIPTS_METEOR_SPAWNER_HPP

#include "Actors/Meteor.hpp"
#include "Helpers/ArrayHelpers.hpp"
#include "Helpers/DontWarn.hpp"
#include "Helpers/PropertiesGetSet.hpp"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/error_macros.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/path_follow2d.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/timer.hpp>

#include <cstdint>

namespace mb2d
{

  struct MeteorSpawner
      : godot::Node2D
      , helpers::EasyProperties<MeteorSpawner>
  {
    auto constexpr static cDefaultMaximumSpawnAngle{135.0};
    auto constexpr static cDefaultMinimumSpawnAngle{45.0};
    auto constexpr static cDefaultMeteorLimit{5u};
    auto constexpr static cDefaultSpawnDelay{1};
    auto constexpr static cDefaultSpawnDelayJitter{0.5};

    ENABLE_EASY_PROPERTIES();
    DONT_WARN(GDCLASS(MeteorSpawner, godot::Node2D))

    auto static _bind_methods() -> void
    {
      godot::ClassDB::bind_method(godot::D_METHOD("on_spawn_timer_timeout"), &MeteorSpawner::on_spawn_timer_timeout);
      godot::ClassDB::bind_method(godot::D_METHOD("start"), &MeteorSpawner::start);
      godot::ClassDB::bind_method(godot::D_METHOD("stop"), &MeteorSpawner::stop);

      add_property("maximum_spawn_angle", &MeteorSpawner::maximum_spawn_angle, &MeteorSpawner::set_maximum_spawn_angle, 0, 180, 0.1);
      add_property("minimum_spawn_angle", &MeteorSpawner::minimum_spawn_angle, &MeteorSpawner::set_minimum_spawn_angle, 0, 180, 0.1);
      add_property("meteor_count", &MeteorSpawner::get_meteor_count);
      add_property("meteor_limit", &MeteorSpawner::meteor_limit, 0, 32, 1);
      add_property("meteor_scene", &MeteorSpawner::meteor_scene);
      add_property("spawn_delay", &MeteorSpawner::spawn_delay, 0.1, 10, 0.01);
      add_property("spawn_delay_jitter", &MeteorSpawner::spawn_delay_jitter, 0, 10, 0.01);
    }

    auto _ready() -> void override
    {
      rng.instantiate();

      meteors = get_node<godot::Node>("%Meteors");
      spawn_point = get_node<godot::PathFollow2D>("%SpawnPoint");
      spawn_timer = get_node<godot::Timer>("%SpawnTimer");

      ERR_FAIL_NULL(meteor_scene);
      ERR_FAIL_NULL(meteors);
      ERR_FAIL_NULL(rng);
      ERR_FAIL_NULL(spawn_point);
      ERR_FAIL_NULL(spawn_timer);

      if (!godot::Engine::get_singleton()->is_editor_hint())
      {
        ready_in_game();
      }
    }

    /**
     * @brief Ready actions to be perfomed when not running in the editor.
     */
    auto ready_in_game() -> void
    {
      rng->randomize();

      spawn_timer->connect("timeout", {this, "on_spawn_timer_timeout"});
      spawn_timer->set_one_shot(true);
    }

    auto start_spawn_timer() -> void
    {
      auto delay = rng->randf_range(spawn_delay - spawn_delay_jitter, spawn_delay + spawn_delay_jitter);
      spawn_timer->start(delay);
    }

    /**
     * @brief Spawn a new meteor at a random location.
     */
    auto spawn_meteor() -> void
    {
      if (static_cast<unsigned int>(this->get("meteor_count")) >= meteor_limit)
      {
        return;
      }

      auto progress = rng->randf_range(0.0, 1.0);
      auto meteor = cast_to<Meteor>(meteor_scene->instantiate());

      spawn_point->set_progress_ratio(progress);

      auto position = spawn_point->get_global_position();
      auto angle = rng->randf_range(minimum_spawn_angle, maximum_spawn_angle);

      meteor->set_position(position);
      meteor->set("starting_angle", angle);
      meteors->add_child(meteor);
    }

    /**
     * @brief Set the maximum spawn angle for meteors.
     */
    auto set_maximum_spawn_angle(double degrees) -> void
    {
      maximum_spawn_angle = degrees;
      if (maximum_spawn_angle < minimum_spawn_angle)
      {
        this->set("minimum_spawn_angle", degrees);
      }
    }

    /**
     * @brief Set the minimum spawn angle for meteors.
     */
    auto set_minimum_spawn_angle(double degrees) -> void
    {
      minimum_spawn_angle = degrees;
      if (minimum_spawn_angle > maximum_spawn_angle)
      {
        this->set("maximum_spawn_angle", degrees);
      }
    }

    auto get_meteor_count() const -> int64_t
    {
      if (!is_inside_tree())
      {
        return 0;
      }
      return CountIf(meteors->get_children(), [](auto node) { return !!godot::Object::cast_to<Meteor>(node); });
    }

    /**
     * @brief Start the spawning of new meteors.
     */
    auto start() -> void
    {
      start_spawn_timer();
    }

    /**
     * @brief Stop the spawning of new meteors.
     */
    auto stop() -> void
    {
      spawn_timer->stop();
    }

    /**
     * @brief Callback for the "timeout()" event of the spawn timer.
     */
    auto on_spawn_timer_timeout() -> void
    {
      spawn_meteor();
      start_spawn_timer();
    }

  private:
    double maximum_spawn_angle{cDefaultMaximumSpawnAngle};
    double minimum_spawn_angle{cDefaultMinimumSpawnAngle};
    unsigned int meteor_limit{cDefaultMeteorLimit};
    godot::Ref<godot::PackedScene> meteor_scene{};
    godot::Node * meteors{};
    godot::Ref<godot::RandomNumberGenerator> rng{};
    double spawn_delay{cDefaultSpawnDelay};
    double spawn_delay_jitter{cDefaultSpawnDelayJitter};
    godot::PathFollow2D * spawn_point{};
    godot::Timer * spawn_timer{};
  };

}  // namespace mb2d

#endif