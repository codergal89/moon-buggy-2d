extends "res://addons/gut/test.gd"


class TestLevelGeneratorParameters:
	extends "res://addons/gut/test.gd"

	const Generator = preload("res://scripts/LevelGenerator.gdns")
	var instance: Generator

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

class TestLevelGeneratorLoadConfiguration:
	extends "res://addons/gut/test.gd"

	const Generator = preload("res://scripts/LevelGenerator.gdns")
	var instance: Generator

	func before_each():
		instance = Generator.new()

	func after_each():
		instance.free()

	func test_loading_from_a_non_existent_file_yields_zero_levels():
		assert_eq(instance.load("res://THIS_FILE_MUST_NOT_EXIST"), 0)

	func test_loading_invalid_json_file_yields_zero_levels():
		assert_eq(instance.load("res://test/resources/invalid_levels"), 0)

	func test_loading_a_json_file_with_an_empty_list_yields_zero_levels():
		assert_eq(instance.load("res://test/resources/empty_levels"), 0)

	func test_loading_a_json_file_with_a_single_entry_yields_one_level():
		assert_eq(instance.load("res://test/resources/single_level"), 1)

	func test_loading_a_json_file_with_a_three_entries_yields_three_levels():
		assert_eq(instance.load("res://test/resources/three_levels"), 3)

	func test_loading_a_json_file_mixed_with_invalid_entry_yields_two_levels():
		assert_eq(instance.load("res://test/resources/levels_with_invalid"), 2)

class TestLevelGeneratorGenerateNext:
	extends "res://addons/gut/test.gd"

	const Generator = preload("res://scripts/LevelGenerator.gdns")
	var instance: Generator

	func before_each():
		instance = Generator.new()
		instance.load("res://test/resources/three_levels")

	func after_each():
		instance.free()

	func test_can_generate_next_level_with_multiple_loaded_descriptors():
		assert_not_null(autofree(instance.generate_next()))

	func test_can_generate_next_as_much_as_the_number_of_loaded_descriptors():
		for _i in range(instance.get_remaining_level_count()):
			assert_not_null(autofree(instance.generate_next()))

	func test_generating_another_level_after_the_last_available_one_yields_null():
		for _i in range(instance.get_remaining_level_count()):
			assert_not_null(autofree(instance.generate_next()))
		assert_null(instance.generate_next())

class TestLevelGeneratorPropertiesAfterLoad:
	extends "res://addons/gut/test.gd"

	const Generator = preload("res://scripts/LevelGenerator.gdns")
	var instance: Generator

	func before_each():
		instance = Generator.new()

	func after_each():
		instance.free()

	func test_generator_has_three_remaining_levels_after_loading_three_levels():
		instance.load("res://test/resources/three_levels")
		assert_eq(instance.get_remaining_level_count(), 3)

	func test_generator_has_two_remaining_levels_after_loading_three_levels_and_generating_one():
		instance.load("res://test/resources/three_levels")
		autofree(instance.generate_next())
		assert_eq(instance.get_remaining_level_count(), 2)

	func test_generator_has_remaining_levels_after_loading_three_levels():
		instance.load("res://test/resources/three_levels")
		assert_true(instance.has_remaining_levels())

	func test_generator_has_remaining_levels_after_loading_three_levels_and_generating_one():
		instance.load("res://test/resources/three_levels")
		autofree(instance.generate_next())
		assert_true(instance.has_remaining_levels())

	func test_generator_has_no_remaining_levels_after_loading_three_levels_and_generating_all():
		instance.load("res://test/resources/three_levels")
		for _i in range(3):
			autofree(instance.generate_next())
		assert_false(instance.has_remaining_levels())
