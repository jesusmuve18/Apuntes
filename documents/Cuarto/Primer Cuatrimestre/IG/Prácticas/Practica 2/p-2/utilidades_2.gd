extends Node

func generate_revolution_mesh(
	profile: PackedVector2Array, 
	n_copies: int,
	vertices: PackedVector3Array, 
	triangulos: PackedInt32Array,) -> void :
	
	
	
	## Vertices para cerrar la figura
	if (profile[0].x!=0.0):
		var nuevo := PackedVector2Array()
		nuevo.append(Vector2(0,profile[0].y))  # el nuevo elemento al principio
		for p in profile:
			nuevo.append(p)
		profile = nuevo
	
	if (profile[profile.size()-1].x!=0.0):
		profile.append(Vector2(0,profile[profile.size()-1].y))
		
	print(profile)
		
	## Genera vértices rotando el profile alrededor del eje y
	for i in range(n_copies): 
		var angulo = (i * 2 * PI) / n_copies
		var rot = Transform3D(Basis(Vector3.UP, angulo), Vector3.ZERO)
		
		for v in profile:
			var v3 = Vector3(v.x, v.y, 0)  # convertir de 2D a 3D
			vertices.append(rot * v3)       # aplica la rotación
	
	
	var n_points = profile.size()
	
	# Generar triángulos
	for i in range(n_copies):
		var next_i = (i + 1) % n_copies  # copia siguiente (cierra el círculo)
		
		for j in range(n_points - 1):  # conectamos puntos consecutivos del perfil
			# índice de los 4 vértices de este cuadrado
			var a = i * n_points + j
			var b = next_i * n_points + j
			var c = next_i * n_points + (j + 1)
			var d = i * n_points + (j + 1)
			
			# dos triángulos por cuadrado
			triangulos.append(a)
			triangulos.append(c)
			triangulos.append(b)
			
			triangulos.append(a)
			triangulos.append(d)
			triangulos.append(c)
	
