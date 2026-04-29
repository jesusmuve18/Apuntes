## Nombre: Jesús, Apellidos: Muñoz Velasco, Titulación: GIM
## email: jesusmuve18@correo.ugr.es, DNI o pasaporte: 76668409W

extends MeshInstance3D

func _init() -> void:
	## crear las tablas de vértices, triangulos y colores
	var size_x: float = 1
	var size_z: float = 0.8
	var size_y: float = 0.8
	var roof_height: float = 0.4
	
	var vertices := PackedVector3Array([
		# Base
		Vector3(0, 0, size_z),
		Vector3(size_x, 0, size_z),
		Vector3(size_x, 0, 0),
		Vector3(0, 0, 0),

		# Primer piso
		Vector3(0, size_y, size_z),
		Vector3(size_x, size_y, size_z),
		Vector3(size_x, size_y, 0),
		Vector3(0, size_y, 0),

		# Tejado
		Vector3(0, size_y + roof_height, size_z/2),
		Vector3(size_x, size_y + roof_height, size_z/2)
	])
				
	var triangulos := PackedInt32Array([
		# Base
		0,5,1, 0,4,5,
		1,6,2, 1,5,6,
		2,7,3, 2,6,7,
		3,4,0, 3,7,4,
		
		# Tejado
		4,9,5, 4,8,9,
		5,9,6, 
		6,8,7, 6,9,8,
		7,8,4
	])
		
	var normales := Utilidades.calcNormales( vertices, triangulos)
	
	var colores := PackedColorArray()

	for v in vertices:
		colores.append(Color(v.x, v.y, v.z))
	
	## inicializar el array con las tablas
	var tablas : Array = []   ## tabla vacía incialmente
	tablas.resize( Mesh.ARRAY_MAX ) ## redimensionar al tamaño adecuado
	tablas[ Mesh.ARRAY_VERTEX ] = vertices
	tablas[ Mesh.ARRAY_INDEX  ] = triangulos
	tablas[ Mesh.ARRAY_NORMAL ] = normales
	tablas[ Mesh.ARRAY_COLOR  ] = colores
	
	## crear e inicialzar el objeto 'mesh' de este nodo 
	mesh = ArrayMesh.new() ## crea malla en modo diferido, vacía
	mesh.add_surface_from_arrays( Mesh.PRIMITIVE_TRIANGLES, tablas )
	
	## crear un material
	var mat := StandardMaterial3D.new() 
	mat.vertex_color_use_as_albedo = true
	
	material_override = mat
