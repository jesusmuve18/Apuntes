extends Node3D

@export var textura: Texture2D

func _ready():
	crear_plano_uv(3, 1.0, 90,0,Vector3(0,1.5,-4)) # Textura completa

func crear_plano_uv( size: float, escala_uv: float,
	rx:float, rz:float, pos : Vector3 ):
	var st = SurfaceTool.new()
	st.begin(Mesh.PRIMITIVE_TRIANGLES) ## comienza a crear la malla
	var normal = Vector3.UP
	# Coordenadas de los vértices (plano en XZ, centrado en 'centro')
	var p0 = Vector3(-size, 0, -size)
	var p1 = Vector3(size, 0, -size)
	var p2 = Vector3(size, 0, size)
	var p3 = Vector3(-size, 0, size)
	# UVs multiplicados por escala para repetir textura
	var uv0 = Vector2(0, 0)
	var uv1 = Vector2(escala_uv, 0)
	var uv2 = Vector2(escala_uv, escala_uv)
	var uv3 = Vector2(0, escala_uv)
	# Primer triángulo
	st.set_normal(normal) ; st.set_uv(uv0) ; st.add_vertex(p0)
	st.set_normal(normal) ; st.set_uv(uv1) ; st.add_vertex(p1)
	st.set_normal(normal) ; st.set_uv(uv2) ; st.add_vertex(p2)
	
	# Segundo triángulo
	st.set_normal(normal) ;
	st.set_normal(normal) ;
	st.set_normal(normal) ;
	st.set_uv(uv2) ; st.add_vertex(p2)
	st.set_uv(uv3) ; st.add_vertex(p3)
	st.set_uv(uv0) ; st.add_vertex(p0)
	var mesh = st.commit() ## termina de crear la malla
	var mi = MeshInstance3D.new()
	mi.mesh = mesh
	# Crear material con textura
	var mat = StandardMaterial3D.new()
	mat.albedo_texture = textura
	mat.uv1_offset = Vector3.ZERO
	mat.uv1_scale = Vector3(1, 1, 1)
	mi.material_override = mat
	mi.rotation.x = deg_to_rad(rx)
	mi.rotation.z = deg_to_rad(rz)
	mi.position = pos
	add_child(mi)
