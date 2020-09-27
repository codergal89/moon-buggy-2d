#include "LevelGenerator.hpp"

#include "Level.hpp"

#include <Godot.hpp>

#include <algorithm>
#include <iterator>

namespace moon_buggy
{

  auto LevelGenerator::_register_methods() -> void
  {
    godot::register_property("start_area_length", &LevelGenerator::start_area_length, default_start_area_length);
  }

  auto LevelGenerator::_init() -> void
  {
  }

  auto LevelGenerator::generate(unsigned difficulty) -> Level
  {
    static_cast<void>(difficulty);

    auto level = Level{};
    level.tiles.reserve(start_area_length);

    std::generate_n(back_inserter(level.tiles), start_area_length, [] { return tile_kind::ground; });

    return level;
  }

}  // namespace moon_buggy