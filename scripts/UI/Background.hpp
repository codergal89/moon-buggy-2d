#ifndef MB2D_SCRIPTS_UI_BACKGROUND_HPP
#define MB2D_SCRIPTS_UI_BACKGROUND_HPP

#include "Helpers/DontWarn.hpp"

#include <godot_cpp/classes/parallax_background.hpp>

namespace mb2d
{

  struct Background : godot::ParallaxBackground
  {
    DONT_WARN(GDCLASS(Background, godot::ParallaxBackground))

    auto static _bind_methods() -> void
    {
    }
  };

}  // namespace mb2d

#endif