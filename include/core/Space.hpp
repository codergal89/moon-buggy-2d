#ifndef MOON_BUGGY_2D_CORE_SPACE_HPP
#define MOON_BUGGY_2D_CORE_SPACE_HPP

#include "support/SilentGodotClass.hpp"
#include "support/TypedNodeCastMixin.hpp"

#include <Defs.hpp>
#include <Dictionary.hpp>
#include <Godot.hpp>
#include <ParallaxBackground.hpp>
#include <String.hpp>
#include <TextureRect.hpp>

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

    /// Exported Methods
    auto _process(real_t delta) -> void;
    auto _ready() -> void;
    auto select_theme(godot::String theme) -> void;

    /// Internal Functions
    auto load_images() -> void;
    auto scroll(real_t delta) -> void;

    /// Exported Properties
    real_t autoscrolling_speed{};
    bool enable_autoscrolling{};

    /// Internal Variables
    godot::TextureRect * space_texture;
    godot::TextureRect * stars_texture;
    godot::Dictionary space_images;
    godot::Dictionary stars_images;
  };

}  // namespace moon_buggy

#endif