## Nombre: Jesús, Apellidos: Muñoz Velasco, Titulación: GIM
## email: jesusmuve18@correo.ugr.es, DNI o pasaporte: 76668409W

extends MeshInstance3D

func _init() -> void:
	mesh = crear_piramide(1)

	
func crear_piramide(h: float) -> ArrayMesh:
	var st = SurfaceTool.new()
	st.begin(Mesh.PRIMITIVE_TRIANGLES)

	# Coordenadas de la base (cuadrado centrado en el origen, lado 1)
	var p1 = Vector3(-0.5, 0, -0.5)
	var p2 = Vector3( 0.5, 0, -0.5)
	var p3 = Vector3( 0.5, 0,  0.5)
	var p4 = Vector3(-0.5, 0,  0.5)

	var apex = Vector3(0, h, 0)  # cúspide
	
	##        v0 ------ v1 ------ v2
	##         |        /\         |
	##         |       /  \        |
	##         |      /    \       |
	##         |     /      \      |
	##         |    /        \     |
	##         |   /          \    |
	##         |  /            \   |
	##        v3 ---------------- v4
	
	var v0 := Vector2(0,0)
	var v1 := Vector2(0.5,0)
	var v2 := Vector2(1,0)
	var v3 := Vector2(0,1)
	var v4 := Vector2(1,1)
	
	# Caras laterales (triángulos)
	_add_trianguloUV(st, p1, p2, apex, v4, v3, v1)
	_add_trianguloUV(st, p2, p3, apex, v4, v3, v1)
	_add_trianguloUV(st, p3, p4, apex, v4, v3, v1)
	_add_trianguloUV(st, p4, p1, apex, v4, v3, v1)

	# Base (dos triángulos)
	_add_trianguloUV(st, p1, p3, p2, v3, v2, v4, Vector3.DOWN)
	_add_trianguloUV(st, p1, p4, p3, v3, v0, v2, Vector3.DOWN)

	return st.commit()

# Añade triángulo y calcula normal automáticamente
func _add_trianguloUV(st: SurfaceTool, a: Vector3, b: Vector3, c: Vector3, 
					  a_UV: Vector2, b_UV: Vector2, c_UV: Vector2, normal_override: Vector3 = Vector3.ZERO):
	var normal = normal_override
	if normal == Vector3.ZERO:
		normal = Plane(a, b, c).normal
	st.set_normal(normal)
	
	st.set_uv(a_UV)
	st.add_vertex(a)
	
	st.set_uv(b_UV)
	st.add_vertex(b)
	
	st.set_uv(c_UV)
	st.add_vertex(c)
