## Nombre: Jesús, Apellidos: Muñoz Velasco, Titulación: GIM
## email: jesusmuve18@correo.ugr.es, DNI o pasaporte: 76668409W

extends Node3D

func _ready() -> void:
	var n := 10 # Numero en eje X
	var m := 10 # Numero en eje Z
	
	var material = StandardMaterial3D.new()
	material.vertex_color_use_as_albedo = true
	
	var cubo_central := Node3D.new()
	var plano := ArrayMeshRejilla(m, n)
	
	# Añado las caras al cubo central
	for i in range(6):
		var mesh_plano = MeshInstance3D.new()
		mesh_plano.mesh = plano
		mesh_plano.material_override = material
		
		if(i<4):
			var angulo : float = (i*2*PI)/4
			mesh_plano.rotate(Vector3.UP,angulo)
		if(i>=4):
			var angulo : float = ((i-3.5)*PI)
			print(angulo)
			mesh_plano.rotate(Vector3.RIGHT,angulo)
			
		mesh_plano.translate(Vector3(0,0,0.5))
		cubo_central.add_child(mesh_plano)
	
	add_child(cubo_central)
	
	# Añado los cubos al cubo central
	material.albedo_color = Color(1.0, 1.0, 1.0, 1.0)
	var size_cubo : float = 0.3
	var alargar : float = 1.5
	var cubo := ArrayMeshCubo24(size_cubo)
	
	for i in range(6):
		var mesh_cubo = MeshInstance3D.new()
		mesh_cubo.mesh = cubo
		mesh_cubo.material_override = material
		
		if(i<4):
			var angulo : float = (i*2*PI)/4
			mesh_cubo.rotate(Vector3.UP,angulo)
		if(i>=4):
			var angulo : float = ((i-3.5)*PI)
			print(angulo)
			mesh_cubo.rotate(Vector3.RIGHT,angulo)
		
		
		mesh_cubo.translate(Vector3(0,0,(size_cubo*alargar/2)+0.5))
		mesh_cubo.scale = Vector3(1,1,alargar)
		add_child(mesh_cubo)
	
	
	
	
	
func ArrayMeshRejilla( m: int, n: int ) -> ArrayMesh:
	var st = SurfaceTool.new()
	st.begin(Mesh.PRIMITIVE_TRIANGLES)
	var p : = []
	
	for i in range(n+1):
		var fila = PackedVector3Array()
		var tras_x : float = float(i)/n
		for j in range(m+1):
			var tras_y : float = float(j)/m			
			fila.append(Vector3(-0.5+tras_x, -0.5+tras_y, 0))
		p.append(fila)
		
		
	for i in range(n):
		for j in range(m):
			var a = p[i][j]
			var b = p[i][j+1]
			var c = p[i+1][j]
			var d = p[i+1][j+1]
			
			_add_triangulo_color(st, a, b, c)
			_add_triangulo_color(st, c, b, d)
			
	
	return st.commit()
	
# Añade triángulo y calcula normal automáticamente
func _add_triangulo_color(st: SurfaceTool, a: Vector3, b: Vector3, c: Vector3, normal_override: Vector3 = Vector3.BACK):
	var normal = normal_override
	if normal != Vector3.BACK:
		normal = Plane(a, b, c).normal
	
	# Vértice A
	st.set_normal(normal)
	st.set_color(Color(a.x, a.y, a.z))
	st.add_vertex(a)

	# Vértice B
	st.set_normal(normal)
	st.set_color(Color(b.x, b.y, b.z))
	st.add_vertex(b)

	# Vértice C
	st.set_normal(normal)
	st.set_color(Color(c.x, c.y, c.z))
	st.add_vertex(c)

func ArrayMeshCubo24(size: float = 2) -> ArrayMesh:
	## crear las tablas de vértices y triángulos de un cubo de 8 vertices 
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
	
	## crear e inicialzar el objeto a devolver
	var mesh := ArrayMesh.new() ## crea malla en modo diferido, vacía
	mesh.add_surface_from_arrays( Mesh.PRIMITIVE_TRIANGLES, tablas )
	
	return mesh
