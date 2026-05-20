extends Node2D

@onready var _sprite = $Sprite

func _ready() -> void:
	_sprite.animation = "stand"
	_sprite.frame = 0
	_sprite.stop()
