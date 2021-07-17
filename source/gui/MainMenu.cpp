#include "gui/MainMenu.hpp"

#include "core/MeteorSpawner.hpp"

#include <Dictionary.hpp>
#include <Godot.hpp>
#include <ParallaxBackground.hpp>
#include <Vector2.hpp>

namespace moon_buggy
{

  auto MainMenu::_register_methods() -> void
  {
    godot::register_method("on_start_button_clicked", &MainMenu::on_start_button_clicked);

    godot::register_signal<MainMenu>("start_game", godot::Dictionary{});
  }

  auto MainMenu::_init() -> void
  {
  }

  auto MainMenu::on_start_button_clicked() -> void
  {
    emit_signal("start_game");
  }

}  // namespace moon_buggy