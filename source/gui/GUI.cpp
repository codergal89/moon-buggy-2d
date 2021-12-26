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
    CRASH_COND(main_menu.is_null());
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