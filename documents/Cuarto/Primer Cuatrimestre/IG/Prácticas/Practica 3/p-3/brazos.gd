extends Node3D

@export var rotation_speed_deg := 10.0
@export var activar := "rotar_brazos"
@export var angulo_max := 90.0

var activa := true
var angulo := 0.0
var sentido := -1.0   # 1 = positivo, -1 = negativo

var axis_point := Vector3(0, -0.185, -0.072)
var axis_vector := Vector3(1, 0, 0)

func _process(delta):
	if Input.is_action_just_pressed(activar):
		activa = !activa
	
	if not activa:
		return
	
	var delta_angle = rotation_speed_deg * delta * sentido
	
	# Si el siguiente paso sobrepasa el límite
	if abs(angulo + delta_angle) > angulo_max:
		# Calculamos cuánto falta para el límite
		var restante = angulo_max - abs(angulo)
		
		# Aplicamos solo la parte que falta
		global_transform = Utilidades.rotate_around_axis(global_transform, axis_point, axis_vector, restante * sentido)
		
		# Llegamos justo al límite
		angulo += restante * sentido
		
		# Invertimos el sentido
		sentido *= -1
		
	else:
		# Movimiento normal dentro del rango
		global_transform = Utilidades.rotate_around_axis(global_transform, axis_point, axis_vector, delta_angle)
		angulo += delta_angle
