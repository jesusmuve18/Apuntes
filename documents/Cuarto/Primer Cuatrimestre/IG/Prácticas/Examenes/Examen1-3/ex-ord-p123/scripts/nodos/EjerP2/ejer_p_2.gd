extends MeshInstance3D

@export var n : int = 3 # En el eje X
@export var m : int = 2 # En el eje Z

func _ready() -> void:
	assert(n>=0 && m>=0, "Los parametros n y m deben ser positivos")
	
	# Inicializo los arrays con el cuadrado inicial
	var vertices := PackedVector3Array([
		Vector3(0,0,0), Vector3(1,0,0),
		Vector3(1,0,1), Vector3(0,0,1)
	])
	var triangulos := PackedInt32Array([
		0,2,1, 0,3,2
	])
	
	# Genero los vertices en el eje X
	for i in range(n):
		vertices.append(Vector3(i+2,0,0))
		vertices.append(Vector3(i+2,0,1))
	
	# Genero los vertices en el eje Y
	for i in range(m):
		vertices.append(Vector3(1,0,i+2))
		vertices.append(Vector3(0,0,i+2))
	
	# Construyo triangulos en el eje X
	for i in range(n):
		var index_0 := 4+2*(i-1)
		var index_1 := 5+2*(i-1)
		var index_2 := 4+2*i
		var index_3 := 5+2*i
		
		# Corrijo los vertices iniciales
		if(i == 0):
			index_0 = 1
			index_1 = 2
		
		# Añado los triangulos
		triangulos.append_array([
			index_0, index_1, index_3,
			index_0, index_3, index_2
		])
	
	# Construyo triangulos en el eje Z
	for i in range(m):
		var index_0 := (2*n + 2) + 2*i
		var index_1 := (2*n + 3) + 2*i
		var index_2 := (2*n + 2) + 2*(i+1)
		var index_3 := (2*n + 3) + 2*(i+1)
		
		# Corrijo los vertices iniciales
		if(i == 0):
			index_0 = 2
			index_1 = 3
		
		# Añado los triangulos
		triangulos.append_array([
			index_1, index_3, index_2,
			index_1, index_2, index_0
		])
	
	## inicializar el array con las tablas
	var tablas : Array = []   ## tabla vacía incialmente
	tablas.resize( Mesh.ARRAY_MAX ) ## redimensionar al tamaño adecuado
	tablas[ Mesh.ARRAY_VERTEX ] = vertices
	tablas[ Mesh.ARRAY_INDEX  ] = triangulos
	
	## crear e inicialzar el objeto 'mesh' de este nodo 
	mesh = ArrayMesh.new() ## crea malla en modo diferido, vacía
	mesh.add_surface_from_arrays( Mesh.PRIMITIVE_TRIANGLES, tablas )
	
	## crear un material
	var mat := StandardMaterial3D.new() 
	# Uso el color blanco sin colores
	mat.albedo_color = Color.WHITE
	# Deshabilito la iluminacion
	mat.shading_mode = BaseMaterial3D.SHADING_MODE_UNSHADED
	# Dejo habilitado el cribado de caras 
	# (no hace falta pero por hacerlo explicito)
	mat.cull_mode = BaseMaterial3D.CULL_FRONT 
	material_override = mat
	
