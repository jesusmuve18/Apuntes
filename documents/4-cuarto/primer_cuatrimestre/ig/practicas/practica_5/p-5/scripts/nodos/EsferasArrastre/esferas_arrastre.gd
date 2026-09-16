## Nombre: Jesús, Apellidos: Muñoz Velasco, Titulación: GIM
## email: jesusmuve18@correo.ugr.es, DNI o pasaporte: 76668409W

extends Node3D

@export var n : int = 5
@export var m : int = 8
@export var r : float = 0.1

@export var n_radios : int = 32
@export var disposicion : String = "radial"  # "rejilla" o "radial"

const SCRIPT_SPHERE : String = "res://scripts/nodos/EsferasArrastre/esfera-seleccionable.gd"

func _ready() -> void:
	# Utilizare la jerarquia MesInstance3D -> StaticBody3D -> CollisionShape3D
	var mesh_esfera = SphereMesh.new()
	mesh_esfera.radius = r
	mesh_esfera.height = 2*r
	
	var collision_shape = SphereShape3D.new()
	collision_shape.radius = r
	
	# Coloco las esferas
	for i in range(n):
		for j in range(m):
			var esfera = MeshInstance3D.new()
			var cuerpo = StaticBody3D.new()
			var colision = CollisionShape3D.new()
			
			esfera.mesh = mesh_esfera
			colision.shape = collision_shape
			
			# Utilizare la jerarquia MesInstance3D -> StaticBody3D -> CollisionShape3D
			cuerpo.add_child(colision)
			esfera.add_child(cuerpo)
			
			esfera.set_script(load(SCRIPT_SPHERE)) # Le asocio el script para que se pueda seleccionar
			esfera.name = str(i) + "," + str(j)
			if disposicion == "rejilla":
				esfera.translate((3*r) * Vector3(i,0,j)) 
			if disposicion == "radial":
				esfera.translate((3*r*(i+2)) * Vector3(cos(j * 2*PI/m),0,sin(j * 2*PI/m)))
			add_child(esfera)
