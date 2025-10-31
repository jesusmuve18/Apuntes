extends Node3D

@export var translation_speed := 0.2
@export var activar := "mover_ojos"

var translation_max := 0.15
var activa := true
var sentido := 1.0   # 1 = positivo, -1 = negativo

func _process(delta):
	if Input.is_action_just_pressed(activar):
		activa = !activa
	
	if activa:
		if(sentido==1 && (position.x + (translation_speed * delta)) >= translation_max):
			sentido = -1
		if(sentido==-1 && (position.x - (translation_speed * delta)) <= -translation_max):
			sentido = 1
			
		position.x += sentido * translation_speed * delta
			
			
