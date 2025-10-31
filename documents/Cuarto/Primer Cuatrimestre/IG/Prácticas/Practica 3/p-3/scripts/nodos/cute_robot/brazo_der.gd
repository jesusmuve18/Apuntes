extends Node3D

@export var rotation_speed_deg := 100
@export var activar := "mover_brazos"

var angulo_max := deg_to_rad(90.0)
var activa := true
var sentido := -1.0   # 1 = positivo, -1 = negativo

func _process(delta):
	if Input.is_action_just_pressed(activar):
		activa = !activa
	
	if activa:	
		if(sentido==1 && rotation.x + deg_to_rad(rotation_speed_deg * delta) >= angulo_max):
			sentido = -1	
		if(sentido==-1 && rotation.x - deg_to_rad(rotation_speed_deg * delta) <= -angulo_max):
			sentido = 1
			
		rotation.x += sentido*deg_to_rad(rotation_speed_deg * delta)
			
