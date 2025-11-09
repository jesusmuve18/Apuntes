## Nombre: Jesús, Apellidos: Muñoz Velasco, Titulación: GIM
## email: jesusmuve18@correo.ugr.es, DNI o pasaporte: 76668409W

extends MeshInstance3D

func _ready() -> void:
	var n: int = 5
	var vertices := PackedVector3Array([])
	var triangulos := PackedInt32Array([])
	
	for i in range(n+1):
		var v0 = Vector3(0.5, i, 0.5)
		for j in range(4):
			var rot = (j * 2 * PI)/4
			vertices.append(Basis(Vector3.UP, rot) * v0)
	
	for i in range(n):
		for j in range(4):
			var a = 4*i+j
			var b = 4*i+(j+1)%4
			var c = 4*(i+1)+j
			var d = 4*(i+1)+(j+1)%4
			
			triangulos.append_array([a,d,b])
			triangulos.append_array([a,c,d])
			
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
	mat.albedo_color = Color(1.0, 0.0, 1.0, 1.0)
	mat.metallic = 0.3
	mat.roughness = 0.1
	mat.shading_mode = BaseMaterial3D.SHADING_MODE_PER_PIXEL
	
	material_override = mat
