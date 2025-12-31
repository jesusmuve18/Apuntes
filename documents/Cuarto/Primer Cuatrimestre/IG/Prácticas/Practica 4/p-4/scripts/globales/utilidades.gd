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
		#Calcular el valor del parámetro u
		var phi = atan2(v.z, v.x)
		var u = max_u*((phi / (2*PI)+0.5))
		#  Calcular el valor del parámetro v
		#Inserta tu código aquí
		#Puedes calcularlo en función del desplazaniento en el perfil
		# o de forma aproximada en función de y
		var v_coord =  (v.y - min_y) / (max_y - min_y) * max_v
		var uv_coords = Vector2(1-u,1-v_coord)
		uvs.append(uv_coords)
	return uvs

# Carga una textura 
func CargarTextura( arch : String ) -> ImageTexture :
	## crear un objeto 'Image' con la imgen
	var imagen := Image.new()
	assert( imagen.load(arch) == OK, "Error cargando '"+arch+"'." )
	
	## crear un objeto 'ImageTexture' a partir del objeto 'Image'
	var textura := ImageTexture.create_from_image( imagen )
	print("Textura cargada desde archivo: '",arch,"'.")
	
	## devolver la textura
	return textura

# Crea un cubo de 24 vertices de tamaño size con coordenadas uv
func ArrayMeshCubo24_CCT(size: float = 1) -> ArrayMesh:
	## crear las tablas de vértices y triángulos de un cubo de 8 vertices 
	var vertices := PackedVector3Array([])

	for x in [-size/2, size/2]:
		for y in [-size/2, size/2]:
			for z in [-size/2, size/2]:
				for k in range (3): # Triplico los vertices
					vertices.append(Vector3(x, y, z))
				
	
	var triangulos := PackedInt32Array([
		0,6,11, 0,11,3,
		5,10,21, 5,21,16,
		15,23,18, 15,18,14,
		12,20,7, 12,7,1,
		9,8,19, 9,19,22,
		2,4,17, 2,17,13
	])
	
	var normales := calcNormales( vertices, triangulos )
	
	var uvs := PackedVector2Array([
		Vector2(1,1), Vector2(0,1), Vector2(1,1), Vector2(0,1),
		Vector2(1,0), Vector2(1,1), Vector2(1,0), Vector2(0,0),
		Vector2(1,0), Vector2(1,1), Vector2(1,0), Vector2(0,0),
		Vector2(1,1), Vector2(0,1), Vector2(0,1), Vector2(1,1),
		Vector2(0,1), Vector2(0,0), Vector2(0,0), Vector2(0,0),
		Vector2(1,0), Vector2(0,0), Vector2(0,1), Vector2(1,0)
	])
	
	## inicializar el array con las tablas
	var tablas : Array = []   ## tabla vacía incialmente
	tablas.resize( Mesh.ARRAY_MAX ) ## redimensionar al tamaño adecuado
	tablas[ Mesh.ARRAY_VERTEX ] = vertices
	tablas[ Mesh.ARRAY_INDEX  ] = triangulos
	tablas[ Mesh.ARRAY_NORMAL ] = normales
	tablas[ Mesh.ARRAY_TEX_UV ] = uvs
	
	## crear e inicialzar el objeto a devolver
	var mesh := ArrayMesh.new() ## crea malla en modo diferido, vacía
	mesh.add_surface_from_arrays( Mesh.PRIMITIVE_TRIANGLES, tablas )
	
	return mesh

# Crea las tablas de vertices, triangulos y normales para una circunferencia de 
# n*m triangulos
func crear_circunferencia(n: int, m:int, 
						  vertices: PackedVector3Array, 
						  triangulos: PackedInt32Array, 
						  normales: PackedVector3Array):
	assert(n>2 && m>0)
	
	var angulo :float = 2*PI/n
	
	for i in range(n):
		var rot = Transform3D(Basis(Vector3(0,0,-1), i*angulo), Vector3.ZERO)
		for j in range(m+1):
			vertices.append(rot * ((float(j)/m) * Vector3(0,1.0,0)))
	
	for i in range(n):
		for j in range(m):
			var a : int = (i*(m+1)+j)
			var b : int = (i*(m+1)+(j+1))
			var c : int = (((i+1)%n)*(m+1)+j)
			var d : int = (((i+1)%n)*(m+1)+(j+1))
			
			triangulos.append_array([a, d, c])
			triangulos.append_array([a, b, d])
	
	normales.append_array(Utilidades.calcNormales(vertices, triangulos))
