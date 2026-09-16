extends Node3D

@export var ancho_patas : float = 0.25
@export var alto_patas : float = 0.75
@export var ancho_tablero : float =  2
@export var alto_tablero : float = 0.2

func _ready() -> void:
	# Importante que tenga medida 1 para hacer bien las transformaciones
	var mcubo : ArrayMesh = ArrayMeshCubo24(1)
	var material_patas = StandardMaterial3D.new()
	material_patas.albedo_color = Color.RED
	var material_tablero = StandardMaterial3D.new()
	material_tablero.albedo_color = Color.BLUE
	
	# Creo las patas
	for i in range(4):
		var mesh_pata := MeshInstance3D.new()
		mesh_pata.mesh = mcubo
		mesh_pata.material_override = material_patas
		
		var angulo := (i*2*PI)/4
		var traslacion := (ancho_tablero - ancho_patas)/2
		var escala := Vector3(ancho_patas, alto_patas, ancho_patas)
		
		mesh_pata.rotate(Vector3.UP, angulo)
		mesh_pata.translate(Vector3(traslacion, alto_patas/2, traslacion)) 
		mesh_pata.scale = escala
		
		add_child(mesh_pata)
		
	# Creo el tablero
	var mesh_tablero = MeshInstance3D.new()
	mesh_tablero.mesh = mcubo
	mesh_tablero.material_override = material_tablero
	
	var traslacion := alto_patas + (alto_tablero/2)
	var escala := Vector3(ancho_tablero, alto_tablero, ancho_tablero)
	
	mesh_tablero.translate(Vector3(0, traslacion, 0)) 
	mesh_tablero.scale = escala
	
	add_child(mesh_tablero)
	
# Crea un cubo de 24 vertices de tamaño size
func ArrayMeshCubo24(size: float = 1) -> ArrayMesh:
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
