## Nombre: Jesús, Apellidos: Muñoz Velasco, Titulación: GIM
## email: jesusmuve18@correo.ugr.es, DNI o pasaporte: 76668409W

extends MeshInstance3D

func _ready() -> void:
	
	## crear las tablas de vértices y triángulos de un Donut 
	var vertices   := PackedVector3Array([])
	var triangulos := PackedInt32Array([])
	Utilidades.generarDonut( vertices, triangulos )
	
	## crear las tablas de normales de un donut
	var normales := Utilidades.calcNormales( vertices, triangulos )
	
	## crear las coordenadas de textura del donut (x,z)
	var uvs := PackedVector2Array([])
	
	for v in vertices:
		uvs.append(Vector2(v.x, v.z))
			
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
	
	## crear un material con la textura
	var mat := StandardMaterial3D.new() 
	mat.albedo_texture = Utilidades.CargarTextura("res://textures/ea-textura-cuadricula.jpeg")
	mat.metallic = 0.3
	mat.roughness = 0.2
	mat.shading_mode = BaseMaterial3D.SHADING_MODE_PER_PIXEL
	
	material_override = mat
	
	
	
	
	
