## Nombre: Jesús, Apellidos: Muñoz Velasco, Titulación: GIM
## email: jesusmuve18@correo.ugr.es, DNI o pasaporte: 76668409W

extends Node

var size : float = 1.0 # Tamaño del cubo
var margin : float = 0.4 # Margen entre los cubos

var mat : StandardMaterial3D

# Vertices de la cara de abajo
var a00 := Vector2(0, 3.0/4)
var a10 := Vector2(1/3.0, 3.0/4)
var a01 := Vector2(0,1)
var a11 := Vector2(1/3.0, 1)

# Vertices de la cara lateral
var l00 := Vector2(2.0/3,3.0/4)
var l10 := Vector2(1,3.0/4)
var l01 := Vector2(2.0/3, 1)
var l11 := Vector2(1,1)


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
## 
## Crea un cubo de 24 vertices de tamaño size con coordenadas uv
##
##    size = tamaño del lado del cubo
func CrearTecla(i : int, j: int) -> MeshInstance3D:
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
	
	# Vertices de la cara superior
	var c := Vector2(i*(1.0/3.0), j*(1.0/4.0))
	var c00 = c
	var c10 = c + Vector2(1.0/3, 0)
	var c01 = c + Vector2(0, 1.0/4)
	var c11 = c + Vector2(1.0/3, 1.0/4)
	
	
	var uvs := PackedVector2Array([
		l01, l11, a10, 
		l11, a11, l01,
		l00, l10, c00, 
		c01, l00, l10, 
		l01, a00, l11,
		l01, l11, a01,
		l10, c10, l00,
		l10, c11, l00
	])
	
	print(uvs)
	
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
	
	var mesh_instance := MeshInstance3D.new()
	mesh_instance.mesh = mesh
	mesh_instance.position = Vector3(-(size+margin) + i*(size+margin) , 0, -(size+margin) + j*(size+margin))
	mesh_instance.material_override = mat
	mesh_instance.create_trimesh_collision() # Creamos la geometria de colision
		
	return mesh_instance


func _ready() -> void:
	mat = StandardMaterial3D.new()
	mat.albedo_texture = CargarTextura("res://textures/teclado3.png")
	mat.metallic = 0.4
	mat.roughness = 0.3
	mat.shading_mode = BaseMaterial3D.SHADING_MODE_PER_PIXEL
