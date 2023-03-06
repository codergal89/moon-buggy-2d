#ifndef MB2D_SCRIPTS_MAIN_HPP
#define MB2D_SCRIPTS_MAIN_HPP

#include "MeteorSpawner.hpp"

#include <godot_cpp/core/error_macros.hpp>

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/node.hpp>

namespace mb2d
{
  struct Main : godot::Node
  {
    GDCLASS(Main, godot::Node)

    auto static _bind_methods() -> void
    {
    }

    auto _ready() -> void override
    {
      meteor_spawner = get_node<MeteorSpawner>("%MeteorSpawner");

      ERR_FAIL_NULL(meteor_spawner);

      if (!godot::Engine::get_singleton()->is_editor_hint())
      {
        ready_in_game();
      }
    }

    auto ready_in_game() -> void
    {
      meteor_spawner->start();
    }

  private:
    MeteorSpawner * meteor_spawner{};
  };
}  // namespace mb2d

#endif