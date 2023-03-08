#ifndef MB2D_SCRIPTS_MAIN_HPP
#define MB2D_SCRIPTS_MAIN_HPP

#include "Helpers/DontWarn.hpp"
#include "MeteorSpawner.hpp"

#include <godot_cpp/core/error_macros.hpp>

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/parallax_background.hpp>

namespace mb2d
{
  struct Main : godot::Node
  {
    DONT_WARN(GDCLASS(Main, godot::Node))

    auto static _bind_methods() -> void
    {
    }

    auto _ready() -> void override
    {
      background = get_node<godot::ParallaxBackground>("%Background");
      meteor_spawner = get_node<MeteorSpawner>("%MeteorSpawner");

      ERR_FAIL_NULL(background);
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

    auto _process(double delta) -> void override
    {
      if (!godot::Engine::get_singleton()->is_editor_hint())
      {
        process_in_game(delta);
      }
    }

    auto process_in_game(double delta) -> void
    {
      auto offset = background->get_scroll_offset();
      background->set_scroll_offset(offset + godot::Vector2(10.0, 0.0) * delta);
    }

  private:
    godot::ParallaxBackground * background{};
    MeteorSpawner * meteor_spawner{};
  };
}  // namespace mb2d

#endif