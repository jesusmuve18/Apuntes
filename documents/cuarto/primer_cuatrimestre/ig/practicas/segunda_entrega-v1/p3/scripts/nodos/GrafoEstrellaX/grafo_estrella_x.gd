## Nombre: Jesús, Apellidos: Muñoz Velasco, Titulación: GIM
## email: jesusmuve18@correo.ugr.es, DNI o pasaporte: 76668409W

extends Node3D

@export var scalation_speed := 2.5 			# En vueltas por segundo
@export var activar := "rotar_estrella" 	# Tecla 0
@export var n : int = 8
var animacion_activa := true

var material = StandardMaterial3D.new()

func _ready() -> void:	
	# Genero la estrella de n puntas
	var estrella_n_puntas: MeshInstance3D = MeshInstance3D.new()
	estrella_n_puntas.mesh = Utilidades.ArrayMeshEstrellaZ(n)
	material.vertex_color_use_as_albedo = true
	estrella_n_puntas.material_override = material
	add_child(estrella_n_puntas)
	
	# Genero un cono a partir del perfil
	var radio_base: float = 0.14
	var altura: float = 0.15
	var n_copies: int = 10
	
	var perfil := PackedVector2Array([
		Vector2(0,0),
		Vector2(radio_base,0),
		Vector2(0,altura)
	])
	
	var cono_mesh: ArrayMesh = Utilidades.GenerateRevolutionMesh(perfil, n_copies)
	material.albedo_color = Color(1.0, 1.0, 1.0, 1.0)
	
	# Genero todos los conos
	for i in range(n):
		var cono := MeshInstance3D.new()
		cono.mesh = cono_mesh
		cono.material_override = material
	
		var angulo = (2 * PI * i) / n
		cono.translate(Vector3(0, cos(angulo), sin(angulo)))
		cono.rotate(Vector3.RIGHT, angulo)

		add_child(cono)
	
func _process(delta):
	if Input.is_action_just_pressed(activar):
		animacion_activa = !animacion_activa
	
	if animacion_activa:	
		# Girar en torno al eje x a scalation_speed vueltas por minuto
		rotation.x += (scalation_speed * 2 * PI) * delta
