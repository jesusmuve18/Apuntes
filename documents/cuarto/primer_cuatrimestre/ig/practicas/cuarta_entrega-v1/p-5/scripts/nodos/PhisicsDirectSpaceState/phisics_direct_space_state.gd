## Nombre: Jesús, Apellidos: Muñoz Velasco, Titulación: GIM
## email: jesusmuve18@correo.ugr.es, DNI o pasaporte: 76668409W

extends Node3D

var selected_object : Node = null    # almacena el objeto seleccionado

func _unhandled_input( event : InputEvent ):
	if event is InputEventMouseButton and visible:
		if event.button_index == MOUSE_BUTTON_LEFT:
			if event.pressed :
				## 1. Buscar nodo de la cámara
				var cam : Node = get_node_or_null("../../Camera3D")
				assert( cam != null , "No encuento 'CamaraOrbitalSimple'")
				
				## 2. Construir objeto con parámetros de intersección
				var query := PhysicsRayQueryParameters3D.new()
				query.collision_mask = 0xFFFFFFFF # todas las capas
				query.from = cam.project_ray_origin( event.position )
				query.to = query.from+100*cam.project_ray_normal( event.position )
				
				## 3. Calcular la intersección
				var space_state = get_world_3d().direct_space_state
				var result = space_state.intersect_ray(query)
				
				## 4. Procesar el resultado
				if result:
					print("SÍ hay objeto en ese punto.")
					var padre : Node = result.collider.get_parent()
					if padre != null :
						print("Objeto padre intersecado: ", padre.name )
						selected_object = padre
						if padre.has_method("click"): padre.click()
						else: print("Objeto padre no tiene 'click()'")
					else: print("No hay objeto padre.")
				else: print("NO hay objeto en ese punto.")
		
			else:  # si suelto el click
				if selected_object != null and selected_object.has_method("unclick"):
					selected_object.unclick()
					selected_object = null
	
	if event is InputEventMouseMotion and selected_object != null and visible:
		var delta_y = -event.relative.y * 0.01  # factor de escala para movimiento
		selected_object.translate(Vector3(0, delta_y, 0))  # mover solo en Y	
