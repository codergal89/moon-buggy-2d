#include "LevelGenerator.hpp"

#include "Level.hpp"

#include <Godot.hpp>
#include <RandomNumberGenerator.hpp>

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
    random_number_generator = RandomNumberGeneratorPtr{godot::RandomNumberGenerator::_new()};
    random_number_generator->randomize();
  }

  auto LevelGenerator::generate(unsigned difficulty) -> Level
  {
    static_cast<void>(difficulty);

    auto level = Level{};
    level.tiles.reserve(start_area_length);

    generate_n(back_inserter(level.tiles), start_area_length, [] { return tile_kind::ground; });

    for (auto segment{0}; segment < 8; ++segment)
    {
      auto ground_length = random_number_generator->randi_range(7, 12);
      auto hole_lentgth = random_number_generator->randi_range(2, 4);

      generate_n(back_inserter(level.tiles), ground_length, [] { return tile_kind::ground; });
      generate_n(back_inserter(level.tiles), hole_lentgth, [] { return tile_kind::hole; });
    }

    generate_n(back_inserter(level.tiles), start_area_length, [] { return tile_kind::ground; });

    return level;
  }

}  // namespace moon_buggy