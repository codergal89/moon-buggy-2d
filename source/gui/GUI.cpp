#include "gui/GUI.hpp"

#include "core/ScrollCamera.hpp"
#include "gui/BuggyCrashed.hpp"
#include "gui/HUD.hpp"
#include "gui/LevelComplete.hpp"
#include "gui/MainMenu.hpp"
#include "support/ArrayIterator.hpp"

#include <Camera2D.hpp>
#include <Godot.hpp>
#include <Node.hpp>

#include <algorithm>
#include <iterator>

namespace moon_buggy
{

  auto GUI::_register_methods() -> void
  {
    godot::register_method("_ready", &GUI::_ready);
    godot::register_method("show_buggy_crashed_screen", &GUI::show_buggy_crashed_screen);
    godot::register_method("show_hud", &GUI::show_hud);
    godot::register_method("show_level_complete_screen", &GUI::show_level_complete_screen);
    godot::register_method("show_main_menu", &GUI::show_main_menu);
  }

  auto GUI::_init() -> void
  {
  }

  auto GUI::_ready() -> void
  {
    buggy_crashed_screen = get_typed_node<BuggyCrashed>();
    hud = get_typed_node<HUD>();
    level_complete_screen = get_typed_node<LevelComplete>();
    main_menu = get_typed_node<MainMenu>();
    scroll_camera = get_typed_node<ScrollCamera>();

    CRASH_COND(!buggy_crashed_screen);
    CRASH_COND(!hud);
    CRASH_COND(!level_complete_screen);
    CRASH_COND(!main_menu);
    CRASH_COND(!scroll_camera);

    show_main_menu();
  }

  auto GUI::show_buggy_crashed_screen() -> void
  {
    hide_all_layers();
    buggy_crashed_screen->show();
  }

  auto GUI::show_hud() -> void
  {
    hide_all_layers();
    hud->show();
  }

  auto GUI::show_level_complete_screen(int level_number) -> void
  {
    hide_all_layers();
    level_complete_screen->call("set_level_number", level_number);
    level_complete_screen->show();
  }

  auto GUI::show_main_menu() -> void
  {
    hide_all_layers();
    main_menu->show();
    scroll_camera->make_current();
  }
 
  auto GUI::hide_all_layers() -> void
  {
    std::ranges::for_each(get_children(), [](auto child) {
      if (auto control = cast_to<godot::Control>(child))
      {
        control->hide();
      }
    });
  }

}  // namespace moon_buggy