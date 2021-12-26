#include "MoonBuggy2D.hpp"

#include "core/Space.hpp"
#include "core/Theming.hpp"
#include "game/Game.hpp"
#include "gui/GUI.hpp"
#include "gui/Signals.hpp"

#include <Godot.hpp>

namespace moon_buggy
{

  auto MoonBuggy2D::_register_methods() -> void
  {
    godot::register_method("_ready", &MoonBuggy2D::_ready);
    godot::register_method("on_start_game", &MoonBuggy2D::on_start_game);
  }

  auto MoonBuggy2D::_init() -> void
  {
    theme_rng.instance();
    theme_rng->randomize();
    theme = random_theme(theme_rng);
  }

  auto MoonBuggy2D::_ready() -> void
  {
    game = get_typed_node<Game>("Game");
    gui = get_typed_node<GUI>("GUI");
    space = get_typed_node<Space>("Background/Space");

    space->call("select_theme", theme);
    space->set("enable_autoscrolling", true);

    auto main_menu = static_cast<godot::Node *>(gui->call("show_main_menu"));
    main_menu->connect(gui::signals::start_game, this, "on_start_game");
  }

  auto MoonBuggy2D::on_start_game() -> void
  {
    gui->call("show_hud", godot::Array::make(++level_number));
    space->set("enable_autoscrolling", false);
    game->call("start");
  }

}  // namespace moon_buggy