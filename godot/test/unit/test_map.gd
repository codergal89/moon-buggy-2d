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

	func test_can_instantiate_map_scene():
		assert_not_null(instance)

	func test_map_scene_uses_ground_tiles_as_default_tileset():
		assert_eq(instance["tile_set"], GroundTiles)

	func test_the_end_of_the_world_of_an_empty_map_is_at_zero():
		assert_eq(instance.get_world_end(), 0)

	func test_the_tile_map_for_an_empty_map_is_empty():
		assert_true(instance.get_node("Ground").get_used_cells().empty())
