#ifndef MOON_BUGGY_2D_REFERENCE_DELETER_HPP
#define MOON_BUGGY_2D_REFERENCE_DELETER_HPP

#include <Reference.hpp>

struct ReferenceDeleter
{
  auto operator()(godot::Reference * object) -> void
  {
    object->unreference();
  }
};

#endif