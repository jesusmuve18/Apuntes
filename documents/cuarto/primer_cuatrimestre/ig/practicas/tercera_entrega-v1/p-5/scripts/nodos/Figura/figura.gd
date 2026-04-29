extends MeshInstance3D

## Hago que no se lean las colisiones de objetos ocultos
var col

func _ready() -> void:
	col = $StaticBody3D/CollisionShape3D

func _process(_delta):
	col.disabled = not visible
