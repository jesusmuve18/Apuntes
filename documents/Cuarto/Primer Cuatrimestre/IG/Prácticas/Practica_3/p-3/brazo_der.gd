extends Node3D

@export var rotation_speed_deg := 100
@export var activar := "mover_brazos"

var angulo_max := 90.0
var activa := true
var angulo := 0.0
var sentido := -1.0   # 1 = positivo, -1 = negativo

var axis_point := Vector3(0, 0.911,0)
var axis_vector := Vector3(1, 0, 0)

func _process(delta):
	if Input.is_action_just_pressed(activar):
		activa = !activa
	
	if activa:
		var delta_angle = rotation_speed_deg * delta * sentido
		
		# Si el siguiente paso sobrepasa el límite
		if abs(angulo + delta_angle) > angulo_max:
			# Calculamos cuánto falta para el límite
			var restante = angulo_max - abs(angulo)
			
			# Aplicamos solo la parte que falta
			transform = Utilidades.rotate_around_axis(transform, axis_point, axis_vector, restante * sentido)
			
			# Llegamos justo al límite
			angulo += restante * sentido
			
			# Invertimos el sentido
			sentido *= -1
			
		else:
			# Movimiento normal dentro del rango
			transform = Utilidades.rotate_around_axis(transform, axis_point, axis_vector, delta_angle)
			angulo += delta_angle
