package com.jesusmuve.etsiitsmartcampus.feature.route

import com.jesusmuve.etsiitsmartcampus.core.model.DemoRoute
import com.jesusmuve.etsiitsmartcampus.core.model.EdgeKind
import com.jesusmuve.etsiitsmartcampus.core.model.IndoorMap
import com.jesusmuve.etsiitsmartcampus.core.model.IndoorNode

/**
 * Tramo de la ruta de muestra: ir de [from] (donde está el usuario, punto rojo del plano) a [to]
 * (siguiente punto) siguiendo la instrucción del sentido de la marcha.
 *
 * @property bearingDegrees Rumbo real del tramo (grados desde el norte, sentido horario) para la
 * brújula de «¿Voy bien?»; `null` en escaleras, ascensores o si no se puede calcular.
 * @property levelChange Plantas que se suben (positivo) o bajan (negativo) en el tramo; lo usa el
 * barómetro. 0 si el tramo no cambia de planta.
 * @property photoAsset Foto del punto donde está el usuario ([from]), vista en el sentido de la
 * marcha. `null` si todavía no hay.
 */
data class RouteLeg(
    val from: IndoorNode,
    val to: IndoorNode,
    val kind: EdgeKind,
    val instructionEs: String,
    val instructionEn: String,
    val bearingDegrees: Double? = null,
    val levelChange: Int = 0,
    val photoAsset: String? = null,
)

/**
 * Convierte una [DemoRoute] (lista fija de paradas escrita en `graph.json`) en los tramos que
 * recorre la pantalla. No busca caminos: entre cada dos paradas seguidas toma el tramo del grafo
 * que las une, con la instrucción del sentido en que se recorre.

 */
object DemoRouteLegs {

    /**
     * Tramos de [route] sobre [map], en orden. Devuelve `null` si la ruta está mal enlazada (un
     * nodo que no existe o dos paradas sin tramo entre ellas); `IndoorMapParser` ya lo comprueba
     * al cargar, así que en la app no debería ocurrir.
     */
    fun build(map: IndoorMap, route: DemoRoute): List<RouteLeg>? {
        if (route.stops.size < 2) return null
        return route.stops.zipWithNext().map { (stop, next) ->
            val from = map.node(stop.nodeId) ?: return null
            val to = map.node(next.nodeId) ?: return null
            val (edge, reversed) = map.connection(from.id, to.id) ?: return null
            RouteLeg(
                from = from,
                to = to,
                kind = edge.kind,
                instructionEs = (if (reversed) edge.reverseInstructionEs else edge.instructionEs) ?: return null,
                instructionEn = (if (reversed) edge.reverseInstructionEn else edge.instructionEn) ?: return null,
                // Al recorrerlo al revés, el rumbo fijado a mano se invierte.
                bearingDegrees = PlanBearing.forLeg(
                    from,
                    to,
                    edge.kind,
                    edge.bearingDegrees?.let { if (reversed) it + 180.0 else it },
                    map.building,
                ),
                levelChange = levelOf(map, to.floorId) - levelOf(map, from.floorId),
                photoAsset = stop.photoAsset,
            )
        }
    }

    private fun levelOf(map: IndoorMap, floorId: String): Int = map.floor(floorId)?.level ?: 0
}
