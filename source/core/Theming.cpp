#include "core/Theming.hpp"

#include <GodotGlobal.hpp>
#include <PoolArrays.hpp>
#include <RandomNumberGenerator.hpp>
#include <Ref.hpp>
#include <String.hpp>

#include <algorithm>
#include <array>
#include <iterator>
#include <utility>

namespace moon_buggy
{
  namespace
  {
    auto constexpr theme_names = std::array{
        std::pair{Theme::blue, "blue"},
        std::pair{Theme::purple, "purple"},
    };

  }

  auto random_theme(godot::Ref<godot::RandomNumberGenerator> rng) -> godot::String
  {
    auto number_of_themes = static_cast<int>(theme_names.size());
    auto random_number = rng->randi_range(0, number_of_themes - 1);
    return theme_names[random_number].second;
  }

  auto available_themes() -> godot::PoolStringArray
  {
    auto themes = godot::PoolStringArray{};
    for (auto const & entry : theme_names)
    {
      themes.push_back(entry.second);
    }
    return themes;
  }

  auto find_theme(godot::String const & name) -> std::optional<Theme>
  {
    auto found = find_if(cbegin(theme_names), cend(theme_names), [&](auto const & entry) { return entry.second == name; });
    if (found != cend(theme_names))
    {
      return found->first;
    }

    return {};
  }

  auto theme_name(Theme theme) -> godot::String
  {
    auto found = find_if(cbegin(theme_names), cend(theme_names), [=](auto const & entry) { return entry.first == theme; });
    if (found == cend(theme_names))
    {
      ERR_PRINT(("Internal inconsistency. Failed to map theme id to actual theme"));
      CRASH_NOW();
    }

    return found->second;
  }

}  // namespace moon_buggy