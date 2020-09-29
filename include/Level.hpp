#ifndef MOON_BUGGY_2D_LEVEL_HPP
#define MOON_BUGGY_2D_LEVEL_HPP

#include <String.hpp>

#include <cstdint>
#include <vector>

namespace moon_buggy
{

  enum struct tile_kind
  {
    ground,
    hole,
  };

  struct Level
  {
    std::vector<tile_kind> tiles{};
  };

  struct LevelDescriptor
  {
    int segments{};
    int minimum_hole_length{};
    int maximum_hole_length{};
    int minimum_platform_length{};
    int maximum_platform_length{};
  };

  auto load_level_descriptors(godot::String path) -> std::vector<LevelDescriptor>;

}  // namespace moon_buggy

#endif