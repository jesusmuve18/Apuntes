## Nombre: Jesús, Apellidos: Muñoz Velasco, Titulación: GIM
## email: jesusmuve18@correo.ugr.es, DNI o pasaporte: 76668409W

extends Node3D

@export var rotation_speed_deg := 100
@export var activar := "mover_brazos"
@export var seleccionar := "seleccionar_brazos"

var angulo_max := deg_to_rad(90.0)
var activa := true
var seleccionada := false
var sentido := 1.0   # 1 = positivo, -1 = negativo

var hijos: Array = []  # lista de hijos para colorear

func _ready():
	hijos = [$dedos_izquierda, $antebrazo_izquierdo]
	
func _process(delta):
	if Input.is_action_just_pressed(activar):
		activa = !activa
	if Input.is_action_just_pressed(seleccionar):
		select()
	
	if activa and seleccionada:	
		if(sentido==1 && rotation.x + deg_to_rad(rotation_speed_deg * delta) >= angulo_max):
			sentido = -1	
		if(sentido==-1 && rotation.x - deg_to_rad(rotation_speed_deg * delta) <= -angulo_max):
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
