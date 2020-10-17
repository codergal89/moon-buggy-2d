extends "res://addons/gut/test.gd"

const Scene = preload("res://scenes/gui/MainMenu.tscn")
const MainMenu = preload("res://scripts/gui/MainMenu.gdns")

func test_can_instantiate_main_menu():
	assert_not_null(autofree(MainMenu.new()))

func test_main_menu_has_start_game_signal():
	assert_has_signal(autofree(MainMenu.new()), "start_game")

func test_invoking_start_button_clicked_emits_start_game():
	var instance: MainMenu = autofree(MainMenu.new())
	watch_signals(instance)
	instance.start_button_clicked()
	assert_signal_emitted(instance, "start_game")

func test_button_pressed_signal_is_connected_to_start_button_clicked():
	var scene: MainMenu = add_child_autofree(Scene.instance())
	var start_button: Button = scene.get_node("RowContainer/StartButton")
	assert_connected(start_button, scene, "pressed", "start_button_clicked")
