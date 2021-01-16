#include "gui/GUI.hpp"

#include "support/ArrayIterator.hpp"

#include <Godot.hpp>
#include <Node.hpp>

#include <algorithm>
#include <iterator>

namespace moon_buggy
{

  auto GUI::_register_methods() -> void
  {
    godot::register_method("_ready", &GUI::_ready);
    godot::register_method("set_level_number", &GUI::set_level_number);
    godot::register_method("show_hud", &GUI::show_hud);
    godot::register_method("show_level_complete_screen", &GUI::show_level_complete_screen);
    godot::register_method("show_main_menu", &GUI::show_main_menu);
  }

  auto GUI::_init() -> void
  {
  }

  auto GUI::_ready() -> void
  {
    hud = get_typed_node<HUD>("HUD");
    level_complete_screen = get_typed_node<LevelComplete>("LevelComplete");
    main_menu = get_typed_node<MainMenu>("MainMenu");

    CRASH_COND(!hud);
    CRASH_COND(!level_complete_screen);
    CRASH_COND(!main_menu);
  }

  auto GUI::set_level_number(int number) -> void
  {
    hud->set_level_number(number);
    level_complete_screen->set_level_number(number);
  }

  auto GUI::show_hud() -> void
  {
    hide_all_layers();
    hud->show();
  }

  auto GUI::show_level_complete_screen() -> void
  {
    hide_all_layers();
    level_complete_screen->show();
  }

  auto GUI::show_main_menu() -> void
  {
    hide_all_layers();
    main_menu->show();
  }

  auto GUI::hide_all_layers() -> void
  {
    auto layers = get_children();
    std::for_each(begin(layers), end(layers), [](auto layer) { cast_to<godot::Node>(layer)->call("hide"); });
  }

}  // namespace moon_buggy