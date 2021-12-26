#include "gui/GUI.hpp"

#include "core/MeteorSpawner.hpp"
#include "core/ScrollCamera.hpp"
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
    godot::register_method("show_main_menu", &GUI::show_main_menu);
    godot::register_method("show_hud", &GUI::show_hud);

    godot::register_property("hud",
                             &GUI::hud,
                             {},
                             GODOT_METHOD_RPC_MODE_DISABLED,
                             GODOT_PROPERTY_USAGE_DEFAULT,
                             GODOT_PROPERTY_HINT_FILE,
                             "*.tscn");
    godot::register_property("main_menu",
                             &GUI::main_menu,
                             {},
                             GODOT_METHOD_RPC_MODE_DISABLED,
                             GODOT_PROPERTY_USAGE_DEFAULT,
                             GODOT_PROPERTY_HINT_FILE,
                             "*.tscn");
  }

  auto GUI::_init() -> void
  {
  }

  auto GUI::_ready() -> void
  {
    CRASH_COND(hud.is_null());
    CRASH_COND(main_menu.is_null());
  }

  auto GUI::show_hud(unsigned level_number) -> godot::Node *
  {
    hide_all_layers();
    auto hud_node = hud->instance();
    add_child(hud_node);
    hud_node->set("level_number", level_number);
    return hud_node;
  }

  auto GUI::show_main_menu() -> godot::Node *
  {
    hide_all_layers();
    auto main_menu_node = main_menu->instance();
    add_child(main_menu_node);
    return main_menu_node;
  }

  auto GUI::hide_all_layers() -> void
  {
    std::ranges::for_each(get_children(), [this](auto child) { this->remove_child(cast_to<godot::Node>(child)); });
  }

}  // namespace moon_buggy