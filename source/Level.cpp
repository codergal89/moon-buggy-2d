#include "Level.hpp"

#include <Array.hpp>
#include <Dictionary.hpp>
#include <File.hpp>
#include <JSON.hpp>
#include <JSONParseResult.hpp>
#include <Ref.hpp>
#include <String.hpp>
#include <Variant.hpp>

#include <experimental/source_location>
#include <memory>

namespace moon_buggy
{

  auto check_array_entry_type(godot::Variant const & entry,
                              std::experimental::source_location where = std::experimental::source_location::current()) -> godot::Variant
  {
    if (entry.get_type() != godot::Variant::DICTIONARY)
    {
      godot::Godot::print_warning("found invalid level descriptor", where.function_name(), where.file_name(), where.line());
      return godot::Variant::NIL;
    }
    return entry;
  }

  auto get_int(godot::String key, godot::Dictionary const & dictionary, int default_value) -> int
  {
    if (dictionary.has(key) && dictionary[key].get_type() == godot::Variant::REAL)
    {
      return static_cast<int>(dictionary[key]);
    }
    return default_value;
  }

  auto load_level_descriptors(godot::String path) -> std::vector<LevelDescriptor>
  {
    auto config_file = godot::Ref{godot::File::_new()};
    if (config_file->open(path, godot::File::READ) != godot::Error::OK)
    {
      return {};
    }

    auto configuration = config_file->get_as_text();
    auto parsed = godot::JSON::get_singleton()->parse(configuration);

    if (parsed->get_error() != godot::Error::OK || parsed->get_result().get_type() != godot::Variant::ARRAY)
    {
      return {};
    }

    auto entries = static_cast<godot::Array>(parsed->get_result());
    auto descriptors = std::vector<LevelDescriptor>{};
    for (auto i{0}; i < entries.size(); ++i)
    {
      if (auto entry = check_array_entry_type(entries[i]))
      {
        auto dictionary = static_cast<godot::Dictionary>(entry);
        descriptors.push_back(LevelDescriptor{
            get_int("segments", dictionary, 0),
            get_int("minimum_hole_length", dictionary, 0),
            get_int("maximum_hole_length", dictionary, 0),
            get_int("minimum_platform_length", dictionary, 0),
            get_int("maximum_platform_length", dictionary, 0),
        });
      }
    }

    return descriptors;
  }

}  // namespace moon_buggy