extends GutTest

var scene: PackedScene = preload("res://scenes/Menus/MainMenu.tscn")
var instance: MainMenu

func before_each():
	instance = add_child_autofree(scene.instantiate())
	watch_signals(instance)

################################################################################
## Instantiation tests
################################################################################

func test_can_instantiate():
	assert_not_null(instance)

################################################################################
## Signal existence tests
################################################################################

func test_has_signal_credits_pressed():
	assert_has_signal(instance, "credits_pressed")

func test_has_signal_quit_pressed():
	assert_has_signal(instance, "quit_pressed")

func test_has_signal_start_pressed():
	assert_has_signal(instance, "start_pressed")

################################################################################
## Signal emission tests
################################################################################

func test_clicking_the_Credits_button_emits_credits_pressed():
	instance.get_node("%Credits").pressed.emit()
	assert_signal_emitted(instance, "credits_pressed")

func test_clicking_the_Quit_button_emits_quit_pressed():
	instance.get_node("%Quit").pressed.emit()
	assert_signal_emitted(instance, "quit_pressed")

func test_clicking_the_Start_button_emits_start_pressed():
	instance.get_node("%Start").pressed.emit()
	assert_signal_emitted(instance, "start_pressed")
