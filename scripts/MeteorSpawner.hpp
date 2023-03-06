#ifndef MB2D_SCRIPTS_METEOR_SPAWNER_HPP
#define MB2D_SCRIPTS_METEOR_SPAWNER_HPP

#include "Actors/Meteor.hpp"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/error_macros.hpp>
#include <godot_cpp/core/memory.hpp>

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/path_follow2d.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/resource_loader.hpp>

namespace mb2d
{

  struct MeteorSpawner : godot::Node2D
  {
    GDCLASS(MeteorSpawner, godot::Node2D);

    auto static _bind_methods() -> void
    {
      godot::ClassDB::bind_method(godot::D_METHOD("get_meteor_scene"), &MeteorSpawner::get_meteor_scene);
      godot::ClassDB::bind_method(godot::D_METHOD("set_meteor_scene"), &MeteorSpawner::set_meteor_scene, "scene");

      bind_properties();
    }

    auto static bind_properties() -> void
    {
      auto meteor_scene = godot::PropertyInfo{godot::Variant::OBJECT, "meteor_scene", godot::PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"};

      ADD_PROPERTY(meteor_scene, "set_meteor_scene", "get_meteor_scene");
    }

    auto _ready() -> void override
    {
      spawn_point = get_node<godot::PathFollow2D>("%SpawnPoint");

      ERR_FAIL_NULL(meteor_scene);
      ERR_FAIL_NULL(spawn_point);
    }

    auto _physics_process(double delta) -> void override
    {
      if (!godot::Engine::get_singleton()->is_editor_hint())
      {
        physics_process_in_game(delta);
      }
    }

    auto physics_process_in_game(double delta) -> void
    {
      auto current_ratio = spawn_point->get_progress_ratio();
      auto new_ratio = current_ratio + delta * 1.0;
      spawn_point->set_progress_ratio(new_ratio);
    }

    auto _process(double) -> void override
    {
      if (!godot::Engine::get_singleton()->is_editor_hint())
      {
        process_in_game();
      }
    }

    auto process_in_game() -> void
    {
      auto meteor = meteor_scene->instantiate();
      spawn_point->add_child(meteor);
    }

    auto get_meteor_scene() const -> godot::Ref<godot::PackedScene>
    {
      return meteor_scene;
    }

    auto set_meteor_scene(godot::Ref<godot::PackedScene> scene) -> void
    {
      meteor_scene = scene;
    }

  private:
    godot::PathFollow2D * spawn_point{};
    godot::Ref<godot::PackedScene> meteor_scene{};
  };

}  // namespace mb2d

#endif