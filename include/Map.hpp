#ifndef MOON_BUGGY_2D_MAP_HPP
#define MOON_BUGGY_2D_MAP_HPP

#include "Level.hpp"
#include "TypedNodeCastMixin.hpp"

#include <Godot.hpp>
#include <Node2D.hpp>
#include <Ref.hpp>
#include <TileMap.hpp>
#include <TileSet.hpp>

#include <cstdint>

namespace moon_buggy
{

  struct Map
      : godot::Node2D
      , TypedNodeCastMixin<Map>
  {
    auto static constexpr ground_tile_name{"ground"};
    auto static constexpr hole_tile_name{"hole"};

    auto static _register_methods() -> void;

    auto _init() -> void;
    auto _ready() -> void;

    auto level(Level level, std::uint64_t width, std::uint64_t height) -> void;
    auto world_end() -> std::int64_t;

  private:
    GODOT_CLASS(Map, godot::Node2D);

    std::int64_t end_tile{};

    godot::Ref<godot::TileSet> tile_set{};

    godot::TileMap * ground{};
    godot::TileMap * holes{};
  };

}  // namespace moon_buggy

#endif