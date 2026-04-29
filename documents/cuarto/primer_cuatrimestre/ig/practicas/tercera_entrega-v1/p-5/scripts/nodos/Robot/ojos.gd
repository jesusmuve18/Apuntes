## Nombre: Jesús, Apellidos: Muñoz Velasco, Titulación: GIM
## email: jesusmuve18@correo.ugr.es, DNI o pasaporte: 76668409W

extends Node3D

@export var translation_speed := 0.2
@export var activar := "mover_ojos"
@export var seleccionar := "seleccionar_ojos"

var translation_max := 0.15
var activa := true
var seleccionada := false
var sentido := 1.0   # 1 = positivo, -1 = negativo

var hijos: Array = []  # lista de hijos para colorear

func _ready():
	hijos = [$ojo_derecho, $ojo_izquierdo]
	
func _process(delta):
	if Input.is_action_just_pressed(activar):
		activa = !activa
	if Input.is_action_just_pressed(seleccionar):
		select()
	
	if activa and seleccionada:
		if(sentido==1 && (position.x + (translation_speed * delta)) >= translation_max):
			sentido = -1
		if(sentido==-1 && (position.x - (translation_speed * delta)) <= -translation_max):
			sentido = 1
			
		position.x += sentido * translation_speed * delta
			
func select() -> void:
	seleccionada = !seleccionada
	
	if(seleccionada==true):
		for hijo in hijos:
			hijo.material_override = Utilidades.selectedMaterial()
	else:
		for hijo in hijos:
			hijo.material_override=null	
