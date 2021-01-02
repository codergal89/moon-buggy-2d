#ifndef MOON_BUGGY_2D_GAME_LEVEL_GENERATOR_HPP
#define MOON_BUGGY_2D_GAME_LEVEL_GENERATOR_HPP

#include "core/Fwd.hpp"
#include "core/LevelDescriptor.hpp"
#include "support/ObjectPointer.hpp"
#include "support/TypedNodeCastMixin.hpp"

#include <Godot.hpp>
#include <Node.hpp>
#include <RandomNumberGenerator.hpp>
#include <Ref.hpp>
#include <String.hpp>

#include <queue>

namespace moon_buggy
{

  struct LevelGenerator
      : godot::Node
      , TypedNodeCastMixin<LevelGenerator>
  {
    auto static _register_methods() -> void;

    auto _init() -> void;
    auto _ready() -> void;

    auto generate_next() -> Level *;
    [[nodiscard]] auto get_remaining_level_count() const -> int;
    [[nodiscard]] auto has_remaining_levels() const -> bool;

    GODOT_CLASS(LevelGenerator, godot::Node)  // NOLINT

  private:
    auto load(godot::String descriptor_file_path) -> int;

    godot::Ref<godot::RandomNumberGenerator> random_number_generator{};
    std::queue<object_ptr<LevelDescriptor>> level_descriptors{};

    godot::String level_descriptor_source{};

    int minimum_hole_length;
    int maximum_hole_length;
    int minimum_platform_length;
    int maximum_platform_length;
    int minimum_segments;
    int maximum_segments;
  };

}  // namespace moon_buggy

#endif