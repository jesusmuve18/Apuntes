## Nombre: Jesús, Apellidos: Muñoz Velasco, Titulación: GIM
## email: jesusmuve18@correo.ugr.es, DNI o pasaporte: 76668409W

extends Camera3D

## ----------------------------------------------------------------------
## contantes y variables de instancia 

const at   := 2.5   ## ángulo de rot. con teclas (en grados x pulsación)
const ar   := 0.5   ## ángulo de rot. con ratón (en grados x pixel)
var   bdrp := false ## botón derecho del ratón presionado sí/no
var   dz   := 3.0   ## distancia en Z de la cámara al origen
var   dxy  := Vector2( 0.0, 0.0 ) ## ángulos hor. y vert. (en grados)

@onready var ray = $RayCast3D # == nodo `RayCast3D` hijo de la cámara

## -------------------------------------------------------------------
## actualiza la variable 'transform' de este nodo cámara

func _actualiza_transf_vista(  ) -> void : 
	var ahr  := ((45.0+float(dxy.x))*2.0*PI)/360.0 ## ang.horiz.radi.
	var avr  := ((30.0+float(dxy.y))*2.0*PI)/360.0 ## ang.vert.radi.
	var tras := Transform3D().translated( Vector3( 0.0, 0.0, dz))	
	var rotx := Transform3D().rotated( Vector3.RIGHT, -avr )
	var roty := Transform3D().rotated( Vector3.UP, ahr ) 
	transform = roty*rotx*tras   ## actualiza transform del nodo cámara

## -------------------------------------------------------------------
func _ready() -> void :  ## se ejecuta una vez al inicio
	_actualiza_transf_vista() ## inicializa la vista al inicio
	
## -------------------------------------------------------------------
## procesa evento de entrada

func _input( event : InputEvent ): 
	
	var av : bool = true ## actualizar vista sí/no
	var mouse_pos = get_viewport().get_mouse_position()
	var from = project_ray_origin( mouse_pos )
	var to = from + project_ray_normal( mouse_pos ) * 1000.0
	
	if event is InputEventKey and event.pressed: ## teclas
		match event.keycode:
			KEY_UP:    
				dxy += Vector2( 0, -at )
			KEY_DOWN:  
				dxy += Vector2( 0, +at )
			KEY_RIGHT: 
				dxy += Vector2( -at, 0 )
			KEY_LEFT:
				dxy += Vector2( at, 0 )
			KEY_MINUS, KEY_PAGEDOWN, KEY_KP_SUBTRACT: 
				dz *= 1.05 
			KEY_PLUS, KEY_PAGEUP, KEY_KP_ADD: 
				dz = max( dz/1.05, 0.1 )
			_: 
				av = false
				
	elif event is InputEventMouseButton: ## botón ratón o rueda
		match event.button_index:
			MOUSE_BUTTON_RIGHT: 
				bdrp = event.pressed 
				av = false 
			MOUSE_BUTTON_WHEEL_DOWN: 
				dz *= 1.05
			MOUSE_BUTTON_WHEEL_UP:   
				dz = max( dz/1.05, 0.1 )
			MOUSE_BUTTON_LEFT:
				if event.pressed:
					ray.global_position = from
					ray.look_at(to)
					ray.target_position = Vector3(0, 0, -1000) # Apunta en su -Z local
					ray.force_raycast_update()
					if ray.is_colliding():
						var objeto = ray.get_collider()
						var padre : Node = objeto.get_parent()
						if padre and padre.visible:
							print("Seleccionado:", padre.name)
							# Mantengo la utilidad de clicar en el suelo cuando es
							if padre.name == "Suelo":
								Utilidades.crear_cubo_en(ray.get_collision_point())
								print("Creado cubo en:", ray.get_collision_point())
						seleccionar(padre)
					else:
						print("Ningun objeto seleccionado")
			_: av = false
				
	elif event is InputEventMouseMotion and bdrp: ## movim. ratón
		dxy += ar * Vector2( -event.relative.x, event.relative.y ) 
		
	else: ## otros tipos de eventos (no hace nada)
		av = false ## (no actualizar transf)

	if av:
		_actualiza_transf_vista( )
		
func seleccionar(selected : Node) -> void:
	if(selected!=null and selected.has_method("select")): selected.select()

	
