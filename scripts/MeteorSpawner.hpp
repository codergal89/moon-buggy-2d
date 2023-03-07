#ifndef MB2D_SCRIPTS_METEOR_SPAWNER_HPP
#define MB2D_SCRIPTS_METEOR_SPAWNER_HPP

#include "Actors/Meteor.hpp"
#include "Helpers/ArrayHelpers.hpp"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/error_macros.hpp>
#include <godot_cpp/core/memory.hpp>
#include <godot_cpp/core/object.hpp>
#include <godot_cpp/core/property_info.hpp>
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
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/timer.hpp>
#include <cstdint>

namespace mb2d
{

  struct MeteorSpawner : godot::Node2D
  {
    auto constexpr static cDefaultMeteorLimit{5u};

    GDCLASS(MeteorSpawner, godot::Node2D);

    auto static _bind_methods() -> void
    {
      godot::ClassDB::bind_method(godot::D_METHOD("get_meteor_limit"), &MeteorSpawner::get_meteor_limit);
      godot::ClassDB::bind_method(godot::D_METHOD("set_meteor_limit"), &MeteorSpawner::set_meteor_limit, "count");
      godot::ClassDB::bind_method(godot::D_METHOD("get_meteor_scene"), &MeteorSpawner::get_meteor_scene);
      godot::ClassDB::bind_method(godot::D_METHOD("set_meteor_scene"), &MeteorSpawner::set_meteor_scene, "scene");
      godot::ClassDB::bind_method(godot::D_METHOD("on_spawn_timer_timeout"), &MeteorSpawner::on_spawn_timer_timeout);
      godot::ClassDB::bind_method(godot::D_METHOD("start"), &MeteorSpawner::start);
      godot::ClassDB::bind_method(godot::D_METHOD("stop"), &MeteorSpawner::stop);

      bind_properties();
    }

    auto static bind_properties() -> void
    {
      auto meteor_limit = godot::PropertyInfo{godot::Variant::INT, "meteor_limit", godot::PROPERTY_HINT_RANGE, "0,32,1"};
      auto meteor_scene = godot::PropertyInfo{godot::Variant::OBJECT, "meteor_scene", godot::PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"};

      ADD_PROPERTY(meteor_limit, "set_meteor_limit", "get_meteor_limit");
      ADD_PROPERTY(meteor_scene, "set_meteor_scene", "get_meteor_scene");
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
      spawn_timer->set_wait_time(0.2);
    }

    /**
     * @brief Spawn a new meteor at a random location.
     */
    auto spawn_meteor() -> void
    {
      if (get_meteor_count() >= meteor_limit)
      {
        return;
      }

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
     * @brief Get the maximum number of concurrently spawned meteors.
     */
    auto get_meteor_limit() const -> unsigned int
    {
      return meteor_limit;
    }

    /**
     * @brief Set the maximum number of concurrently spawned meteors.
     */
    auto set_meteor_limit(unsigned int count) -> void
    {
      meteor_limit = count;
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

    auto get_meteor_count() const -> int64_t
    {
      CRASH_COND(meteors == nullptr);
      return CountIf(meteors->get_children(), [](auto node){
        return !!godot::Object::cast_to<Meteor>(node);
      });
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
    unsigned int meteor_limit{cDefaultMeteorLimit};
    godot::Ref<godot::PackedScene> meteor_scene{};
    godot::Node * meteors{};
    godot::Ref<godot::RandomNumberGenerator> rng{};
    godot::PathFollow2D * spawn_point{};
    godot::Timer * spawn_timer{};
  };

}  // namespace mb2d

#endif