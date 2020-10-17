#include "gui/LevelComplete.hpp"

#include <Array.hpp>
#include <Dictionary.hpp>
#include <Godot.hpp>
#include <Label.hpp>
#include <String.hpp>

namespace moon_buggy
{
  auto LevelComplete::_register_methods() -> void
  {
    godot::register_method("_ready", &LevelComplete::_ready);
    godot::register_method("set_level_number", &LevelComplete::set_level_number);
    godot::register_method("start_button_clicked", &LevelComplete::start_button_clicked);

    godot::register_signal<LevelComplete>("start_next_level", godot::Dictionary{});
  }

  auto LevelComplete::_init() -> void
  {
  }

  auto LevelComplete::_ready() -> void
  {
    level_number = get_typed_node<godot::Label>("RowContainer/MessageContainer/LevelNumber");
  }

  auto LevelComplete::set_level_number(int number) -> void
  {
    auto static level_number_format = godot::String{"#{0}"};
    level_number->set_text(level_number_format.format(godot::Array::make(number)));
  }

  auto LevelComplete::start_button_clicked() -> void
  {
    emit_signal("start_next_level");
  }

}  // namespace moon_buggy