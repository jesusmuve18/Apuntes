## Nombre: Jesús, Apellidos: Muñoz Velasco, Titulación: GIM
## email: jesusmuve18@correo.ugr.es, DNI o pasaporte: 76668409W

extends Node3D

func _ready() -> void:
	for i in range(3):
		for j in range(3):
			var tecla = Utilidades.CrearTecla(i,j)
			add_child(tecla)
