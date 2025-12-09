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
