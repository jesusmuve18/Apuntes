extends MeshInstance3D

@export var color_selected : Color = Color.RED

var mat_selected := StandardMaterial3D.new()

func _ready() -> void:
	mat_selected.albedo_color = color_selected

func click():
	## cambiar el color al ser clickado
	print("La esfera ha sido clickada")
	material_override = mat_selected

func unclick():
	## volver al color original
	print("La esfera ha sido clickada")
	material_override = null
