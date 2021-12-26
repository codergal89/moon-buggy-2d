#include "gui/HUD.hpp"

#include <Godot.hpp>
#include <Label.hpp>
#include <String.hpp>

namespace moon_buggy
{

  auto HUD::_register_methods() -> void
  {
    godot::register_method("_ready", &HUD::_ready);

    godot::register_property("level_number", &HUD::set_level_number, &HUD::get_level_number, {});
  }

  auto HUD::_init() -> void
  {
  }

  auto HUD::_ready() -> void
  {
    level_number = get_typed_node<godot::Label>("RowContainer/TopRow/LevelNumber");
  }

  auto HUD::get_level_number() const -> std::int64_t
  {
    auto label_text = static_cast<godot::String>(level_number->get("text"));
    ERR_FAIL_COND_V(!label_text.is_valid_integer(), 0);

    auto number = label_text.to_int();
    ERR_FAIL_COND_V(number < 0, 0);

    return number;
  }

  auto HUD::set_level_number(std::int64_t number) -> void
  {
    level_number->set("text", godot::String::num_int64(number));
  }

}  // namespace moon_buggy