## Nombre: Jesús, Apellidos: Muñoz Velasco, Titulación: GIM
## email: jesusmuve18@correo.ugr.es, DNI o pasaporte: 76668409W

extends MeshInstance3D

func _init() -> void:
	## crear las tablas de vértices y triángulos de un cubo de 8 vertices 
	var size: float = 2
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
	
	var normales := Utilidades.calcNormales( vertices, triangulos )
	
	# Después de calcular las normales
	for i in range(vertices.size()):
		var n = normales[i].normalized()
		var desplazamiento = 0.2  # cuánto quieres "abrir" el cubo
		#vertices[i] += n * desplazamiento
	
	
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
	mat.albedo_color = Color(1.0, 0.502, 0.161, 1.0)
	mat.metallic = 0.3
	mat.roughness = 0.1
	mat.shading_mode = BaseMaterial3D.SHADING_MODE_PER_PIXEL
	
	material_override = mat
	
	## Dibujar normales
	var normales_inst: MeshInstance3D = Utilidades2.dibujarNormalesMesh(vertices, normales, 0.5)
	add_child(normales_inst)
	
	## Translado el cubo para ponerlo al lado del de 8 vertices
	var tras := Transform3D().translated( Vector3( -3, 0.0, 0.0))
	transform = tras
