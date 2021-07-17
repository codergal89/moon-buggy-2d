#ifndef MOON_BUGGY_2D_CORE_THEME_HPP
#define MOON_BUGGY_2D_CORE_THEME_HPP

#include <PoolArrays.hpp>
#include <RandomNumberGenerator.hpp>
#include <Ref.hpp>
#include <String.hpp>

#include <optional>

namespace moon_buggy
{

  enum struct Theme
  {
    blue,
    purple,
  };

  auto random_theme(godot::Ref<godot::RandomNumberGenerator> rng) -> godot::String;

  auto available_themes() -> godot::PoolStringArray;

  auto find_theme(godot::String const & name) -> std::optional<Theme>;

  auto theme_name(Theme theme) -> godot::String;

}  // namespace moon_buggy

#endif