extends MeshInstance3D

## Hago que no se lean las colisiones de objetos ocultos
@onready var col = $StaticBody3D/CollisionShape3D

func _process(_delta):
	col.disabled = not visible
