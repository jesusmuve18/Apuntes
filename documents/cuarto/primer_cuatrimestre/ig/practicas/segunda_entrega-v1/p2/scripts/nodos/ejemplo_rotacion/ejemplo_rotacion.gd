## Nombre: Jesús, Apellidos: Muñoz Velasco, Titulación: GIM
## email: jesusmuve18@correo.ugr.es, DNI o pasaporte: 76668409W

extends MeshInstance3D

# Numero de copias del perfil a revolucionar
@export var n_copies : int = 20

func _ready() -> void:
	var vertices := PackedVector3Array()
	var triangulos := PackedInt32Array()
	
	# Creo el perfil de un jarron (diseñado en geogebra)
	var profile := PackedVector2Array([
		Vector2(0,0), #A
		Vector2(0.5,0), #B
		Vector2(0.87,0.5), #C
		Vector2(1,1), #D
		Vector2(0.8,1.4), #E
		Vector2(0.5,1.6), #F
		Vector2(0.55,2.14), #G
		Vector2(0.69,2.5), #H
		Vector2(0.8,2.5), #I
		Vector2(0.8,2.6), #J
		Vector2(0.62,2.6), #K
		Vector2(0.5,2.5), #L
		Vector2(0.32,2.16), #M
		Vector2(0.32, 1.6), #N
		Vector2(0.62,1.4), #O
		Vector2(0.8,1), #P
		Vector2(0.7,0.5), #Q
		Vector2(0.47, 0.2), #R
		Vector2(0,0.2), #S
	])
	
	Utilidades2.generate_revolution_mesh(profile, n_copies, vertices, triangulos)
	var normales := Utilidades.calcNormales(vertices, triangulos)
	
	## inicializar el array con las tablas
	var tablas : Array = []   ## tabla vacía incialmente
	tablas.resize( Mesh.ARRAY_MAX ) ## redimensionar al tamaño adecuado
	tablas[ Mesh.ARRAY_VERTEX ] = vertices
	tablas[ Mesh.ARRAY_INDEX  ] = triangulos
	tablas[ Mesh.ARRAY_NORMAL] = normales
	
	## crear e inicialzar el objeto 'mesh' de este nodo 
	mesh = ArrayMesh.new() ## crea malla en modo diferido, vacía
	mesh.add_surface_from_arrays( Mesh.PRIMITIVE_TRIANGLES, tablas )
	
	## crear un material
	var mat := StandardMaterial3D.new() 
	mat.albedo_color = Color(0.596, 0.2, 0.773, 1.0)
	mat.metallic = 0.3
	mat.roughness = 0.1
	mat.shading_mode = BaseMaterial3D.SHADING_MODE_PER_PIXEL
	
	material_override = mat
	
