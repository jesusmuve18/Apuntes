## Nombre: Jesús, Apellidos: Muñoz Velasco, Titulación: GIM
## email: jesusmuve18@correo.ugr.es, DNI o pasaporte: 76668409W

extends Node3D

@export var altura: float = 1.5

func _ready():
	var piramide = crear_piramide(altura)
	var mesh_instance = MeshInstance3D.new()
	mesh_instance.mesh = piramide
	
	var material = StandardMaterial3D.new()
	material.albedo_color = Color(1.0, 0.1, 0.2)  # Naranja claro
	mesh_instance.material_override = material
	
	add_child(mesh_instance)

func crear_piramide(h: float) -> ArrayMesh:
	var st = SurfaceTool.new()
	st.begin(Mesh.PRIMITIVE_TRIANGLES)

	# Coordenadas de la base (cuadrado centrado en el origen, lado 1)
	var p1 = Vector3(-0.5, 0, -0.5)
	var p2 = Vector3( 0.5, 0, -0.5)
	var p3 = Vector3( 0.5, 0,  0.5)
	var p4 = Vector3(-0.5, 0,  0.5)

	var apex = Vector3(0, h, 0)  # cúspide

	# Caras laterales (triángulos)
	_add_triangulo(st, p1, p2, apex)
	_add_triangulo(st, p2, p3, apex)
	_add_triangulo(st, p3, p4, apex)
	_add_triangulo(st, p4, p1, apex)

	# Base (dos triángulos)
	_add_triangulo(st, p1, p3, p2, Vector3.DOWN)
	_add_triangulo(st, p1, p4, p3, Vector3.DOWN)

	return st.commit()

# Añade triángulo y calcula normal automáticamente
func _add_triangulo(st: SurfaceTool, a: Vector3, b: Vector3, c: Vector3, normal_override: Vector3 = Vector3.ZERO):
	var normal = normal_override
	if normal == Vector3.ZERO:
		normal = Plane(a, b, c).normal
	st.set_normal(normal)
	st.add_vertex(a)
	st.add_vertex(b)
	st.add_vertex(c)
