## Nombre: Jesús, Apellidos: Muñoz Velasco, Titulación: GIM
## email: jesusmuve18@correo.ugr.es, DNI o pasaporte: 76668409W

extends Node3D

@export var rotation_speed_deg := 25
@export var activar := "asentir_cabeza"
@export var seleccionar := "seleccionar_cabeza"
@export var angulo_max := 15

var activa := true
var seleccionada := false
var sentido := 1.0   # 1 = positivo, -1 = negativo

var hijos: Array = []  # lista de hijos para colorear

func _ready():
	hijos = [$cubo, $oreja_derecha, $oreja_izquierda]
	
func _process(delta):
	if Input.is_action_just_pressed(activar):
		activa = !activa
	if Input.is_action_just_pressed(seleccionar):
		select()
		
	if activa and seleccionada:
		if(sentido==1 && (rotation.x + deg_to_rad(rotation_speed_deg * delta)) >= deg_to_rad(angulo_max)):
			sentido = -1
		if(sentido==-1 && (rotation.x - deg_to_rad(rotation_speed_deg * delta)) <= deg_to_rad(-angulo_max)):
			sentido = 1
			
		rotation.x += sentido*deg_to_rad(rotation_speed_deg * delta)
		
func select() -> void:
	seleccionada = !seleccionada
	
	if(seleccionada==true):
		for hijo in hijos:
			hijo.material_override = Utilidades.selectedMaterial()
	else:
		for hijo in hijos:
			hijo.material_override=null	
