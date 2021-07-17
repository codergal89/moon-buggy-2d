#ifndef MOON_BUGGY_2D_CORE_SPACE_HPP
#define MOON_BUGGY_2D_CORE_SPACE_HPP

#include "support/TypedNodeCastMixin.hpp"
#include "support/SilentGodotClass.hpp"

#include <Defs.hpp>
#include <Godot.hpp>
#include <ParallaxBackground.hpp>

namespace moon_buggy
{

  struct Space
      : godot::ParallaxBackground
      , TypedNodeCastMixin<Space>
  {
    auto static _register_methods() -> void;

    auto _init() -> void;

  private:
    SILENT_GODOT_CLASS(Space, godot::ParallaxBackground)

    // Exported Methods
    auto _process(real_t delta) -> void;

    // Internal Functions
    auto scroll(real_t delta) -> void;

    // Exported Properties
    real_t autoscrolling_speed{};
    bool enable_autoscrolling{};
  };

}  // namespace moon_buggy

#endif