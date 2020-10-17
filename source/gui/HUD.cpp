#include "gui/HUD.hpp"

#include <Godot.hpp>
#include <Label.hpp>
#include <String.hpp>

namespace moon_buggy
{

  auto HUD::_register_methods() -> void
  {
    godot::register_method("_ready", &HUD::_ready);
    godot::register_method("set_level_number", &HUD::set_level_number);
  }

  auto HUD::_init() -> void
  {
  }

  auto HUD::_ready() -> void
  {
    level_number = get_typed_node<godot::Label>("RowContainer/TopRow/LevelNumber");
  }

  auto HUD::set_level_number(int number) -> void
  {
    auto string_representation = godot::String::num_int64(number);
    level_number->set_text(string_representation);
  }

}  // namespace moon_buggy