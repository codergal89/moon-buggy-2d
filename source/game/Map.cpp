#include "game/Map.hpp"

#include "core/Level.hpp"
#include "support/ArrayIterator.hpp"

#include <Array.hpp>
#include <Defs.hpp>
#include <GodotGlobal.hpp>
#include <Image.hpp>
#include <ImageTexture.hpp>
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

  auto Map::_register_methods() -> void
  {
    godot::register_method("_ready", &Map::_ready);
    godot::register_method("set_level", &Map::set_level);
    godot::register_method("get_world_end", &Map::get_world_end);

    godot::register_property("ground_textures",
                             &Map::ground_textures,
                             godot::Array{},
                             GODOT_METHOD_RPC_MODE_DISABLED,
                             GODOT_PROPERTY_USAGE_DEFAULT,
                             GODOT_PROPERTY_HINT_RESOURCE_TYPE);
  }

  auto Map::_init() -> void
  {
  }

  auto Map::_ready() -> void
  {
    ground = get_typed_node<godot::TileMap>("Ground");
    auto tile_set = ground->get_tileset();
    CRASH_COND(!tile_set.is_valid());

    transform(cbegin(level_tile_names), cend(level_tile_names), inserter(m_tile_ids, begin(m_tile_ids)), [&](auto const & mapping) {
      return std::pair{mapping.first, tile_set->find_tile_by_name(mapping.second)};
    });

    std::for_each(cbegin(ground_textures), cend(ground_textures), [](auto texture) {
      CRASH_COND(!static_cast<godot::Ref<godot::Image>>(texture).is_valid());
    });

    auto tile_texture = tile_set->tile_get_texture(m_tile_ids[Level::Tile::ground_surface]);
    ground_texture = static_cast<godot::Ref<godot::ImageTexture>>(tile_texture);
    CRASH_COND(!ground_texture.is_valid());
  }

  auto Map::set_level(Level * level, std::uint64_t width, std::uint64_t height) -> void
  {
    ground->clear();

    auto cell_size = ground->get_cell_size();
    auto x_tiles_per_screen = static_cast<std::int64_t>((width + cell_size.x - 1) / cell_size.x);
    auto y_tiles_per_screen = static_cast<std::int64_t>((height + cell_size.y - 1) / cell_size.y);

    end_tile = -(level->tiles.size() + x_tiles_per_screen);
    auto bottom = y_tiles_per_screen - 2;
    auto right = x_tiles_per_screen - 1;

    generate_surface(*level, x_tiles_per_screen, bottom, right);

    for (auto x{0}; x < 2 * x_tiles_per_screen + level->tiles.size(); ++x)
    {
      ground->set_cell(right - x, bottom + 1, m_tile_ids[Level::Tile::ground_layer1_border]);
    }

    auto rng = godot::Ref<godot::RandomNumberGenerator>{};
    rng.instance();

    auto stones_layer1 = get_typed_node<godot::TileMap>("StonesLayer1");

    for (auto i{0}; i < 25; ++i)
    {
      auto pos = rng->randi_range(right, end_tile);
      stones_layer1->set_cell(pos, bottom + 1, m_tile_ids[Level::Tile::small_stone1]);
    }
  }

  auto Map::get_world_end() -> std::int64_t
  {
    return static_cast<std::int64_t>(ground->map_to_world({static_cast<real_t>(end_tile), 0.f}).x);
  }

  auto Map::generate_surface(Level const & level, std::int64_t x_tiles_per_screen, std::int64_t bottom, std::int64_t right) const -> void
  {
    auto surface_tiles = std::vector<std::int64_t>{};
    surface_tiles.reserve(level.tiles.size() + 2 * x_tiles_per_screen);

    generate_n(back_inserter(surface_tiles), x_tiles_per_screen, [&] { return m_tile_ids.at(Level::Tile::ground_surface); });

    transform(cbegin(level.tiles), cend(level.tiles), back_inserter(surface_tiles), [&](auto tile) {
      return m_tile_ids.at(static_cast<Level::Tile>(static_cast<int>(tile)));
    });

    generate_n(back_inserter(surface_tiles), x_tiles_per_screen, [&] { return m_tile_ids.at(Level::Tile::ground_surface); });

    for_each(cbegin(surface_tiles), cend(surface_tiles), [x = 0, this, bottom, right](auto tile_id) mutable {
      ground->set_cell(right - x++, bottom, tile_id);
    });
  }

}  // namespace moon_buggy