package com.jesusmuve.etsiitsmartcampus.core.data

import com.jesusmuve.etsiitsmartcampus.core.model.IndoorMap
import kotlinx.serialization.SerializationException
import kotlinx.serialization.json.Json

/**
 * Convierte el JSON del mapa interior en un [IndoorMap] y comprueba que es coherente.
 *
 * No depende de Android.  * `assets/`. Un JSON inválido nunca cierra la app: devuelve [IndoorMapResult.Invalid] con la
 * lista de problemas.
 */
object IndoorMapParser {

    /** Versión del formato que entiende esta versión de la app. */
    const val SUPPORTED_VERSION = 1

    private val json = Json {
        ignoreUnknownKeys = true
    }

    /** Lee y valida [text]. */
    fun parse(text: String): IndoorMapResult {
        val map = try {
            json.decodeFromString(IndoorMap.serializer(), text)
        } catch (e: SerializationException) {
            return IndoorMapResult.Invalid(listOf("JSON no válido: ${e.message}"))
        } catch (e: IllegalArgumentException) {
            return IndoorMapResult.Invalid(listOf("JSON no válido: ${e.message}"))
        }
        val problems = validate(map)
        return if (problems.isEmpty()) {
            IndoorMapResult.Loaded(map)
        } else {
            IndoorMapResult.Invalid(problems)
        }
    }

    /**
     * Comprueba la coherencia del grafo y devuelve los problemas encontrados (vacía si es válido):
     * identificadores únicos y no vacíos, referencias a plantas y nodos existentes, rutas de
     * muestra bien enlazadas, posiciones de plano entre 0 y 1 y coordenadas geográficas plausibles.
     */
    fun validate(map: IndoorMap): List<String> {
        val problems = mutableListOf<String>()

        if (map.version != SUPPORTED_VERSION) {
            problems += "Versión ${map.version} no soportada (se esperaba $SUPPORTED_VERSION)"
        }
        if (map.floors.isEmpty()) problems += "No hay plantas"
        if (map.nodes.isEmpty()) problems += "No hay nodos"

        duplicates(map.floors.map { it.id }).forEach { problems += "Planta repetida: $it" }
        duplicates(map.nodes.map { it.id }).forEach { problems += "Nodo repetido: $it" }

        val floorIds = map.floors.map { it.id }.toSet()
        val nodeIds = map.nodes.map { it.id }.toSet()

        map.nodes.forEach { node ->
            if (node.id.isBlank()) problems += "Hay un nodo sin identificador"
            if (node.floorId !in floorIds) {
                problems += "El nodo ${node.id} usa una planta inexistente: ${node.floorId}"
            }
            if (node.planX !in 0f..1f || node.planY !in 0f..1f) {
                problems += "El nodo ${node.id} tiene una posición de plano fuera de 0..1"
            }
        }

        map.edges.forEach { edge ->
            val label = "${edge.from} → ${edge.to}"
            if (edge.from !in nodeIds) problems += "El tramo $label sale de un nodo inexistente"
            if (edge.to !in nodeIds) problems += "El tramo $label llega a un nodo inexistente"
            if (edge.from == edge.to) problems += "El tramo $label une un nodo consigo mismo"
            if (edge.instructionEs.isBlank() || edge.instructionEn.isBlank()) {
                problems += "Al tramo $label le falta la instrucción ES o EN"
            }
            if ((edge.reverseInstructionEs == null) != (edge.reverseInstructionEn == null)) {
                problems += "El tramo $label tiene instrucción inversa solo en un idioma"
            }
        }

        validateRoutes(map, nodeIds, problems)

        val building = map.building
        if (building.entranceNodeId !in nodeIds) {
            problems += "La entrada (${building.entranceNodeId}) no es un nodo del grafo"
        }
        if (building.defaultDestinationNodeId !in nodeIds) {
            problems += "El destino por defecto (${building.defaultDestinationNodeId}) no es un nodo"
        }
        if (building.entranceLatitude !in -90.0..90.0 || building.entranceLongitude !in -180.0..180.0) {
            problems += "Las coordenadas de la entrada no son válidas"
        }
        if (building.planDepthFactor <= 0.0 || building.planDepthFactor > 1.0) {
            problems += "planDepthFactor debe estar entre 0 y 1"
        }
        if (building.planAspectRatio <= 0.0) problems += "planAspectRatio debe ser positivo"
        if (building.floorHeightMeters <= 0.0) problems += "floorHeightMeters debe ser positivo"
        if (building.arrivalRadiusMeters <= 0.0) {
            problems += "El radio de llegada debe ser positivo"
        }
        return problems
    }

    /**
     * Rutas de muestra: al menos una, identificadores únicos, dos paradas como mínimo, nodos que
     * existen y un tramo del grafo (en el sentido de la marcha) entre cada dos paradas seguidas.
     */
    private fun validateRoutes(map: IndoorMap, nodeIds: Set<String>, problems: MutableList<String>) {
        if (map.routes.isEmpty()) problems += "No hay ninguna ruta de muestra"
        duplicates(map.routes.map { it.id }).forEach { problems += "Ruta repetida: $it" }
        map.routes.forEach { route ->
            if (route.stops.size < 2) problems += "La ruta ${route.id} necesita al menos dos paradas"
            route.stops.forEach { stop ->
                if (stop.nodeId !in nodeIds) problems += "La ruta ${route.id} pasa por un nodo inexistente: ${stop.nodeId}"
            }
            route.stops.zipWithNext().forEach { (a, b) ->
                if (a.nodeId in nodeIds && b.nodeId in nodeIds && map.connection(a.nodeId, b.nodeId) == null) {
                    problems += "En la ruta ${route.id} no hay tramo de ${a.nodeId} a ${b.nodeId}"
                }
            }
        }
    }

    private fun duplicates(ids: List<String>): Set<String> =
        ids.groupingBy { it }.eachCount().filterValues { it > 1 }.keys
}
