## Nombre: Jesús, Apellidos: Muñoz Velasco, Titulación: GIM
## email: jesusmuve18@correo.ugr.es, DNI o pasaporte: 76668409W

extends MeshInstance3D

var material = StandardMaterial3D.new()

func _ready():
	var n_lados := 8
	
	var piramide = ArrayMeshEstrellaZ(n_lados)
	var mesh_instance = MeshInstance3D.new()
	mesh_instance.mesh = piramide
	material.vertex_color_use_as_albedo = true
	mesh_instance.material_override = material
	add_child(mesh_instance)
	
func ArrayMeshEstrellaZ( n: int ) -> ArrayMesh :
	var st = SurfaceTool.new()
	st.begin(Mesh.PRIMITIVE_TRIANGLES)
	
	var centro := Vector3(0.5,0.5,0)
	var vertice := Vector3(0.5,0.5,0.5)
	var p0 := Vector3(0.5,1,0) # Punto superior
	var p : PackedVector3Array = []

	var offset := p0 - centro

	for i in range(n):
		var angulo = (i * 2 * PI) / n 	# Angulo para las puntas
		var angulo_ex = angulo + PI/n	 	# Angulo para el tramo corto
		var rot = Basis(Vector3.FORWARD, angulo) # Rotacion para las puntas
		var rot2 = Basis(Vector3.FORWARD, angulo_ex) # Rotacion para tramo corto
		var esc = 0.5
		p.append(centro + rot * offset) # Añado la punta
		p.append(centro + rot2 * esc * offset) # Añado el vertice del tramo corto
		
		
	for i in range(2*n):
		var p1 : Vector3 = p[i%(2*n)]
		var p2 : Vector3 = p[(i+1)%(2*n)]
		_add_triangulo_color(st, p2, p1, centro)
		_add_triangulo_color(st, p1, p2, vertice)
	
	return st.commit()
	
# Añade triángulo y calcula normal automáticamente
func _add_triangulo_color(st: SurfaceTool, a: Vector3, b: Vector3, c: Vector3, normal_override: Vector3 = Vector3.ZERO):
	var normal = normal_override
	if normal == Vector3.ZERO:
		normal = Plane(a, b, c).normal
	st.set_normal(normal)
	# Vértice A
	st.set_color(Color(a.x, a.y, a.z))
	st.add_vertex(a)

	# Vértice B
	st.set_color(Color(b.x, b.y, b.z))
	st.add_vertex(b)

	# Centro siempre blanco
	st.set_color(Color.WHITE)
	st.add_vertex(c)
