#include "Map.hpp"

#include <Defs.hpp>
#include <GodotGlobal.hpp>
#include <Ref.hpp>
#include <TileMap.hpp>
#include <TileSet.hpp>
#include <Vector2.hpp>

#include <algorithm>
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

    holes = get_typed_node<godot::TileMap>("Holes");
    holes->set_tileset(tile_set);
  }

  auto Map::level(Level level) -> void
  {
    auto const & tiles = level.tiles;
    for_each(cbegin(tiles), cend(tiles), [x = 0, this](auto tile) mutable {
      switch (tile)
      {
      case tile_kind::ground:
        ground->set_cellv({static_cast<real_t>(x++), 14}, tile_set->find_tile_by_name(ground_tile_name));
        break;
      case tile_kind::hole:
        ground->set_cellv({static_cast<real_t>(x++), 14}, tile_set->find_tile_by_name(hole_tile_name));
        break;
      }
    });
  }

}  // namespace moon_buggy