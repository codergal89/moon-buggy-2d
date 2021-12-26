#include "MoonBuggy2D.hpp"

#include <Godot.hpp>

#include "game/Game.hpp"
#include "gui/GUI.hpp"
#include "gui/Signals.hpp"

namespace moon_buggy
{

auto MoonBuggy2D::_register_methods() -> void
{
    godot::register_method("_ready", &MoonBuggy2D::_ready);
    godot::register_method("on_start_game", &MoonBuggy2D::on_start_game);
}

auto MoonBuggy2D::_init() -> void
{

}

auto MoonBuggy2D::_ready() -> void
{
    game = get_typed_node<Game>("Game");
    gui = get_typed_node<GUI>("GUI");

    auto main_menu = static_cast<godot::Node *>(gui->call("show_main_menu"));
    main_menu->connect(gui::signals::start_game, this, "on_start_game");
}

auto MoonBuggy2D::on_start_game() -> void
{
    game->call("start");
}

}