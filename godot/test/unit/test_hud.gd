extends "res://addons/gut/test.gd"

const HUD = preload("res://scripts/HUD.gdns")
const Scene = preload("res://scenes/HUD.tscn")
const LevelNumberPath = "RowContainer/TopRow/LevelNumber"

var instance: HUD

func before_each():
	instance = add_child_autofree(Scene.instance())

func test_can_instantiate_hud():
	assert_not_null(instance)

func level_number() -> Label:
	return instance.get_node(LevelNumberPath)

func test_hud_has_a_child_called_level_number_of_type_label():
	assert_true(instance.get_node_or_null(LevelNumberPath) is Label)

func test_level_number_is_empty_by_default():
	assert_true(level_number().text.empty())

func test_hud_has_set_level_number_function():
	assert_has_method(instance, "set_level_number")

func test_level_number_label_text_can_be_changed_using_set_level_number():
	var old_text: String = level_number().text
	instance.set_level_number(42)
	assert_ne(level_number().text, old_text)

func test_level_number_label_shows_number_set_via_set_level_number():
	instance.set_level_number(21)
	assert_eq(level_number().text, "21")
