#include "Map.hpp"

#include <Defs.hpp>
#include <GodotGlobal.hpp>
#include <Ref.hpp>
#include <TileMap.hpp>
#include <TileSet.hpp>
#include <Vector2.hpp>

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

    for (auto x{0}; x < 20; ++x)
    {
      ground->set_cellv({static_cast<real_t>(x), 14}, tile_set->find_tile_by_name(ground_tile_name));
    }
  }

}  // namespace moon_buggy