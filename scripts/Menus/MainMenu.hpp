#ifndef MB2D_SCRIPTS_MENUS_MAIN_MENU_HPP
#define MB2D_SCRIPTS_MENUS_MAIN_MENU_HPP

#include "Helpers/DontWarn.hpp"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/error_macros.hpp>
#include <godot_cpp/core/object.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/control.hpp>

#include <algorithm>
#include <initializer_list>

namespace mb2d
{

  struct MainMenu : godot::Control
  {
    DONT_WARN(GDCLASS(MainMenu, godot::Control))

    auto static _bind_methods() -> void
    {
      godot::ClassDB::bind_method((godot::D_METHOD("handle_button_press", "button")), &MainMenu::handle_button_press);

      ADD_SIGNAL(godot::MethodInfo{"credits_pressed"});
      ADD_SIGNAL(godot::MethodInfo{"quit_pressed"});
      ADD_SIGNAL(godot::MethodInfo{"start_pressed"});
    }

    auto _notification(int notification) -> void
    {
      switch (notification)
      {
      case NOTIFICATION_READY:
        ready();
        set_process(true);
      }
    }

  private:
    auto connect_buttons(std::initializer_list<godot::Button *> buttons) -> void
    {
      std::ranges::for_each(buttons, [this](auto button) {
        button->connect("pressed", godot::Callable{this, "handle_button_press"}.bindv(godot::Array::make(button)));
      });
    }

    auto handle_button_press(godot::Button * button) -> void
    {
      if (button == credits_button)
      {
        emit_signal("credits_pressed");
      }
      else if (button == quit_button)
      {
        emit_signal("quit_pressed");
      }
      else if (button == start_button)
      {
        emit_signal("start_pressed");
      }
      else
      {
        godot::UtilityFunctions::push_warning("Unknown button '", button, "' pressed!");
      }
    }

    auto ready() -> void
    {
      credits_button = get_node<godot::Button>("%Credits");
      quit_button = get_node<godot::Button>("%Quit");
      start_button = get_node<godot::Button>("%Start");

      ERR_FAIL_NULL(credits_button);
      ERR_FAIL_NULL(quit_button);
      ERR_FAIL_NULL(start_button);

      connect_buttons({credits_button, quit_button, start_button});

      start_button->grab_focus();
    }

    godot::Button * credits_button;
    godot::Button * quit_button;
    godot::Button * start_button;
  };

}  // namespace mb2d

#endif