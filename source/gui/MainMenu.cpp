#include "gui/MainMenu.hpp"

#include "core/MeteorSpawner.hpp"

#include <Dictionary.hpp>
#include <Godot.hpp>
#include <ParallaxBackground.hpp>
#include <Vector2.hpp>

namespace moon_buggy
{

  namespace
  {
    auto constexpr default_background_scroll_speed = real_t{100.f};
  }

  auto MainMenu::_register_methods() -> void
  {
    godot::register_method("_process", &MainMenu::_process);
    godot::register_method("_ready", &MainMenu::_ready);
    godot::register_method("on_start_button_clicked", &MainMenu::on_start_button_clicked);

    godot::register_property("background_scroll_speed", &MainMenu::background_scroll_speed, default_background_scroll_speed);

    godot::register_signal<MainMenu>("start_game", godot::Dictionary{});
  }

  auto MainMenu::_init() -> void
  {
    background_scroll_speed = default_background_scroll_speed;
  }

  auto MainMenu::_process(real_t delta) -> void
  {
    auto current_offset = background->get_scroll_offset();
    auto offset_delta = godot::Vector2::RIGHT * background_scroll_speed * delta;
    background->set_scroll_offset(current_offset + offset_delta);
  }

  auto MainMenu::_ready() -> void
  {
    background = get_typed_node<godot::ParallaxBackground>("Background");
  }

  auto MainMenu::on_start_button_clicked() -> void
  {
    emit_signal("start_game");
  }

}  // namespace moon_buggy