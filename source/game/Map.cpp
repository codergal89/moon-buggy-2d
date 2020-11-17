#include "game/Map.hpp"

#include "core/Level.hpp"
#include "support/ArrayIterator.hpp"

#include <Defs.hpp>
#include <GodotGlobal.hpp>
#include <Ref.hpp>
#include <TileMap.hpp>
#include <TileSet.hpp>
#include <Vector2.hpp>

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <vector>

namespace moon_buggy
{

  auto Map::_register_methods() -> void
  {
    godot::register_method("_ready", &Map::_ready);
    godot::register_method("set_level", &Map::set_level);
    godot::register_method("get_world_end", &Map::get_world_end);

    godot::register_property("tile_set", &Map::tile_set, decltype(Map::tile_set){});
  }

  auto Map::_init() -> void
  {
  }

  auto Map::_ready() -> void
  {
    ground = get_typed_node<godot::TileMap>("Ground");
    ground->set_tileset(tile_set);
  }

  auto Map::set_level(Level * level, std::uint64_t width, std::uint64_t height) -> void
  {
    ground->clear();

    auto cell_size = ground->get_cell_size();
    auto x_tiles_per_screen = static_cast<std::int64_t>((width + cell_size.x - 1) / cell_size.x);
    auto y_tiles_per_screen = static_cast<std::int64_t>((height + cell_size.y - 1) / cell_size.y);

    end_tile = -(level->tiles.size() + x_tiles_per_screen);
    auto tile_ids = std::vector<std::int64_t>();
    tile_ids.reserve(level->tiles.size() + 2 * x_tiles_per_screen);

    auto ground_tile = tile_set->find_tile_by_name(ground_tile_name);
    auto hole_tile = tile_set->find_tile_by_name(hole_tile_name);
    auto left_shoulder_tile = tile_set->find_tile_by_name(left_shoulder_tile_name);
    auto right_shoulder_tile = tile_set->find_tile_by_name(right_shoulder_tile_name);

    generate_n(back_inserter(tile_ids), x_tiles_per_screen, [&] { return ground_tile; });

    transform(cbegin(level->tiles), cend(level->tiles), back_inserter(tile_ids), [&](auto tile) {
      switch (static_cast<Level::Tile>(static_cast<int>(tile)))
      {
      case Level::Tile::ground:
        return ground_tile;
      case Level::Tile::hole:
        return hole_tile;
      case Level::Tile::left_shoulder:
        return left_shoulder_tile;
      case Level::Tile::right_shoulder:
        return right_shoulder_tile;
      default:
        return static_cast<std::int64_t>(-1);
      }
    });

    generate_n(back_inserter(tile_ids), x_tiles_per_screen, [&] { return ground_tile; });

    auto right = x_tiles_per_screen - 1;
    auto bottom = y_tiles_per_screen - 1;
    for_each(cbegin(tile_ids), cend(tile_ids), [x = 0, this, bottom, right](auto tile_id) mutable {
      ground->set_cell(right - x++, bottom, tile_id);
    });
  }

  auto Map::get_world_end() -> std::int64_t
  {
    return static_cast<std::int64_t>(ground->map_to_world({static_cast<real_t>(end_tile), 0.f}).x);
  }

}  // namespace moon_buggy