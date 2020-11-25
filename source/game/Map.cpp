#include "game/Map.hpp"

#include "core/Level.hpp"
#include "support/ArrayIterator.hpp"

#include <Defs.hpp>
#include <Node.hpp>
#include <RandomNumberGenerator.hpp>
#include <Ref.hpp>
#include <TileMap.hpp>
#include <TileSet.hpp>
#include <Vector2.hpp>

#include <algorithm>
#include <array>
#include <cstdint>
#include <iterator>
#include <utility>
#include <vector>

namespace moon_buggy
{

  auto constexpr level_tile_names = std::array{
      std::pair{Level::Tile::ground_layer1_border, "ground_layer1_border"},
      std::pair{Level::Tile::ground_surface, "ground_surface"},
      std::pair{Level::Tile::hole, "hole"},
      std::pair{Level::Tile::left_shoulder, "left_shoulder"},
      std::pair{Level::Tile::long_stone1, "long_stone1"},
      std::pair{Level::Tile::right_shoulder, "right_shoulder"},
      std::pair{Level::Tile::small_stone1, "small_stone1"},
  };

  auto constexpr stone_tiles = std::array{
      std::pair{
          "Small",
          std::array{
              Level::Tile::small_stone1,
              Level::Tile::long_stone1,
          },
      },
  };

  auto Map::_register_methods() -> void
  {
    godot::register_method("_ready", &Map::_ready);
    godot::register_method("set_level", &Map::set_level);
    godot::register_method("get_world_end", &Map::get_world_end);
    godot::register_method("get_surface_level", &Map::get_surface_level);
  }

  auto Map::_init() -> void
  {
  }

  auto Map::_ready() -> void
  {
    ground = get_typed_node<godot::TileMap>("Ground");
    auto tile_set = ground->get_tileset();
    CRASH_COND(!tile_set.is_valid());

    transform(cbegin(level_tile_names), cend(level_tile_names), inserter(map_tile_ids, begin(map_tile_ids)), [&](auto const & mapping) {
      return std::pair{mapping.first, tile_set->find_tile_by_name(mapping.second)};
    });

    auto stone_layers_node = get_typed_node<godot::Node>("StoneLayers");
    stone_layers = stone_layers_node->get_children();
    CRASH_COND(std::any_of(cbegin(stone_layers), cend(stone_layers), [](auto layer) { return !cast_to<godot::TileMap>(layer); }));
  }

  auto Map::set_level(Level * level, std::uint64_t width, std::uint64_t height) -> void
  {
    ground->clear();

    auto cell_size = ground->get_cell_size();
    auto x_tiles_per_screen = static_cast<std::int64_t>((width + cell_size.x - 1) / cell_size.x);
    auto y_tiles_per_screen = static_cast<std::int64_t>((height + cell_size.y - 1) / cell_size.y);

    auto last_tile = -(level->surface_tiles.size() + x_tiles_per_screen);
    world_end = static_cast<std::int64_t>(ground->map_to_world({static_cast<real_t>(last_tile), 0.f}).x);
    auto bottom = y_tiles_per_screen - 2;
    auto right = x_tiles_per_screen - 1;

    generate_surface(*level, x_tiles_per_screen, bottom, right);
    generate_stones(right, last_tile, bottom);

    for (auto x{0}; x < 2 * x_tiles_per_screen + level->surface_tiles.size(); ++x)
    {
      ground->set_cell(right - x, bottom + 1, map_tile_ids[Level::Tile::ground_layer1_border]);
    }
  }

  auto Map::get_world_end() -> std::int64_t
  {
    return world_end;
  }

  auto Map::get_surface_level() const noexcept -> std::int64_t
  {
    return surface_level;
  }

  auto Map::generate_surface(Level const & level, std::int64_t x_tiles_per_screen, std::int64_t bottom, std::int64_t right) -> void
  {
    surface_level = static_cast<std::int64_t>(ground->map_to_world({0.f, static_cast<real_t>(bottom)}).y);

    auto surface_tiles = std::vector<std::int64_t>{};
    surface_tiles.reserve(level.surface_tiles.size() + 2 * x_tiles_per_screen);

    generate_n(back_inserter(surface_tiles), x_tiles_per_screen, [&] { return map_tile_ids.at(Level::Tile::ground_surface); });

    transform(cbegin(level.surface_tiles), cend(level.surface_tiles), back_inserter(surface_tiles), [&](auto tile) {
      return map_tile_ids.at(static_cast<Level::Tile>(static_cast<int>(tile)));
    });

    generate_n(back_inserter(surface_tiles), x_tiles_per_screen, [&] { return map_tile_ids.at(Level::Tile::ground_surface); });

    for_each(cbegin(surface_tiles), cend(surface_tiles), [x = 0, this, bottom, right](auto tile_id) mutable {
      ground->set_cell(right - x++, bottom, tile_id);
    });
  }

  auto Map::generate_stones(std::int64_t right, std::int64_t last_tile, std::int64_t bottom) -> void
  {
    auto rng = godot::Ref<godot::RandomNumberGenerator>{};
    rng.instance();
    rng->randomize();

    std::for_each(cbegin(stone_layers), cend(stone_layers), [&](auto entry) {
      auto layer = cast_to<godot::TileMap>(entry);
      auto name = layer->get_name();
      auto stone_kind_mapping =
          std::find_if(cbegin(stone_tiles), cend(stone_tiles), [&](auto entry) { return godot::String{entry.first} == name; });
      if (stone_kind_mapping == cend(stone_tiles))
      {
        WARN_PRINT(godot::String{"Unknown stone layer: {0}"}.format(godot::Array::make(name)));
        return;
      }

      auto const & candidate_tiles = stone_kind_mapping->second;

      for (auto i{0}; i < 12; ++i)
      {
        auto pos = rng->randi_range(right, last_tile);
        auto tile = candidate_tiles[rng->randi_range(0, candidate_tiles.size() - 1)];
        layer->set_cell(pos, bottom, map_tile_ids.at(tile));
      }
    });
  }

}  // namespace moon_buggy