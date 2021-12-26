#include "gui/MainMenu.hpp"

#include "core/MeteorSpawner.hpp"
#include "core/Theming.hpp"
#include "gui/Signals.hpp"

#include <Dictionary.hpp>
#include <Godot.hpp>
#include <RandomNumberGenerator.hpp>
#include <Vector2.hpp>

namespace moon_buggy
{

  auto MainMenu::_register_methods() -> void
  {
    godot::register_method("_ready", &MainMenu::_ready);
    godot::register_method("on_start_button_clicked", &MainMenu::on_start_button_clicked);

    godot::register_signal<MainMenu>(gui::signals::start_game, godot::Dictionary{});
  }

  auto MainMenu::_init() -> void
  {
    theme_rng.instance();
    theme_rng->randomize();
  }

  auto MainMenu::_ready() -> void
  {
    auto background = get_node("Background/Space");
    background->call("select_theme", random_theme(theme_rng));
  }

  auto MainMenu::on_start_button_clicked() -> void
  {
    emit_signal(gui::signals::start_game);
  }

}  // namespace moon_buggy