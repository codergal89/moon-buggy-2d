#include "gui/MainMenu.hpp"

#include "core/MeteorSpawner.hpp"

#include <Button.hpp>
#include <Dictionary.hpp>
#include <Godot.hpp>
#include <Label.hpp>

namespace moon_buggy
{

  auto MainMenu::_register_methods() -> void
  {
    godot::register_method("_ready", &MainMenu::_ready);
    godot::register_method("on_start_button_clicked", &MainMenu::on_start_button_clicked);
    godot::register_method("on_visibility_changed", &MainMenu::on_visibility_changed);

    godot::register_signal<MainMenu>("start_game", godot::Dictionary{});
  }

  auto MainMenu::_init() -> void
  {
  }

  auto MainMenu::_ready() -> void
  {
    meteor_spawner = get_typed_node<MeteorSpawner>("MeteorSpawnerPath/Spawner");
    CRASH_COND(!meteor_spawner);
  }

  auto MainMenu::on_start_button_clicked() -> void
  {
    emit_signal("start_game");
  }

  auto MainMenu::on_visibility_changed() -> void
  {
    if (is_visible())
    {
      meteor_spawner->call("start");
    }
    else
    {
      meteor_spawner->call("stop");
    }
  }

}  // namespace moon_buggy