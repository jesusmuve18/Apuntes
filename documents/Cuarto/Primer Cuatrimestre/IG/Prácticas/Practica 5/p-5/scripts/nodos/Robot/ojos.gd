## Nombre: Jesús, Apellidos: Muñoz Velasco, Titulación: GIM
## email: jesusmuve18@correo.ugr.es, DNI o pasaporte: 76668409W

extends Node3D

@export var translation_speed := 0.2
@export var activar := "mover_ojos"

var translation_max := 0.15
var activa := false
var sentido := 1.0   # 1 = positivo, -1 = negativo

func _process(delta):
	if Input.is_action_just_pressed(activar):
		activa = !activa
	
	if activa:
		if(sentido==1 && (position.x + (translation_speed * delta)) >= translation_max):
			sentido = -1
		if(sentido==-1 && (position.x - (translation_speed * delta)) <= -translation_max):
			sentido = 1
			
		position.x += sentido * translation_speed * delta
			
func select() -> void:
	activa = true

func unselect() -> void:
	activa = false				
