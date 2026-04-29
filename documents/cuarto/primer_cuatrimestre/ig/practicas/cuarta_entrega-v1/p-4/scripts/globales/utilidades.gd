## Nombre: Jesús, Apellidos: Muñoz Velasco, Titulación: GIM
## email: jesusmuve18@correo.ugr.es, DNI o pasaporte: 76668409W

extends Node

## -----------------------------------------------------------------------------
## Función que genera una malla por revolucion a partir de un perfil 2D 
##
##    profile = vector de puntos 2D que define el perfil
##    n_copies = numero de copias del perfil a revolucionar
##    vertices = array de vertices que se rellenara
##    triangulos = array de triangulos que se rellenara
##
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
## Función que genera una malla por revolucion a partir de un perfil 2D con
## coordenadas de textura
##
##    profile = vector de puntos 2D que define el perfil
##    n_copies = numero de copias del perfil a revolucionar
##    vertices = array de vertices que se rellenara
##    triangulos = array de triangulos que se rellenara
##    uvs = array de coordenadas de textura que se rellenara
##
func generate_revolution_mesh_CCT(
	profile: PackedVector2Array, 
	n_copies: int,
	vertices: PackedVector3Array, 
	triangulos: PackedInt32Array,
	uvs: PackedVector2Array) -> void :
	
	var n_points = profile.size()
	
	## Calcular distancias 
	var d_ac : Array =([0.0]) # distancia acumulada a lo largo del perfil
	var d_total : float = 0
		
	for j in range(n_points-1):
		d_total+= profile[j].distance_to(profile[j+1])
		d_ac.append(d_total)
	
	## Genera vértices rotando el profile alrededor del eje y (duplicando los primeros)
	for i in range(n_copies+1): 
		var angulo = (i * 2 * PI) / n_copies
		var rot = Transform3D(Basis(Vector3.UP, angulo), Vector3.ZERO)
		
		for j in range(n_points):
			var v := profile[j]
			var v_3D = rot * Vector3(v.x, v.y, 0)
			vertices.append(v_3D)
			uvs.append(Vector2((float(i)/n_copies), 1-(d_ac[j]/d_total)))
	
	# Generar triángulos (duplicando vertices)
	for i in range(n_copies):		
		for j in range(n_points-1):  # conectamos puntos consecutivos del perfil
			# índice de los 4 vértices de este cuadrado
			var a = i * n_points + j
			var b = (i+1) * n_points + j
			var c = (i+1) * n_points + (j + 1)
			var d = i * n_points + (j + 1)
			
			triangulos.append_array([a,c,b])
			triangulos.append_array([a,d,c])
 
## -----------------------------------------------------------------------------
## Función que calcula las normales promedio de los vértices de una malla, 
## a partir de la tabla de posiciones de vértices y la tabla de triángulos
##
##    verts = tabla de posiciones de vertices
##    tris = tabla de posiciones de triangulos
##
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

## -----------------------------------------------------------------------------
## Función que calcula las coordenadas de textura a partir de un vector de 
## vertices de textura
##
##    vertices = tabla de posiciones de vertices
##
static func calcUV(vertices: PackedVector3Array) -> PackedVector2Array:
	var uvs := PackedVector2Array()
	var max_u = 1.0
	var max_v = 1.0
	
	# Calcular extremos de y
	var min_y : float = vertices[0].y
	var max_y : float = vertices[0].y
	for vert in vertices:
		if vert.y < min_y:
			min_y = vert.y
		if vert.y > max_y:
			max_y = vert.y
	
	var altura := max_y - min_y
	
	for v in vertices:
		#Calcular el valor del parámetro u
		var phi = atan2(v.z, v.x)
		var u = max_u*((phi / (2*PI)+0.5))
		
		#  Calcular el valor del parámetro v de forma aproximada en función de y
		var v_coord = max_v * ((min_y - v.y) / altura)
		var uv_coords = Vector2(1-u,1-v_coord)
		
		uvs.append(uv_coords)
	return uvs

## -----------------------------------------------------------------------------
## 
## Funcion que carga una textura 
##
##    arch = direccion de la textura
##
func CargarTextura( arch : String ) -> ImageTexture :
	## crear un objeto 'Image' con la imgen
	var imagen := Image.new()
	assert( imagen.load(arch) == OK, "Error cargando '"+arch+"'." )
	
	## crear un objeto 'ImageTexture' a partir del objeto 'Image'
	var textura := ImageTexture.create_from_image( imagen )
	print("Textura cargada desde archivo: '",arch,"'.")
	
	## devolver la textura
	return textura

## -----------------------------------------------------------------------------
## 
## Crea un cubo de 24 vertices de tamaño size con coordenadas uv
##
##    size = tamaño del lado del cubo
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

## -----------------------------------------------------------------------------
## 
## Funcion que crea las tablas de vertices, triangulos y normales para una  
## circunferencia de n*m triangulos
##
##    n = numero de radios
##    m = numero de circunferencias concentricas
##    vertices = array de vertices que se rellenara
##    triangulos = array de triangulos que se rellenara
##    normales = array de normales que se rellenara
##
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

## -----------------------------------------------------------------------------
##
## Función de parametrización de un toroide o donut 
##
##    u = 1er parámetro (entre 0 y 1)
##    v = 2o parámetro (entre 0 y 1)
##    R  = radio mayor 
##    r  = radio menor
##
func ParamDonut( u,v, r, R : float ) -> Vector3:

		var cu := cos(2.0*PI*u); var su := sin(2.0*PI*u)
		var cv := cos(2.0*PI*v); var sv := sin(2.0*PI*v)
		return Vector3( (R+r*cv)*cu, (R+r*cv)*su, r*sv )

## -----------------------------------------------------------------------------
## 
## Genera una tabla de triángulos (indices) con topología toroidal
## los añade a 'indices', 
##
##    nu = numero de divisiones del 1er parámetro
##    nv = numero de divisiones del 2o parámetro	
##    indices = tabla donde se añaden los triángulos 
##
func GenTriToroidal( nu, nv : int , indices : PackedInt32Array )  :
	
	for i in nu: 
		var isig = (i + 1) % nu
		for j in nv:
			var jsig = (j + 1) % nv
			var i00 = i    * nv + j
			var i01 = i    * nv + jsig
			var i10 = isig * nv + j
			var i11 = isig * nv + jsig
			
			indices.append(i00); indices.append(i11); indices.append(i10)
			indices.append(i00); indices.append(i01); indices.append(i11)
					
## -----------------------------------------------------------------------------
## 
## Función que genera un toroide (Donut), con 'nu x nv' vértices 
## (como una malla indexada con vértices compartidos entre triángulos adyacentes)
##
##    vertices : tabla de vértices (añade)
##    indices : tabla de índices (añade)
##    nu = numero de divisiones del 1er parámetro
##    nv = numero de divisiones del 2o parámetro	
##    R  = radio mayor 
##    r  = radio menor
##
func generarDonut( vertices: PackedVector3Array, 
 					indices: PackedInt32Array, 
					nu: int = 128, nv: int = 32,   
					R: float = 1.2, r: float = 0.4):			
	
	## Genera vértices con la geometría de un donut
	for i in nu: 
		for j in nv:
			vertices.append( ParamDonut( float(i)/nu, float(j)/nv, r, R )) 

	## Genera los triángulos con topología toroidal (la de un donut)	
	GenTriToroidal( nu, nv, indices )
