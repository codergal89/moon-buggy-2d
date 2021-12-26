#ifndef MOON_BUGGY_2D_GUI_HUD_HPP
#define MOON_BUGGY_2D_GUI_HUD_HPP

#include "support/SilentGodotClass.hpp"
#include "support/TypedNodeCastMixin.hpp"

#include <Control.hpp>
#include <Godot.hpp>
#include <Label.hpp>

#include <cstdint>

namespace moon_buggy
{

  struct HUD
      : godot::Control
      , TypedNodeCastMixin<HUD>
  {
    auto static _register_methods() -> void;

    auto _init() -> void;

  private:
    SILENT_GODOT_CLASS(HUD, godot::Control)

    /// Exported Methods
    auto _ready() -> void;

    /// Exported Properties
    auto get_level_number() const -> std::int64_t;
    auto set_level_number(std::int64_t number) -> void;

    /// Internal Variables
    godot::Label * level_number{};
  };

}  // namespace moon_buggy

#endif