## Nombre: Jesús, Apellidos: Muñoz Velasco, Titulación: GIM
## email: jesusmuve18@correo.ugr.es, DNI o pasaporte: 76668409W

extends Node

func crear_cubo_en( pos ):
	var nuevo_cubo = MeshInstance3D.new()
	nuevo_cubo.mesh = BoxMesh.new()
	# Elevarlo para que no atraviese el suelo
	nuevo_cubo.position = pos + Vector3.UP * 0.5
	var mat = StandardMaterial3D.new()
	mat.albedo_color = Color(randf(), randf(), randf())
	nuevo_cubo.material_override = mat
	get_tree().get_current_scene().add_child(nuevo_cubo)

var selected_material: StandardMaterial3D = StandardMaterial3D.new()

func _ready() -> void:
	selected_material.albedo_color = Color.ORANGE_RED
	selected_material.cull_mode = BaseMaterial3D.CULL_DISABLED
	selected_material.metallic = 0.0      # 0 = no metálico, 1 = metálico
	selected_material.roughness = 0.5     # 0 = superficie lisa, 1 = rugosa
	selected_material.specular = 0.5      # intensidad del brillo especular
	selected_material.emission_enabled = true                   # activar emisión
	selected_material.emission = Color(1, 0, 0)                # color de la luz que emite
	selected_material.emission_energy = 2.0                     # intensidad de la emisión

func selectedMaterial() -> StandardMaterial3D :
	return selected_material
