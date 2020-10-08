extends "res://addons/gut/test.gd"


class TestLevelGeneratorProperties:
	extends "res://addons/gut/test.gd"
	
	var Generator = load("res://bin/scripts_LevelGenerator.gdns")
	var instance: Node
	
	func before_each():
		instance = Generator.new()
	
	func after_each():
		instance.free()
	
	func test_can_instanciate_level_generator():
		assert_not_null(instance)

	func test_has_minimum_hole_length():
		assert_exports(instance, "minimum_hole_length", TYPE_INT)

	func test_minimum_hole_length_defaults_to_three():
		assert_eq(instance["minimum_hole_length"], 3)

	func test_has_maximum_hole_length():
		assert_exports(instance, "maximum_hole_length", TYPE_INT)

	func test_maximum_hole_length_defaults_to_ten():
		assert_eq(instance["maximum_hole_length"], 10)

	func test_has_minimum_platform_length():
		assert_exports(instance, "minimum_platform_length", TYPE_INT)

	func test_minimum_platform_length_is_five():
		assert_eq(instance["minimum_platform_length"], 5)

	func test_has_maximum_platform_length():
		assert_exports(instance, "maximum_platform_length", TYPE_INT)

	func test_maximum_platform_length_is_fifteen():
		assert_eq(instance["maximum_platform_length"], 15)

	func test_has_minimum_segments():
		assert_exports(instance, "minimum_segments", TYPE_INT)

	func test_minimum_segments_defaults_to_four():
		assert_eq(instance["minimum_segments"], 4)

	func test_has_maximum_segments():
		assert_exports(instance, "maximum_segments", TYPE_INT)
	
	func test_maximum_segments_default_to_thirtytwo():
		assert_eq(instance["maximum_segments"], 32)
