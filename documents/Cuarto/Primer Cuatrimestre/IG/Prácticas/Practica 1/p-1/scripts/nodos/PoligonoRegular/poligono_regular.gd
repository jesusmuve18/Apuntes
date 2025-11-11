## Nombre: Jesús, Apellidos: Muñoz Velasco, Titulación: GIM
## email: jesusmuve18@correo.ugr.es, DNI o pasaporte: 76668409W

extends MeshInstance3D

var material = StandardMaterial3D.new()

func _ready():
	var n_lados := 8
	
	var piramide = ArrayMeshPoligonoRegular(n_lados)
	var mesh_instance = MeshInstance3D.new()
	mesh_instance.mesh = piramide
	material.vertex_color_use_as_albedo = true
	mesh_instance.material_override = material
	add_child(mesh_instance)
	
func ArrayMeshPoligonoRegular( n: int ) -> ArrayMesh :
	var st = SurfaceTool.new()
	st.begin(Mesh.PRIMITIVE_TRIANGLES)
	
	var centro := Vector3(0.5,0.5,0)
	var p0 := Vector3(0.5,1,0) # Punto superior
	var p : PackedVector3Array = []

	var offset := p0 - centro

	for i in range(n):
		var angulo = (i * 2 * PI) / n
		var rot = Basis(Vector3.FORWARD, angulo)
		p.append(centro + rot * offset)
		
	for i in range(n):
		var p1 : Vector3 = p[i%n]
		var p2 : Vector3 = p[(i+1)%n]
		_add_triangulo_color(st, p1, p2, centro)
	
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
