class_name MeteorSpawner
extends Path2D

## Whether meteors spawned by this spawner ignore gravity or not.
@export var ignore_gravity: bool = false

@export_group("Spawning")

## The duration of time, in seconds, that has to pass between consecutive spawn attempts.
@export_range(0.0, 120, 0.1, "suffix:s") var cooldown: float = 5.0:
	get:
		return cooldown
	set(value):
		$CooldownTimer.wait_time = value
		cooldown = value

## The likelihood of actually spawning a meteor once the cooldown has elapsed.
@export_range(0.0, 1.0) var probability: float = 0.5

const _meteor_scene = preload("res://components/meteor.tscn")

func _ready() -> void:
	assert(curve.point_count == 2)
	$SpawnPoint.progress_ratio = 0
	$CooldownTimer.wait_time = cooldown
	$CooldownTimer.start()

func _try_to_spawn() -> void:
	if(randf() > probability):
		return
	
	$SpawnPoint.progress_ratio = randf()
	var meteor = _meteor_scene.instantiate()
	add_child(meteor)

	meteor.position = $SpawnPoint.position
