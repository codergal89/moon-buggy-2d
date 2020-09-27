#include "Map.hpp"

#include <Defs.hpp>
#include <GodotGlobal.hpp>
#include <Ref.hpp>
#include <TileMap.hpp>
#include <TileSet.hpp>
#include <Vector2.hpp>
#include <Viewport.hpp>

#include <algorithm>
#include <cstdint>
#include <iterator>

namespace moon_buggy
{

  auto Map::_register_methods() -> void
  {
    godot::register_method("_ready", &Map::_ready);
    godot::register_property("tile_set", &Map::tile_set, decltype(Map::tile_set){});
  }

  auto Map::_init() -> void
  {
  }

  auto Map::_ready() -> void
  {
    ground = get_typed_node<godot::TileMap>("Ground");
    ground->set_tileset(tile_set);

    auto view_port = get_viewport();
    auto cell_size = ground->get_cell_size();

    x_tiles_per_screen = static_cast<std::int64_t>(view_port->get_size().x / cell_size.x);
    y_tiles_per_screen = static_cast<std::int64_t>(view_port->get_size().y / cell_size.y);
  }

  auto Map::level(Level level) -> void
  {
    auto tile_ids = std::vector<std::int64_t>();
    tile_ids.reserve(level.tiles.size());
    transform(cbegin(level.tiles), cend(level.tiles), back_inserter(tile_ids), [this](auto tile) {
      switch (tile)
      {
      case tile_kind::ground:
        return tile_set->find_tile_by_name(ground_tile_name);
      case tile_kind::hole:
        return tile_set->find_tile_by_name(hole_tile_name);
      default:
        return static_cast<std::int64_t>(-1);
      }
    });

    for_each(cbegin(tile_ids), cend(tile_ids), [x = 0, this](auto tile_id) mutable {
      auto right = x_tiles_per_screen - 1;
      auto bottom = y_tiles_per_screen - 1;
      ground->set_cell(right - x++, bottom, tile_id);
    });
  }

}  // namespace moon_buggy