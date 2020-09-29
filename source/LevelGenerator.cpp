#include "LevelGenerator.hpp"

#include "Level.hpp"

#include <Godot.hpp>
#include <RandomNumberGenerator.hpp>

#include <algorithm>
#include <cstdint>
#include <iterator>

namespace moon_buggy
{

  auto LevelGenerator::_register_methods() -> void
  {
    godot::register_property("minimum_hole_length", &LevelGenerator::minimum_hole_length, default_minimum_hole_length);
    godot::register_property("maximum_hole_length", &LevelGenerator::maximum_hole_length, default_maximum_hole_length);
    godot::register_property("minimum_platform_length", &LevelGenerator::minimum_platform_length, default_minimum_platform_length);
    godot::register_property("maximum_platform_length", &LevelGenerator::maximum_platform_length, default_maximum_platform_length);
    godot::register_property("minimum_segments", &LevelGenerator::minimum_segments, default_minimum_segments);
    godot::register_property("maximum_segments", &LevelGenerator::maximum_segments, default_maximum_segments);
  }

  auto LevelGenerator::_init() -> void
  {
    random_number_generator = RandomNumberGeneratorPtr{godot::RandomNumberGenerator::_new()};
    random_number_generator->randomize();
  }

  auto LevelGenerator::generate(LevelDescriptor descriptor) -> Level
  {
    clamp(descriptor);

    auto level = Level{};
    auto maximum_level_size = descriptor.segments * (descriptor.maximum_platform_length + descriptor.maximum_hole_length);
    level.tiles.reserve(maximum_level_size);

    for (auto segment{0}; segment < descriptor.segments; ++segment)
    {
      auto hole_length = random_number_generator->randi_range(descriptor.minimum_hole_length, descriptor.maximum_hole_length);
      auto platform_length = random_number_generator->randi_range(descriptor.minimum_platform_length, descriptor.maximum_platform_length);

      generate_n(back_inserter(level.tiles), platform_length, [] { return tile_kind::ground; });
      generate_n(back_inserter(level.tiles), hole_length, [] { return tile_kind::hole; });
    }

    return level;
  }

  auto LevelGenerator::clamp(LevelDescriptor & descriptor) const -> void
  {
    auto do_clamp = [this](auto & value, auto min, auto max) {
      value = std::clamp(value, min, max);
    };

    do_clamp(descriptor.minimum_hole_length, minimum_hole_length, maximum_hole_length);
    do_clamp(descriptor.maximum_hole_length, minimum_hole_length, maximum_hole_length);
    do_clamp(descriptor.minimum_platform_length, minimum_platform_length, maximum_platform_length);
    do_clamp(descriptor.maximum_platform_length, minimum_platform_length, maximum_platform_length);
    do_clamp(descriptor.segments, minimum_segments, maximum_segments);
  }

}  // namespace moon_buggy