#ifndef MOON_BUGGY_2D_MAP_HPP
#define MOON_BUGGY_2D_MAP_HPP

#include "core/Level.hpp"
#include "support/TypedNodeCastMixin.hpp"

#include <Array.hpp>
#include <Godot.hpp>
#include <ImageTexture.hpp>
#include <Node2D.hpp>
#include <Ref.hpp>
#include <TileMap.hpp>

#include <cstdint>
#include <map>

namespace moon_buggy
{

  struct Map
      : godot::Node2D
      , TypedNodeCastMixin<Map>
  {
    auto static _register_methods() -> void;

    auto _init() -> void;
    auto _ready() -> void;

    auto set_level(Level * level, std::uint64_t width, std::uint64_t height) -> void;
    auto get_world_end() -> std::int64_t;

  private:
    GODOT_CLASS(Map, godot::Node2D)  // NOLINT

    auto generate_surface(Level const & level, std::int64_t x_tiles_per_screen, std::int64_t bottom, std::int64_t right) const -> void;

    std::map<Level::Tile, std::int64_t> m_tile_ids{};

    std::int64_t end_tile{};

    godot::Array ground_textures{};
    godot::Ref<godot::ImageTexture> ground_texture{};
    godot::TileMap * ground{};
    godot::TileMap * holes{};
  };

}  // namespace moon_buggy

#endif