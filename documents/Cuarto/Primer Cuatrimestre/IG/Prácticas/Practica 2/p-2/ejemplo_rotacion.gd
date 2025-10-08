extends MeshInstance3D

func _init() -> void:
	var profile := PackedVector2Array()  # array vacío
	var n_copies := 10
	var vertices := PackedVector3Array()
	var triangulos := PackedInt32Array()
	
	# Agregar puntos
	profile.append(Vector2(1,-1.5))
	profile.append(Vector2(1,-1))
	profile.append(Vector2(0.5,-0.5))
	profile.append(Vector2(0.5, 0.5))
	profile.append(Vector2(0.75, 0.75))
	profile.append(Vector2(0.5, 1))
	
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
	mat.albedo_color = Color(1.0, 0.434, 0.807, 1.0)
	mat.metallic = 0.3
	mat.roughness = 0.1
	mat.shading_mode = BaseMaterial3D.SHADING_MODE_PER_PIXEL
	
	material_override = mat
	
