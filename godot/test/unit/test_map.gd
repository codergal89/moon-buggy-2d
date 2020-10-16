extends "res://addons/gut/test.gd"


class TestScript:
	extends "res://addons/gut/test.gd"
	
	const Map = preload("res://scripts/Map.gdns")
	const GroundTiles = preload("res://assets/tiles/ground_tiles.tres")

	var instance: Map
	
	func before_each():
		instance = autofree(Map.new())

	func test_can_instantiate_map():
		assert_not_null(instance)

	func test_has_tile_set_property():
		assert_exports(instance, "tile_set", TYPE_OBJECT)

	func test_has_no_tileset_by_default():
		assert_null(instance["tile_set"])

	func test_can_assign_tileset():
		instance["tile_set"] = GroundTiles
		assert_eq(instance["tile_set"], GroundTiles)

	func test_has_set_level_function():
		assert_has_method(instance, "set_level")

	func test_has_get_world_end_function():
		assert_has_method(instance, "get_world_end")

class TestSceneSetup:
	extends "res://addons/gut/test.gd"

	const Map = preload("res://scripts/Map.gdns")
	const Scene = preload("res://scenes/Map.tscn")
	const GroundTiles = preload("res://assets/tiles/ground_tiles.tres")

	var instance: Map

	func before_each():
		instance = add_child_autofree(Scene.instance())

	func tile_map() -> TileMap:
		return instance.get_node("Ground")

	func test_can_instantiate_map_scene():
		assert_not_null(instance)

	func test_map_scene_uses_ground_tiles_as_default_tileset():
		assert_eq(instance["tile_set"], GroundTiles)

	func test_map_scene_has_a_node_called_Ground():
		assert_not_null(tile_map())

	func test_map_scene_has_a_child_of_type_TileMap():
		var has_tile_map = false
		for child in instance.get_children():
			if child is TileMap:
				has_tile_map = true
		assert_true(has_tile_map)

	func test_map_scene_has_a_TileMap_child_called_ground():
		assert_true(tile_map() is TileMap)

	func test_the_end_of_the_world_of_an_empty_map_is_at_zero():
		assert_eq(instance.get_world_end(), 0)

	func test_the_tile_map_for_an_empty_map_is_empty():
		assert_true(tile_map().get_used_cells().empty())

class TestSceneLevel:
	extends "res://addons/gut/test.gd"

	const Map = preload("res://scripts/Map.gdns")
	const Scene = preload("res://scenes/Map.tscn")
	const Level = preload("res://scripts/Level.gdns")
	const GroundTiles = preload("res://assets/tiles/ground_tiles.tres")

	const screen_width: int = 128
	const screen_height: int = 64
	const tile_size: int = 32

	var instance: Map
	var tile_map: TileMap

	func before_each():
		instance = add_child_autofree(Scene.instance())
		tile_map = instance.get_node("Ground")

	func set_empty_level():
		instance.set_level(autofree(Level.new()), screen_width, screen_height)

	func test_setting_an_empty_level_places_two_screen_widths_of_tiles():
		set_empty_level()
		var expected_number_of_ground_tiles: int = 2 * screen_width / tile_size
		var used_cells = tile_map.get_used_cells()
		assert_eq(used_cells.size(), expected_number_of_ground_tiles)

	func test_setting_an_empty_level_places_only_ground_tiles():
		set_empty_level()
		var expected_tile: int = tile_map.tile_set.find_tile_by_name("ground")
		var expected_cells: Array = []
		var cells: Array = []
		for position in tile_map.get_used_cells():
			expected_cells.push_back(expected_tile)
			cells.push_back(tile_map.get_cellv(position))
		assert_eq(cells, expected_cells)

	func test_setting_no_level_causes_the_world_end_to_be_at_zero():
		assert_eq(instance.get_world_end(), 0)

	func test_setting_an_empty_level_causes_the_world_end_to_be_at_negative_screen_width():
		set_empty_level()
		assert_eq(instance.get_world_end(), -screen_width)
