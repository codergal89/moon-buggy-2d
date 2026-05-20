extends RigidBody2D

@onready var _sprite = $Sprite

func _ready() -> void:
	_sprite.animation = "fly"
	_sprite.frame = 0
	_sprite.play()

func _process(_delta: float) -> void:
	if not $VisibleOnScreenNotifier2D.is_on_screen():
		queue_free()
