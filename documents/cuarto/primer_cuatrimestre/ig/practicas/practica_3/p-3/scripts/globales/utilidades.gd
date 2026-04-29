## Nombre: Jesús, Apellidos: Muñoz Velasco, Titulación: GIM
## email: jesusmuve18@correo.ugr.es, DNI o pasaporte: 76668409W

extends Node

# De la practica 2
func GenerateRevolutionMesh(
	profile: PackedVector2Array, 
	n_copies: int) -> ArrayMesh :
	
	var vertices:= PackedVector3Array()
	var triangulos:= PackedInt32Array()
				
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
	
	return GenerateArrayMesh(vertices, triangulos)
	
# De la practica 1
func ArrayMeshEstrellaZ( n: int ) -> ArrayMesh :
	var st = SurfaceTool.new()
	st.begin(Mesh.PRIMITIVE_TRIANGLES)
	
	var centro := Vector3(0,0,0)
	var p0 := Vector3(0,1,0) # Punto superior
	var p : PackedVector3Array = []
	
	var esc = 0.4 #Tamaño del tramo corto con respecto al largo


	for i in range(n):
		var angulo = (i * 2 * PI) / n 	# Angulo para las puntas
		var angulo_ex = angulo + PI/n	 	# Angulo para el tramo corto
		var rot = Basis(Vector3.LEFT, angulo) # Rotacion para las puntas
		var rot2 = Basis(Vector3.LEFT, angulo_ex) # Rotacion para tramo corto
		p.append(centro + rot * p0) # Añado la punta
		p.append(centro + rot2 * esc * p0) # Añado el vertice del tramo corto
		
		
	for i in range(2*n):
		var p1 : Vector3 = p[i%(2*n)]
		var p2 : Vector3 = p[(i+1)%(2*n)]
		_add_triangulo_color_w(st, p1, p2, centro)
	
	return st.commit()
	
# De la practica 1
func _add_triangulo_color_w(st: SurfaceTool, a: Vector3, b: Vector3, c: Vector3, normal_override: Vector3 = Vector3.ZERO):
	var normal = normal_override
	if normal == Vector3.ZERO:
		normal = Plane(a, b, c).normal
	st.set_normal(normal)
	# Vértice A
	st.set_color(Color(a.x, a.y, a.z))
	st.add_vertex(a)

	# Vértice B
	st.set_color(Color(b.x, b.y, b.z))
	st.add_vertex(b)

	# Centro siempre blanco
	st.set_color(Color.WHITE)
	st.add_vertex(c)

# De la practica 2
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

# Genera el arrayMesh a partir de los vertices y triangulos
func GenerateArrayMesh(vertices: PackedVector3Array, triangulos: PackedInt32Array) -> ArrayMesh:
	## Calculo las normales 
	var normales = calcNormales(vertices, triangulos)
	
	## inicializar el array con las tablas
	var tablas : Array = []   ## tabla vacía incialmente
	tablas.resize( Mesh.ARRAY_MAX ) ## redimensionar al tamaño adecuado
	tablas[ Mesh.ARRAY_VERTEX ] = vertices
	tablas[ Mesh.ARRAY_INDEX  ] = triangulos
	tablas[ Mesh.ARRAY_NORMAL] = normales
	
	## crear e inicialzar el objeto 'mesh' de este nodo 
	var mesh = ArrayMesh.new() ## crea malla en modo diferido, vacía
	mesh.add_surface_from_arrays( Mesh.PRIMITIVE_TRIANGLES, tablas )
	
	return mesh
