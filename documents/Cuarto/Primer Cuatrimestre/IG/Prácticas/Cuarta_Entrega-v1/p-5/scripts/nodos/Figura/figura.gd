## Nombre: Jesús, Apellidos: Muñoz Velasco, Titulación: GIM
## email: jesusmuve18@correo.ugr.es, DNI o pasaporte: 76668409W

extends MeshInstance3D

## Hago que no se lean las colisiones de objetos ocultos
var col

func _ready() -> void:
	col = $StaticBody3D/CollisionShape3D

func _process(_delta):
	col.disabled = not visible
