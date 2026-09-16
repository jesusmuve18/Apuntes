## Nombre: Jesús, Apellidos: Muñoz Velasco, Titulación: GIM
## email: jesusmuve18@correo.ugr.es, DNI o pasaporte: 76668409W

extends MeshInstance3D

@export var n_lados : int = 8

func _ready():	
	var poligono_regular = ArrayMeshPoligonoRegular(n_lados)
	var mesh_instance = MeshInstance3D.new()
	mesh_instance.mesh = poligono_regular
	
	var material = StandardMaterial3D.new()
	material.vertex_color_use_as_albedo = true
	mesh_instance.material_override = material
	
	add_child(mesh_instance)

# Dado un numero de lados devuelve la malla del poligono regular con dicho numero de lados y 
# contenida en el cuadrado de diagonal (0,0,0) -- (1,1,0)
func ArrayMeshPoligonoRegular( n: int ) -> ArrayMesh :
	var st = SurfaceTool.new()
	st.begin(Mesh.PRIMITIVE_TRIANGLES)
	
	var centro := Vector3(0.5,0.5,0)
	var p0 := Vector3(0.5,1,0) # Punto superior
	var p : PackedVector3Array = []

	var offset := p0 - centro
	
	# Añado los vertices a p
	for i in range(n):
		var angulo = (i * 2 * PI) / n
		var rot = Basis(Vector3.FORWARD, angulo)
		p.append(centro + rot * offset)
	
	# Añado los triangulos a st
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
	
	# vertice A
	st.set_color(Color(a.x, a.y, a.z))
	st.add_vertex(a)

	# vertice B
	st.set_color(Color(b.x, b.y, b.z))
	st.add_vertex(b)

	# vertice C (central de color blanco)
	st.set_color(Color.WHITE)
	st.add_vertex(c)
