extends Node3D

func _ready() -> void:
	for i in range(3):
		for j in range(3):
			add_child(Utilidades.CrearTecla(i,j))
