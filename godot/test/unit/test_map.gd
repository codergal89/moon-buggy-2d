extends "res://addons/gut/test.gd"


class TestScript:
	extends "res://addons/gut/test.gd"
	
	const Map = preload("res://scripts/game/Map.gdns")
	const GroundTiles = preload("res://assets/tiles/moon_tiles_set.tres")

	var instance: Map
	
	func before_each():
		instance = autofree(Map.new())

	func test_can_instantiate_map():
		assert_not_null(instance)

	func test_has_set_level_function():
		assert_has_method(instance, "set_level")

	func test_has_get_world_end_function():
		assert_has_method(instance, "get_world_end")

class TestSceneSetup:
	extends "res://addons/gut/test.gd"

	const Map = preload("res://scripts/game/Map.gdns")
	const Scene = preload("res://scenes/game/Map.tscn")
	const GroundTiles = preload("res://assets/tiles/moon_tiles_set.tres")

	var instance: Map

	func before_each():
		instance = add_child_autofree(Scene.instance())

	func tile_map() -> TileMap:
		return instance.get_node("Ground")

	func test_can_instantiate_map_scene():
		assert_not_null(instance)

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

	const Map = preload("res://scripts/game/Map.gdns")
	const Scene = preload("res://scenes/game/Map.tscn")
	const Level = preload("res://scripts/core/Level.gdns")
	const GroundTiles = preload("res://assets/tiles/moon_tiles_set.tres")

	const screen_width: int = 128
	const screen_height: int = 64
	const tile_size: int = 32
	const x_tiles_per_screen: int = screen_width / tile_size

	var instance: Map
	var tile_map: TileMap

	func before_each():
		instance = add_child_autofree(Scene.instance())
		tile_map = instance.get_node("Ground")

	func set_empty_level():
		instance.set_level(autofree(Level.new()), screen_width, screen_height)

	func test_setting_an_empty_level_places_seven_times_two_screen_widths_of_tiles():
		set_empty_level()
		var expected_number_of_ground_tiles: int = 2 * 7 * screen_width / tile_size
		var used_cells = tile_map.get_used_cells()
		assert_eq(used_cells.size(), expected_number_of_ground_tiles)

	func test_setting_an_empty_level_places_two_screen_widths_of_ground_surface_tiles():
		set_empty_level()
		var ground_tile: int = tile_map.tile_set.find_tile_by_name("ground_surface")
		var expected_cells: Array = []
		var cells: Array = []

		var slice_start: int = 0 * 2 * x_tiles_per_screen
		var slice_end: int = slice_start + 2 * x_tiles_per_screen - 1
		var used_cells: Array = tile_map.get_used_cells().slice(slice_start, slice_end)
		var used_cell_count: int = used_cells.size()

		for _i in range(used_cell_count):
			expected_cells.push_back(ground_tile)

		for position in used_cells:
			cells.push_back(tile_map.get_cellv(position))
		assert_eq(cells, expected_cells)

	func test_setting_an_empty_level_places_two_screen_widths_of_ground_layer1_border_tiles():
		set_empty_level()
		var ground_tile: int = tile_map.tile_set.find_tile_by_name("ground_layer1_border")
		var expected_cells: Array = []
		var cells: Array = []

		var slice_start: int = 1 * 2 * x_tiles_per_screen
		var slice_end: int = slice_start + 2 * x_tiles_per_screen - 1
		var used_cells: Array = tile_map.get_used_cells().slice(slice_start, slice_end)
		var used_cell_count: int = used_cells.size()

		for _i in range(used_cell_count):
			expected_cells.push_back(ground_tile)

		for position in used_cells:
			cells.push_back(tile_map.get_cellv(position))
		assert_eq(cells, expected_cells)

	func test_setting_an_empty_level_places_two_screen_widths_of_ground_layer1_tiles():
		set_empty_level()
		var ground_tile: int = tile_map.tile_set.find_tile_by_name("ground_layer1")
		var expected_cells: Array = []
		var cells: Array = []

		var slice_start: int = 2 * 2 * x_tiles_per_screen
		var slice_end: int = slice_start + 2 * x_tiles_per_screen - 1
		var used_cells: Array = tile_map.get_used_cells().slice(slice_start, slice_end)
		var used_cell_count: int = used_cells.size()

		for _i in range(used_cell_count):
			expected_cells.push_back(ground_tile)

		for position in used_cells:
			cells.push_back(tile_map.get_cellv(position))
		assert_eq(cells, expected_cells)
	
	func test_setting_an_empty_level_places_two_screen_widths_of_ground_layer2_border_tiles():
		set_empty_level()
		var ground_tile: int = tile_map.tile_set.find_tile_by_name("ground_layer2_border")
		var expected_cells: Array = []
		var cells: Array = []

		var slice_start: int = 3 * 2 * x_tiles_per_screen
		var slice_end: int = slice_start + 2 * x_tiles_per_screen - 1
		var used_cells: Array = tile_map.get_used_cells().slice(slice_start, slice_end)
		var used_cell_count: int = used_cells.size()

		for _i in range(used_cell_count):
			expected_cells.push_back(ground_tile)

		for position in used_cells:
			cells.push_back(tile_map.get_cellv(position))
		assert_eq(cells, expected_cells)

	func test_setting_an_empty_level_places_two_screen_widths_of_ground_layer2_tiles():
		set_empty_level()
		var ground_tile: int = tile_map.tile_set.find_tile_by_name("ground_layer2")
		var expected_cells: Array = []
		var cells: Array = []

		var slice_start: int = 4 * 2 * x_tiles_per_screen
		var slice_end: int = slice_start + 2 * x_tiles_per_screen - 1
		var used_cells: Array = tile_map.get_used_cells().slice(slice_start, slice_end)
		var used_cell_count: int = used_cells.size()

		for _i in range(used_cell_count):
			expected_cells.push_back(ground_tile)

		for position in used_cells:
			cells.push_back(tile_map.get_cellv(position))
		assert_eq(cells, expected_cells)
	
	func test_setting_an_empty_level_places_two_screen_widths_of_ground_layer3_border_tiles():
		set_empty_level()
		var ground_tile: int = tile_map.tile_set.find_tile_by_name("ground_layer3_border")
		var expected_cells: Array = []
		var cells: Array = []

		var slice_start: int = 5 * 2 * x_tiles_per_screen
		var slice_end: int = slice_start + 2 * x_tiles_per_screen - 1
		var used_cells: Array = tile_map.get_used_cells().slice(slice_start, slice_end)
		var used_cell_count: int = used_cells.size()

		for _i in range(used_cell_count):
			expected_cells.push_back(ground_tile)

		for position in used_cells:
			cells.push_back(tile_map.get_cellv(position))
		assert_eq(cells, expected_cells)

	func test_setting_an_empty_level_places_two_screen_widths_of_ground_layer3_tiles():
		set_empty_level()
		var ground_tile: int = tile_map.tile_set.find_tile_by_name("ground_layer3")
		var expected_cells: Array = []
		var cells: Array = []

		var slice_start: int = 6 * 2 * x_tiles_per_screen
		var slice_end: int = slice_start + 2 * x_tiles_per_screen - 1
		var used_cells: Array = tile_map.get_used_cells().slice(slice_start, slice_end)
		var used_cell_count: int = used_cells.size()

		for _i in range(used_cell_count):
			expected_cells.push_back(ground_tile)

		for position in used_cells:
			cells.push_back(tile_map.get_cellv(position))
		assert_eq(cells, expected_cells)

	func test_setting_no_level_causes_the_world_end_to_be_at_zero():
		assert_eq(instance.get_world_end(), 0)

	func test_setting_an_empty_level_causes_the_world_end_to_be_at_negative_screen_width():
		set_empty_level()
		assert_eq(instance.get_world_end(), -screen_width)
