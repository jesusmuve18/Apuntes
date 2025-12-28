## Nombre: Jesús, Apellidos: Muñoz Velasco, Titulación: GIM
## email: jesusmuve18@correo.ugr.es, DNI o pasaporte: 76668409W

extends Node

# Genera una malla por revolucion a partir de un perfil 2D
func generate_revolution_mesh(
	profile: PackedVector2Array, 
	n_copies: int,
	vertices: PackedVector3Array, 
	triangulos: PackedInt32Array,) -> void :
				
				
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
 
## -----------------------------------------------------------------------------
## Función que calcula las normales promedio de los vértices de una malla, 
## a partir de la tabla de posiciones de vértices y la tabla de triángulos
func calcNormales( verts : PackedVector3Array, 
				   tris  : PackedInt32Array ) -> PackedVector3Array :

	## Paso 1: comprobar precondiciones
	assert( verts.size() >= 3 , "CalcNormales: la malla debe tener al menos 3 vértices" )
	assert( tris.size() % 3 == 0 , "CalcNormales: el número de enteros en 'tris' debe ser múltiplo de 3")
	
	var nv : int = verts.size()  ## número de vértices
	var nt : int = tris.size()/3 ## número de triángulos
	
	# Paso 2: inicializa normales a cero
	var normales := PackedVector3Array([]) 
	for i in range(nv):
		normales.append( Vector3.ZERO )
	
	# Paso 3: sumar en cada vértice las normales de sus tris. adyacentes
	for it in range(nt) :
		var t := Vector3i( tris[3*it+0], tris[3*it+1], tris[3*it+2] )
		var a := verts[t[0]] ; 
		var b := verts[t[1]] ; 
		var c := verts[t[2]] ; 
		var normalv := (c-a).cross(b-a).normalized()
		for iv in range(3) :
			normales[t[iv]] += normalv
	
	# Paso 4: normalizar normales
	for iv in nv:
		normales[iv] = normales[iv].normalized()
	
	# Hecho
	return normales

static func calcUV(vertices: PackedVector3Array) -> PackedVector2Array:
	var uvs := PackedVector2Array()
	var max_u = 1.0
	var max_v = 1.0
	
	# Calcular min y max de Y para normalizar v
	var min_y = vertices[0].y
	var max_y = vertices[0].y
	for vert in vertices:
		if vert.y < min_y:
			min_y = vert.y
		if vert.y > max_y:
			max_y = vert.y
	
	for v in vertices:
		# 1. Calcular el valor del parámetro u
		var phi = atan2(v.z, v.x)
		var u = max_u*((phi / (2*PI)+0.5))
		# 2. Calcular el valor del parámetro v
		#Inserta tu código aquí
		#Puedes calcularlo en función del desplazaniento en el perfil
		# o de forma aproximada en función de y
		var v_coord =  (v.y - min_y) / (max_y - min_y) * max_v
		var uv_coords = Vector2(1-u,1-v_coord)
		uvs.append(uv_coords)
	return uvs
