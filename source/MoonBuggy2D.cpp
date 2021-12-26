#include "MoonBuggy2D.hpp"

#include <Godot.hpp>

#include "game/Game.hpp"
#include "gui/GUI.hpp"

namespace moon_buggy
{

auto MoonBuggy2D::_register_methods() -> void
{
    godot::register_method("_ready", &MoonBuggy2D::_ready);
}

auto MoonBuggy2D::_init() -> void
{

}

auto MoonBuggy2D::_ready() -> void
{
    game = get_typed_node<Game>("Game");
    gui = get_typed_node<GUI>("GUI");

    gui->call("show_main_menu");
}

}