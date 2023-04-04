#ifndef MB2D_SCRIPTS_MAIN_HPP
#define MB2D_SCRIPTS_MAIN_HPP

#include "Helpers/DontWarn.hpp"
#include "Menus/MainMenu.hpp"
#include "MeteorSpawner.hpp"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/error_macros.hpp>

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/parallax_background.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

namespace mb2d
{
  struct Main : godot::Node
  {
    DONT_WARN(GDCLASS(Main, godot::Node))

    auto static _bind_methods() -> void
    {
      godot::ClassDB::bind_method(godot::D_METHOD("on_quit_pressed"), &Main::on_quit_pressed);
      godot::ClassDB::bind_method(godot::D_METHOD("on_start_pressed"), &Main::on_start_pressed);
    }

    auto _notification(int notification) -> void
    {
      switch (notification)
      {
      case NOTIFICATION_READY:
        ready();
        set_process(true);
        break;
      case NOTIFICATION_PROCESS:
        process(get_process_delta_time());
        break;
      }
    }

  private:
    auto ready() -> void
    {
      background = get_node<godot::ParallaxBackground>("%Background");
      main_menu = get_node<MainMenu>("%MainMenu");
      meteor_spawner = get_node<MeteorSpawner>("%MeteorSpawner");

      ERR_FAIL_NULL(background);
      ERR_FAIL_NULL(main_menu);
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

    auto process(double delta) -> void
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

    auto on_quit_pressed() -> void
    {
      get_tree()->quit();
    }

    auto on_start_pressed() -> void
    {
      main_menu->hide();
    }

    godot::ParallaxBackground * background{};
    MainMenu * main_menu{};
    MeteorSpawner * meteor_spawner{};
  };
}  // namespace mb2d

#endif