#ifndef MB2D_SCRIPTS_UI_COLOR_HPP
#define MB2D_SCRIPTS_UI_COLOR_HPP

#include "Helpers/DontWarn.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/error_macros.hpp>
#include <godot_cpp/variant/string_name.hpp>

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/wrapped.hpp>

namespace mb2d
{

  enum UIColor
  {
    UIColorBlue,
    UIColorPurple,
  };

  auto inline to_string(UIColor const & color) -> godot::StringName
  {
    switch (color)
    {
    case UIColor::UIColorBlue:
      return "blue";
    case UIColor::UIColorPurple:
      return "purple";
    default:
      CRASH_NOW_MSG("Invalid enum value for background color!");
    }
  }

}  // namespace mb2d

VARIANT_ENUM_CAST(mb2d::UIColor);

#endif