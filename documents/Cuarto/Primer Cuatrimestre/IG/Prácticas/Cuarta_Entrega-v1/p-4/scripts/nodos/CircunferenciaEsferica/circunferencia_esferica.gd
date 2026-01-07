## Nombre: Jesús, Apellidos: Muñoz Velasco, Titulación: GIM
## email: jesusmuve18@correo.ugr.es, DNI o pasaporte: 76668409W

extends MeshInstance3D

@export var n: int = 50  # Numero de radios
@export var m: int = 4  # Numero de circunferencias concentricas

func _ready() -> void:
	var vertices: PackedVector3Array;
	var triangulos: PackedInt32Array;
	var normales: PackedVector3Array;
	var uvs: PackedVector2Array;
	
	mesh = Utilidades.crear_circunferencia(n,m, vertices, triangulos, normales)
	
	# Asigamos coordenadas de textura a cada vértice de forma que una coordenada sea
	# proporcional al ángulo y la otra al radio de ese vértice
	
	for i in range(n):
		for j in range(m+1):
			uvs.append(Vector2(float(i)/n, float(j)/m))
	
	var tablas : Array = [] 
	tablas.resize(Mesh.ARRAY_MAX)
	
	tablas[Mesh.ARRAY_VERTEX] = vertices
	tablas[Mesh.ARRAY_INDEX] = triangulos
	tablas[Mesh.ARRAY_NORMAL] = normales
	tablas[Mesh.ARRAY_TEX_UV] = uvs
	
	mesh = ArrayMesh.new()
	mesh.add_surface_from_arrays(Mesh.PRIMITIVE_TRIANGLES, tablas)
	
