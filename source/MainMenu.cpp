#include "MainMenu.hpp"

#include <Button.hpp>
#include <Dictionary.hpp>
#include <Godot.hpp>
#include <Label.hpp>

namespace moon_buggy
{

  auto MainMenu::_register_methods() -> void
  {
    godot::register_method("start_button_clicked", &MainMenu::start_button_clicked);

    godot::register_signal<MainMenu>("start_game", godot::Dictionary{});
  }

  auto MainMenu::_init() -> void
  {
  }

  auto MainMenu::start_button_clicked() -> void
  {
    emit_signal("start_game");
  }

}  // namespace moon_buggy