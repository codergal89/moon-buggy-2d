extends Node2D

@export var autoscroll: bool = false:
	set(value):
		autoscroll = value
		$Space.autoscroll.x = int(value) * 5
		$Stars.autoscroll.x = int(value) * 7

@export_enum("Blue", "Purple") var theme: int = 0:
	set(value):
		theme = value
		apply_theme(theme)

func _ready() -> void:
	apply_theme(theme)

func apply_theme(id: int) -> void:
		match id:
			0:
				$Space/Blue.visible = true
				$Stars/Blue.visible = true
			1:
				$Space/Blue.visible = false
				$Stars/Blue.visible = false
		$Space/Purple.visible = not $Space/Blue.visible
		$Stars/Purple.visible = not $Stars/Blue.visible
	
