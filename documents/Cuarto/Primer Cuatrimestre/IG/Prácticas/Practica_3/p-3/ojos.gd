extends Node3D

@export var speed := 0.2
@export var activar := "mover_ojos"

var translation := 0.0
var translation_max := 0.15
var activa := true
var sentido := 1.0   # 1 = positivo, -1 = negativo

var trans_direction := Vector3(1, 0, 0)

func _process(delta):
	if Input.is_action_just_pressed(activar):
		activa = !activa
	
	if activa:
		var distance = speed * delta * sentido
			
		if abs(translation + distance) > translation_max:
			var restante = sentido * (translation_max - abs(translation))
			transform = Utilidades.translate_along_vector(transform, trans_direction, restante)
			translation += restante
			sentido *= -1
		else:
			transform = Utilidades.translate_along_vector(transform, trans_direction, distance)
			translation += distance
