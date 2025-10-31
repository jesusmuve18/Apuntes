extends Node3D

@export var scalation_speed := 0.75
@export var activar := "escalar_boca"

var scalation_max := 3
var scalation_min := 0.5
var activa := true
var sentido := 1.0   # 1 = positivo, -1 = negativo

func _process(delta):
	if Input.is_action_just_pressed(activar):
		activa = !activa
	
	if activa:
		if(sentido==1 && (scale.x + (scalation_speed * delta)) >= scalation_max):
			sentido = -1
		if(sentido==-1 && (scale.x - (scalation_speed * delta)) <= scalation_min):
			sentido = 1
			
		scale.x += sentido * scalation_speed * delta
			
			
