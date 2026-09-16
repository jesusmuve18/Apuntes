## Nombre: Jesús, Apellidos: Muñoz Velasco, Titulación: GIM
## email: jesusmuve18@correo.ugr.es, DNI o pasaporte: 76668409W

extends MeshInstance3D

# Numero de copias del perfil a revolucionar
@export var n_copies : int = 30

func _ready() -> void:
	var vertices := PackedVector3Array()
	var triangulos := PackedInt32Array()
	var uvs := PackedVector2Array()
	
	# Creo el perfil de un jarron (diseñado en geogebra)
	var profile := PackedVector2Array([
		Vector2(0.4,0), #A
		Vector2(0.55,0.2), #B
		Vector2(0.55,1.7), #C
		Vector2(0.4,1.9), #D
	])
	
	Utilidades.generate_revolution_mesh_CCT(profile, n_copies, vertices, triangulos, uvs)
	var normales := Utilidades.calcNormales(vertices, triangulos)
	
	## inicializar el array con las tablas
	var tablas : Array = []   ## tabla vacía incialmente
	tablas.resize( Mesh.ARRAY_MAX ) ## redimensionar al tamaño adecuado
	tablas[ Mesh.ARRAY_VERTEX ] = vertices
	tablas[ Mesh.ARRAY_INDEX  ] = triangulos
	tablas[ Mesh.ARRAY_NORMAL ] = normales
	tablas[ Mesh.ARRAY_TEX_UV ] = uvs
	
	## crear e inicialzar el objeto 'mesh' de este nodo 
	mesh = ArrayMesh.new() ## crea malla en modo diferido, vacía
	mesh.add_surface_from_arrays( Mesh.PRIMITIVE_TRIANGLES, tablas )
	
	## asignar la textura de la lata
	var mat := StandardMaterial3D.new()
	mat.albedo_texture = Utilidades.CargarTextura("res://textures/text-lata-1.jpg")
	mat.metallic = 0.8
	mat.roughness = 0.5
	mat.shading_mode = BaseMaterial3D.SHADING_MODE_PER_PIXEL
	
	material_override = mat
