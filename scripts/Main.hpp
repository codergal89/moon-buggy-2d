#ifndef MB2D_SCRIPTS_MAIN_HPP
#define MB2D_SCRIPTS_MAIN_HPP

#include "Helpers/DontWarn.hpp"
#include "Menus/MainMenu.hpp"
#include "MeteorSpawner.hpp"
#include "UI/Background.hpp"

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
      }
    }

  private:
    auto ready() -> void
    {
      background = get_node<Background>("%Background");
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
      background->start(10);
      meteor_spawner->start();
    }

    auto on_quit_pressed() -> void
    {
      get_tree()->quit();
    }

    auto on_start_pressed() -> void
    {
      main_menu->hide();
    }

    Background * background{};
    MainMenu * main_menu{};
    MeteorSpawner * meteor_spawner{};
  };
}  // namespace mb2d

#endif