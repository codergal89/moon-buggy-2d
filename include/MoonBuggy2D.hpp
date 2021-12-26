#ifndef MOON_BUGGY_2D_HPP
#define MOON_BUGGY_2D_HPP

#include "core/Fwd.hpp"
#include "game/Fwd.hpp"
#include "gui/Fwd.hpp"
#include "support/SilentGodotClass.hpp"
#include "support/TypedNodeCastMixin.hpp"

#include <Godot.hpp>
#include <Node.hpp>
#include <RandomNumberGenerator.hpp>

namespace moon_buggy
{
  struct MoonBuggy2D
      : godot::Node
      , TypedNodeCastMixin<MoonBuggy2D>
  {
    auto static _register_methods() -> void;

    auto _init() -> void;

  private:
    SILENT_GODOT_CLASS(MoonBuggy2D, godot::Node)

    /// Exported Methods
    auto _ready() -> void;

    auto on_start_game() -> void;

    /// Internal Variables
    godot::Ref<godot::RandomNumberGenerator> theme_rng;

    Game * game;
    GUI * gui;
    Space * space;

    unsigned level_number;
  };
}  // namespace moon_buggy

#endif