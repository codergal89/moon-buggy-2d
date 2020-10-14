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

class TestScene:
	extends "res://addons/gut/test.gd"

	const Scene = preload("res://scenes/Map.tscn")
	const Level = preload("res://scripts/Level.gdns")
	const GroundTiles = preload("res://assets/tiles/ground_tiles.tres")

	var instance

	func before_each():
		instance = autofree(Scene.instance())
		add_child(instance)

	func after_each():
		remove_child(instance)

	func _get_ground():
		return instance.get_node("Ground")

	func test_can_instantiate_map_scene():
		assert_not_null(instance)

	func test_map_scene_uses_ground_tiles_as_default_tileset():
		assert_eq(instance["tile_set"], GroundTiles)

	func test_map_scene_has_a_node_called_Ground():
		assert_not_null(_get_ground())

	func test_map_scene_has_a_child_of_type_TileMap():
		var has_tile_map = false
		for child in instance.get_children():
			if child is TileMap:
				has_tile_map = true
		assert_true(has_tile_map)

	func test_map_scene_has_a_TileMap_child_called_ground():
		assert_true(_get_ground() is TileMap)

	func test_the_end_of_the_world_of_an_empty_map_is_at_zero():
		assert_eq(instance.get_world_end(), 0)

	func test_the_tile_map_for_an_empty_map_is_empty():
		assert_true(_get_ground().get_used_cells().empty())

	func test_setting_an_empty_level_on_a_128_wide_screen_causes_8_tiles_to_be_used():
		var level: Level = autofree(Level.new())
		instance.set_level(level, 128, 128);
		var ground: TileMap = _get_ground()
		assert_eq(ground.get_used_cells().size(), 8)

	func test_setting_an_empty_level_causes_bottom_row_of_tiles_to_be_set_as_ground():
		var level: Level = autofree(Level.new())
		instance.set_level(level, 128, 128);
		var ground: TileMap = _get_ground()
		for i in range(4):
			assert_eq(ground.get_cell(i, 3), 0)
