#ifndef MB2D_SCRIPTS_METEOR_SPAWNER_HPP
#define MB2D_SCRIPTS_METEOR_SPAWNER_HPP

#include "Actors/Meteor.hpp"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/error_macros.hpp>
#include <godot_cpp/core/memory.hpp>

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/path_follow2d.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/timer.hpp>

namespace mb2d
{

  struct MeteorSpawner : godot::Node2D
  {
    GDCLASS(MeteorSpawner, godot::Node2D);

    auto static _bind_methods() -> void
    {
      godot::ClassDB::bind_method(godot::D_METHOD("get_meteor_scene"), &MeteorSpawner::get_meteor_scene);
      godot::ClassDB::bind_method(godot::D_METHOD("set_meteor_scene"), &MeteorSpawner::set_meteor_scene, "scene");
      godot::ClassDB::bind_method(godot::D_METHOD("on_spawn_timer_timeout"), &MeteorSpawner::on_spawn_timer_timeout);
      godot::ClassDB::bind_method(godot::D_METHOD("start"), &MeteorSpawner::start);
      godot::ClassDB::bind_method(godot::D_METHOD("stop"), &MeteorSpawner::stop);

      bind_properties();
    }

    auto static bind_properties() -> void
    {
      auto meteor_scene = godot::PropertyInfo{godot::Variant::OBJECT, "meteor_scene", godot::PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"};

      ADD_PROPERTY(meteor_scene, "set_meteor_scene", "get_meteor_scene");
    }

    auto _ready() -> void override
    {
      rng.instantiate();

      meteors = get_node<godot::Node>("%Meteors");
      spawn_point = get_node<godot::PathFollow2D>("%SpawnPoint");
      spawn_timer = get_node<godot::Timer>("%SpawnTimer");

      ERR_FAIL_NULL(meteor_scene);
      ERR_FAIL_NULL(rng);
      ERR_FAIL_NULL(meteors);
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
      spawn_timer->set_wait_time(0.2);
    }

    /**
     * @brief Spawn a new meteor at a random location.
     */
    auto spawn_meteor() -> void
    {
      auto progress = rng->randf_range(0.0, 1.0);
      auto meteor = cast_to<Meteor>(meteor_scene->instantiate());

      spawn_point->set_progress_ratio(progress);

      auto position = spawn_point->get_global_position();
      auto angle = rng->randf_range(45.0, 135.0);

      meteor->set_position(position);
      meteor->set_starting_angle(angle);
      meteors->add_child(meteor);
    }

    /**
     * @brief Get the scene used for new meteors.
     */
    auto get_meteor_scene() const -> godot::Ref<godot::PackedScene>
    {
      return meteor_scene;
    }

    /**
     * @brief Set the scene used for new meteors.
     */
    auto set_meteor_scene(godot::Ref<godot::PackedScene> scene) -> void
    {
      meteor_scene = scene;
    }

    /**
     * @brief Start the spawning of new meteors.
     */
    auto start() -> void
    {
      spawn_timer->start();
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
      spawn_timer->set_wait_time(0.2);
    }

  private:
    godot::Ref<godot::PackedScene> meteor_scene{};
    godot::Ref<godot::RandomNumberGenerator> rng{};
    godot::Node * meteors{};
    godot::PathFollow2D * spawn_point{};
    godot::Timer * spawn_timer{};
  };

}  // namespace mb2d

#endif