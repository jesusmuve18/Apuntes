## Nombre: Jesús, Apellidos: Muñoz Velasco, Titulación: GIM
## email: jesusmuve18@correo.ugr.es, DNI o pasaporte: 76668409W

extends MeshInstance3D

var material = StandardMaterial3D.new()

func _ready():
	var n := 10 # Numero en eje X
	var m := 10 # Numero en eje Z
	
	var plano = ArrayMeshRejilla(m, n)
	var mesh_instance = MeshInstance3D.new()
	mesh_instance.mesh = plano
	material.vertex_color_use_as_albedo = true
	mesh_instance.material_override = material
	add_child(mesh_instance)
	
func ArrayMeshRejilla( m: int, n: int ) -> ArrayMesh:
	var st = SurfaceTool.new()
	st.begin(Mesh.PRIMITIVE_TRIANGLES)
	var p : = []
	
	# Añado los vertices
	for i in range(n+1):
		var fila = PackedVector3Array()
		var tras_x : float = float(i)/n
		for j in range(m+1):
			var tras_z : float = float(j)/m			
			fila.append(Vector3(tras_x, 0.0, tras_z))
		p.append(fila)
	
	# Añado los triangulos
	for i in range(n):
		for j in range(m):
			var a = p[i][j]
			var b = p[i][j+1]
			var c = p[i+1][j]
			var d = p[i+1][j+1]
			
			_add_triangulo_color(st, a, c, b)
			_add_triangulo_color(st, c, d, b)
	
	return st.commit()
	
# Añade triángulo y calcula normal automáticamente
func _add_triangulo_color(st: SurfaceTool, a: Vector3, b: Vector3, c: Vector3, normal_override: Vector3 = Vector3.ZERO):
	var normal = normal_override
	if normal == Vector3.ZERO:
		normal = Plane(a, b, c).normal
	st.set_normal(normal)
	# vertice A
	st.set_color(Color(a.x, a.y, a.z))
	st.add_vertex(a)

	# vertice B
	st.set_color(Color(b.x, b.y, b.z))
	st.add_vertex(b)

	# vertice C
	st.set_color(Color(c.x, c.y, c.z))
	st.add_vertex(c)
