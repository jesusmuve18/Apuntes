extends MeshInstance3D

var blanco := Color.WHITE
var gris := Color(0.5,0.5,0.5)

func _ready() -> void:
	var vertices := PackedVector3Array([
		Vector3(-1,-1,1), Vector3(0,-1,1), Vector3(1,-1,1),
		Vector3(-1,-1,0), Vector3(0,-1,0), Vector3(1,-1,0),
		Vector3(-1,1,0), Vector3(0,1,0), Vector3(1,1,0),
		Vector3(-1,1,-1), Vector3(0,1,-1), Vector3(1,1,-1),
	])
	
	var triangulos := PackedInt32Array([
		0,4,3, 0,1,4,		1,5,4, 1,2,5,
		3,7,6, 3,4,7,		4,8,7, 4,5,8,
		6,10,9, 6,7,10,		7,11,10, 7,8,11
	])
	
	var colores := PackedColorArray([
		blanco, blanco, blanco,
		gris, gris, gris,
		blanco, blanco, blanco,
		gris, gris, gris
	])
	
	
	## inicializar el array con las tablas
	var tablas : Array = []   ## tabla vacía incialmente
	tablas.resize( Mesh.ARRAY_MAX ) ## redimensionar al tamaño adecuado
	tablas[ Mesh.ARRAY_VERTEX ] = vertices
	tablas[ Mesh.ARRAY_INDEX  ] = triangulos
	tablas[ Mesh.ARRAY_COLOR ] = colores
	
	## crear e inicialzar el objeto 'mesh' de este nodo 
	mesh = ArrayMesh.new() ## crea malla en modo diferido, vacía
	mesh.add_surface_from_arrays( Mesh.PRIMITIVE_TRIANGLES, tablas )
	
	## crear un material
	var mat := StandardMaterial3D.new() 
	# Uso el color que se ha establecido
	mat.vertex_color_use_as_albedo = true
	# Deshabilito la iluminacion
	mat.shading_mode = BaseMaterial3D.SHADING_MODE_UNSHADED
	# Dejo habilitado el cribado de caras 
	# (no hace falta pero por hacerlo explicito)
	mat.cull_mode = BaseMaterial3D.CULL_FRONT 
	material_override = mat
	
	
