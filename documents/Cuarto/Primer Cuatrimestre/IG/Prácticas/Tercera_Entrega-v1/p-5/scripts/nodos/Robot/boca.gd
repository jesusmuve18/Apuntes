## Nombre: Jesús, Apellidos: Muñoz Velasco, Titulación: GIM
## email: jesusmuve18@correo.ugr.es, DNI o pasaporte: 76668409W

extends MeshInstance3D

@export var scalation_speed := 0.75
@export var activar := "escalar_boca"
@export var seleccionar := "seleccionar_boca"

var scalation_max := 3
var scalation_min := 0.5
var activa := true
var seleccionada := false
var sentido := 1.0   # 1 = positivo, -1 = negativo

func _process(delta):
	if Input.is_action_just_pressed(activar):
		activa = !activa
	if Input.is_action_just_pressed(seleccionar):
		select()
	
	if activa and seleccionada:
		if(sentido==1 && (scale.x + (scalation_speed * delta)) >= scalation_max):
			sentido = -1
		if(sentido==-1 && (scale.x - (scalation_speed * delta)) <= scalation_min):
			sentido = 1
			
		scale.x += sentido * scalation_speed * delta
			
func select() -> void:
	seleccionada = !seleccionada
	
	if(seleccionada==true):
		material_override = Utilidades.selectedMaterial()
	else:
		material_override=null	
