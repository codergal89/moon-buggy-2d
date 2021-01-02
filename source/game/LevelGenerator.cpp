#include "game/LevelGenerator.hpp"

#include "core/Level.hpp"
#include "support/ArrayIterator.hpp"

#include <Array.hpp>
#include <Defs.hpp>
#include <Dictionary.hpp>
#include <File.hpp>
#include <Godot.hpp>
#include <JSON.hpp>
#include <JSONParseResult.hpp>
#include <RandomNumberGenerator.hpp>
#include <Ref.hpp>
#include <String.hpp>
#include <Variant.hpp>

#include <algorithm>
#include <tuple>

namespace moon_buggy
{

  namespace
  {
    auto constexpr default_minimum_hole_length{3};
    auto constexpr default_maximum_hole_length{10};
    auto constexpr default_minimum_platform_length{5};
    auto constexpr default_maximum_platform_length{15};
    auto constexpr default_minimum_segments{4};
    auto constexpr default_maximum_segments{32};

    auto check_json_parse_result(godot::Ref<godot::JSONParseResult> result) -> bool
    {
      if (result->get_error() != godot::Error::OK)
      {
        ERR_PRINTS(godot::String{"Failed to parse level descriptors: {0}"}.format(godot::Array::make(result->get_error_string())));
        return false;
      }
      else if (result->get_result().get_type() != godot::Variant::ARRAY)
      {
        ERR_PRINT("Failed to load level descriptors: expected array at top-level");
        return false;
      }
      return true;
    }

    auto get_int(godot::String key, godot::Dictionary const & dictionary, int default_value) -> int
    {
      if (dictionary.has(key) && dictionary[key].get_type() == godot::Variant::REAL)
      {
        return static_cast<int>(dictionary[key]);
      }
      return default_value;
    }
  }  // namespace

  auto LevelGenerator::_register_methods() -> void
  {
    godot::register_method("_ready", &LevelGenerator::_ready);
    godot::register_method("generate_next", &LevelGenerator::generate_next);
    godot::register_method("get_remaining_level_count", &LevelGenerator::get_remaining_level_count);
    godot::register_method("has_remaining_levels", &LevelGenerator::has_remaining_levels);

    godot::register_property("level_descriptor_source",
                             &LevelGenerator::level_descriptor_source,
                             {},
                             GODOT_METHOD_RPC_MODE_DISABLED,
                             GODOT_PROPERTY_USAGE_DEFAULT,
                             GODOT_PROPERTY_HINT_FILE,
                             "*.json");
    godot::register_property("minimum_hole_length", &LevelGenerator::minimum_hole_length, default_minimum_hole_length);
    godot::register_property("maximum_hole_length", &LevelGenerator::maximum_hole_length, default_maximum_hole_length);
    godot::register_property("minimum_platform_length", &LevelGenerator::minimum_platform_length, default_minimum_platform_length);
    godot::register_property("maximum_platform_length", &LevelGenerator::maximum_platform_length, default_maximum_platform_length);
    godot::register_property("minimum_segments", &LevelGenerator::minimum_segments, default_minimum_segments);
    godot::register_property("maximum_segments", &LevelGenerator::maximum_segments, default_maximum_segments);
  }

  auto LevelGenerator::_init() -> void
  {
    random_number_generator.instance();
    random_number_generator->randomize();

    minimum_hole_length = default_minimum_hole_length;
    maximum_hole_length = default_maximum_hole_length;
    minimum_platform_length = default_minimum_platform_length;
    maximum_platform_length = default_maximum_platform_length;
    minimum_segments = default_minimum_segments;
    maximum_segments = default_maximum_segments;
  }

  auto LevelGenerator::_ready() -> void
  {
    CRASH_COND(level_descriptor_source.empty());
    load(level_descriptor_source);
  }

  auto LevelGenerator::load(godot::String descriptor_file_path) -> int
  {
    auto file = godot::Ref{godot::File::_new()};
    if (file->open(descriptor_file_path, godot::File::READ) != godot::Error::OK)
    {
      ERR_PRINTS(
          godot::String{"Failed to load level descriptors. File '{0}' does not exists"}.format(godot::Array::make(descriptor_file_path)));
      return 0;
    }

    auto json = godot::JSON::get_singleton()->parse(file->get_as_text());
    if (!check_json_parse_result(json))
    {
      return 0;
    }

    auto entries = static_cast<godot::Array>(json->get_result());
    for (auto index{0}; index < entries.size(); ++index)
    {
      auto entry = entries[index];
      if (entry.get_type() != godot::Variant::DICTIONARY)
      {
        WARN_PRINT("Skipping invalid level descriptor");
        continue;
      }

      auto parameters = static_cast<godot::Dictionary>(entry);
      auto [segments, min_hole, max_hole, min_platform, max_platform] = std::tuple{
          std::clamp(get_int("segments", parameters, 0), minimum_segments, maximum_segments),
          std::clamp(get_int("minimum_hole_length", parameters, 0), minimum_hole_length, maximum_hole_length),
          std::clamp(get_int("maximum_hole_length", parameters, 0), minimum_hole_length, maximum_hole_length),
          std::clamp(get_int("minimum_platform_length", parameters, 0), minimum_platform_length, maximum_platform_length),
          std::clamp(get_int("maximum_platform_length", parameters, 0), minimum_platform_length, maximum_platform_length),
      };

      level_descriptors.push(make_object_ptr(LevelDescriptor::_new(segments, min_hole, max_hole, min_platform, max_platform)));
    }

    return static_cast<int>(level_descriptors.size());
  }

  auto LevelGenerator::generate_next() -> Level *
  {
    ERR_FAIL_COND_V(level_descriptors.empty(), nullptr);

    auto descriptor = std::move(level_descriptors.front());
    level_descriptors.pop();

    auto generated = make_object_ptr(Level::_new());

    for (auto segment{0}; segment < descriptor->segments; ++segment)
    {
      auto hole_length = random_number_generator->randi_range(descriptor->minimum_hole_length, descriptor->maximum_hole_length);
      auto platform_length = random_number_generator->randi_range(descriptor->minimum_platform_length, descriptor->maximum_platform_length);

      std::generate_n(back_inserter(generated->surface_tiles), platform_length, [] { return static_cast<int>(Level::Tile::ground_surface); });
      generated->surface_tiles.push_back(static_cast<int>(Level::Tile::right_shoulder));
      std::generate_n(back_inserter(generated->surface_tiles), hole_length, [] { return static_cast<int>(Level::Tile::hole); });
      generated->surface_tiles.push_back(static_cast<int>(Level::Tile::left_shoulder));
    }

    return generated.release();
  }

  auto LevelGenerator::get_remaining_level_count() const -> int
  {
    return static_cast<int>(level_descriptors.size());
  }

  auto LevelGenerator::has_remaining_levels() const -> bool
  {
    return !level_descriptors.empty();
  }

}  // namespace moon_buggy