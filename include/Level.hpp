#ifndef MOON_BUGGY_2D_LEVEL_HPP
#define MOON_BUGGY_2D_LEVEL_HPP

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

}  // namespace moon_buggy

#endif