extends Node3D

@export var alpha : float = deg_to_rad(45)
@export var beta : float = deg_to_rad(-160)
@export var h : float =  1
@export var b : float = 0.25

func _ready() -> void:
	# Coloco alpha y beta en [-PI, PI]
	while (alpha <= -PI):
		alpha+=2*PI
	while (alpha > PI):
		alpha-=2*PI
		
	while (beta <= -PI):
		beta+=2*PI
	while (beta >= PI):
		beta-=2*PI
	
	# Importante que tenga medida 1 para hacer bien las transformaciones
	var mcubo : ArrayMesh = ArrayMeshCubo24(1)
	var material_a = StandardMaterial3D.new()
	material_a.albedo_color = Color.RED
	var material_b = StandardMaterial3D.new()
	material_b.albedo_color = Color.BLUE
	var material_c = StandardMaterial3D.new()
	material_c.albedo_color = Color.GREEN
	
	var mesh_a = MeshInstance3D.new()
	mesh_a.mesh = mcubo
	mesh_a.material_override = material_a
	mesh_a.scale = Vector3(b,h,b)
	add_child(mesh_a)
	
	var mesh_b = MeshInstance3D.new()
	mesh_b.mesh = mcubo
	mesh_b.material_override = material_b
	if(alpha >=0 ):
		mesh_b.translate(Vector3(-(b/2.0)*(1.0-cos(alpha)), (b/2.0)*sin(alpha)+h, 0))
	else:
		mesh_b.translate(Vector3((b/2.0)*(1.0-cos(alpha)), (b/2.0)*sin(-alpha)+h, 0))
	mesh_b.rotate(Vector3.BACK, alpha)
	mesh_b.scale = Vector3(b,h,b)
	add_child(mesh_b)
	
	var mesh_c = MeshInstance3D.new()
	mesh_c.mesh = mcubo
	mesh_c.material_override = material_c
	if(alpha >=0 ):
		mesh_c.translate(Vector3(-(b/2.0)*(1.0-cos(alpha)), (b/2.0)*sin(alpha)+h, 0))
	else:
		mesh_c.translate(Vector3((b/2.0)*(1.0-cos(alpha)), (b/2.0)*sin(-alpha)+h, 0))
	mesh_c.rotate(Vector3.BACK, alpha)
	if(beta >=0 ):
		mesh_c.translate(Vector3(-(b/2.0)*(1.0-cos(beta)), (b/2.0)*sin(beta)+h, 0))
	else:
		mesh_c.translate(Vector3((b/2.0)*(1.0-cos(beta)), (b/2.0)*sin(-beta)+h, 0))
	mesh_c.rotate(Vector3.BACK, beta)
	mesh_c.scale = Vector3(b,h,b)
	add_child(mesh_c)
	
	
	
# Crea un cubo de 24 vertices de tamaño size
func ArrayMeshCubo24(size: float = 1) -> ArrayMesh:
	## crear las tablas de vértices y triángulos de un cubo de 8 vertices 
	var vertices := PackedVector3Array([])

	for x in [-size/2, size/2]:
		for y in [0, size]:
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
