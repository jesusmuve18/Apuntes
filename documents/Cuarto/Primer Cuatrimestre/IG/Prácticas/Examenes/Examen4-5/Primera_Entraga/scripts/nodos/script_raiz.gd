## Nombre: Jesús, Apellidos: Muñoz Velasco, Titulación: GIM
## email: jesusmuve18@correo.ugr.es, DNI o pasaporte: 76668409W

extends Node3D

## Ver: 
## https://docs.godotengine.org/en/4.4/classes/class_viewport.html#enum-viewport-debugdraw

var dibujar_aristas : bool = false 
var modo_sin_iluminacion: bool = false

func _init():
	RenderingServer.set_debug_generate_wireframes(true)

func _unhandled_key_input( key_event ):
	if key_event.keycode == KEY_W and not key_event.pressed :
		dibujar_aristas = not dibujar_aristas
		modo_sin_iluminacion = false
		
		var viewport = get_viewport()
		if dibujar_aristas:
			viewport.debug_draw = Viewport.DEBUG_DRAW_WIREFRAME
			print("Dibujar en modo aristas: activado")
		else:
			viewport.debug_draw = Viewport.DEBUG_DRAW_DISABLED
			print("Dibujar en modo aristas: desactivado")
	if key_event.keycode == KEY_E and not key_event.pressed :
		modo_sin_iluminacion = not modo_sin_iluminacion
		dibujar_aristas = false
		
		var viewport = get_viewport()
		if modo_sin_iluminacion:
			viewport.debug_draw = Viewport.DEBUG_DRAW_UNSHADED
			print("Dibujar en modo sin iluminacion: activado")
		else:
			viewport.debug_draw = Viewport.DEBUG_DRAW_DISABLED
			print("Dibujar en modo sin iluminacion: desactivado")
