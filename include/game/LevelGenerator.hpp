#ifndef MOON_BUGGY_2D_LEVEL_GENERATOR_HPP
#define MOON_BUGGY_2D_LEVEL_GENERATOR_HPP

#include "core/Level.hpp"
#include "core/LevelDescriptor.hpp"
#include "support/ObjectPointer.hpp"
#include "support/TypedNodeCastMixin.hpp"

#include <Godot.hpp>
#include <Node.hpp>
#include <RandomNumberGenerator.hpp>
#include <Ref.hpp>

#include <queue>

namespace moon_buggy
{

  struct LevelGenerator
      : godot::Node
      , TypedNodeCastMixin<LevelGenerator>
  {
    auto static constexpr default_minimum_hole_length{3};
    auto static constexpr default_maximum_hole_length{10};
    auto static constexpr default_minimum_platform_length{5};
    auto static constexpr default_maximum_platform_length{15};
    auto static constexpr default_minimum_segments{4};
    auto static constexpr default_maximum_segments{32};

    auto static _register_methods() -> void;

    auto _init() -> void;

    auto load(godot::String descriptor_file_path) -> int;
    auto generate_next() -> Level *;
    [[nodiscard]] auto get_remaining_level_count() const -> int;
    [[nodiscard]] auto has_remaining_levels() const -> bool;

    GODOT_CLASS(LevelGenerator, godot::Node)  // NOLINT

  private:
    godot::Ref<godot::RandomNumberGenerator> random_number_generator{};
    std::queue<object_ptr<LevelDescriptor>> level_descriptors{};

    int minimum_hole_length{default_minimum_hole_length};
    int maximum_hole_length{default_maximum_hole_length};
    int minimum_platform_length{default_minimum_platform_length};
    int maximum_platform_length{default_maximum_platform_length};
    int minimum_segments{default_minimum_segments};
    int maximum_segments{default_maximum_segments};
  };

}  // namespace moon_buggy

#endif