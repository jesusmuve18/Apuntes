extends MeshInstance3D

func _init() -> void:
	## crear las tablas de vértices y triángulos de un cubo de 8 vertices 
	var size: float = 2
	var vertices := PackedVector3Array([])

	for x in [-size/2, size/2]:
		for y in [-size/2, size/2]:
			for z in [-size/2, size/2]:
				vertices.append(Vector3(x, y, z))
				
	var triangulos := PackedInt32Array([
		0,2,3, 0,3,1,
		3,2,6, 3,6,7, 
		1,3,7, 1,7,5,
		0,1,5, 0,5,4,
		5,7,6, 5,6,4,
		4,6,2, 4,2,0
	])
		
	var normales := Utilidades.calcNormales( vertices, triangulos )
			
	## inicializar el array con las tablas
	var tablas : Array = []   ## tabla vacía incialmente
	tablas.resize( Mesh.ARRAY_MAX ) ## redimensionar al tamaño adecuado
	tablas[ Mesh.ARRAY_VERTEX ] = vertices
	tablas[ Mesh.ARRAY_INDEX  ] = triangulos
	tablas[ Mesh.ARRAY_NORMAL ] = normales
	
	## crear e inicialzar el objeto 'mesh' de este nodo 
	mesh = ArrayMesh.new() ## crea malla en modo diferido, vacía
	mesh.add_surface_from_arrays( Mesh.PRIMITIVE_TRIANGLES, tablas )
	
	## crear un material
	var mat := StandardMaterial3D.new() 
	mat.albedo_color = Color(0.0, 0.737, 0.871, 1.0)
	mat.metallic = 0.3
	mat.roughness = 0.1
	mat.shading_mode = BaseMaterial3D.SHADING_MODE_PER_PIXEL
	
	material_override = mat
	
	# Dibujar normales usando ImmediateMesh + MeshInstance3D
	var normales_inst: MeshInstance3D = Utilidades2.dibujarNormalesMesh(vertices, normales, 0.5, Color(1,0,0))
	add_child(normales_inst)
	
	## Translado el cubo para ponerlo al lado del de 24 vertices
	var tras := Transform3D().translated( Vector3( 2.0, 0.0, 0.0))
	#transform = tras
