extends Node

func rotate_around_axis(current_transform: Transform3D, point: Vector3, axis: Vector3, angle_deg: float) -> Transform3D:
	var angle_rad = deg_to_rad(angle_deg)
	var axis_norm = axis.normalized()
	
	# Trasladar al origen
	var T1 = Transform3D(Basis(), -point)
	
	# Rotación alrededor del eje
	var R = Transform3D(Basis().rotated(axis_norm, angle_rad), Vector3.ZERO)
	
	# Volver al punto original
	var T2 = Transform3D(Basis(), point)
	
	# Componer: T2 * R * T1 * Transform_actual
	return T2 * R * T1 * current_transform

func translate_along_vector(current_transform: Transform3D, direction: Vector3, distance: float) -> Transform3D:
	var dir = direction.normalized() * distance
	var new_origin = current_transform.origin + current_transform.basis * dir
	return Transform3D(current_transform.basis, new_origin)
