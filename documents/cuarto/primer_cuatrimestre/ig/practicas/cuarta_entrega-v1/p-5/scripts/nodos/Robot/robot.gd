## Nombre: Jesús, Apellidos: Muñoz Velasco, Titulación: GIM
## email: jesusmuve18@correo.ugr.es, DNI o pasaporte: 76668409W

extends Node3D

## Hago que no se lean las colisiones de objetos ocultos
var hijos : Array = []

func _ready() -> void :
	hijos=[
		$Cabeza/Cara/boca/StaticBody3D/CollisionShape3D,
		$Cabeza/Cara/Ojos/ojo_derecho/StaticBody3D/CollisionShape3D,
		$Cabeza/Cara/Ojos/StaticBody3D/CollisionShape3D,
		$Cabeza/StaticBody3D/CollisionShape3D,
		$Cuerpo/Brazo_derecho/Brazo_derecho/CollisionShape3D,
		$Cuerpo/Brazo_izquierdo/Brazo_izquierdo/CollisionShape3D]

func _process(_delta):
	for hijo in hijos:
		hijo.disabled = not visible
