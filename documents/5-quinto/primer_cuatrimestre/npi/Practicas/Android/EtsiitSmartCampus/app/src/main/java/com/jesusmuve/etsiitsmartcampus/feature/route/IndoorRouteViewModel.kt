package com.jesusmuve.etsiitsmartcampus.feature.route

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.jesusmuve.etsiitsmartcampus.core.data.IndoorMapResult
import com.jesusmuve.etsiitsmartcampus.core.data.IndoorMapSource
import com.jesusmuve.etsiitsmartcampus.core.model.DemoRoute
import com.jesusmuve.etsiitsmartcampus.core.model.IdentifiedPoint
import com.jesusmuve.etsiitsmartcampus.core.model.IndoorMap
import com.jesusmuve.etsiitsmartcampus.core.model.ScanSource
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.flow.update
import kotlinx.coroutines.launch

/** Fase de la pantalla de ruta interior. */
enum class RoutePhase {
    /** Cargando el mapa. */
    LOADING,

    /** El mapa no se ha podido cargar. */
    LOAD_ERROR,

    /** El usuario elige punto de partida, destino y si quiere evitar escaleras. */
    CHOOSING,

    /** Guiado paso a paso. */
    GUIDING,

    /** Destino alcanzado. */
    ARRIVED,
}

/** Avisos puntuales que la interfaz traduce a texto. */
sealed interface RouteMessage {
    /** Origen y destino coinciden. */
    data object SameOriginAndDestination : RouteMessage

    /**
     * La ruta elegida no está en la demostración: solo existen la ida de la entrada al aula y la
     * vuelta (: se dice claramente en lugar de inventar un camino).
     */
    data object NotInDemo : RouteMessage

    /**
     * Se ha leído un punto que no es el siguiente de la ruta. [readId] puede no
     * existir en el mapa (etiqueta ajena); la interfaz busca el nombre en el idioma activo.
     */
    data class UnexpectedPoint(val readId: String) : RouteMessage

    /** El punto [nodeId] se ha confirmado (: se indica si fue manual). */
    data class Confirmed(val nodeId: String, val source: ScanSource) : RouteMessage
}

/**
 * Estado inmutable de la ruta interior.
 *
 * @property originId Punto de partida elegido (un nodo del grafo).
 * @property destinationId Destino elegido: un nodo del grafo o un aula del horario
 * ([ROOM_PREFIX] + número, p. ej. `aula:0.7`) que no tiene ruta en la demostración.
 * @property extraRoom Aula del horario con la que se abrió la pantalla si no es un nodo del grafo
 * (p. ej. «0.7»); aparece como un destino más.
 * @property avoidStairs `true` = la variante por el ascensor.
 * @property legs Tramos de la ruta en curso; vacía fuera del guiado.
 * @property currentLegIndex Tramo que se está recorriendo (el usuario va hacia `legs[i].to`).
 * @property arrivalPhotoAsset Foto de la última parada, para la pantalla de llegada.
 * @property confirmations Cómo se confirmó cada punto alcanzado, en orden.
 */
data class IndoorRouteUiState(
    val phase: RoutePhase = RoutePhase.LOADING,
    val map: IndoorMap? = null,
    val loadProblems: List<String> = emptyList(),
    val originId: String? = null,
    val destinationId: String? = null,
    val extraRoom: String? = null,
    val avoidStairs: Boolean = false,
    val legs: List<RouteLeg> = emptyList(),
    val currentLegIndex: Int = 0,
    val arrivalPhotoAsset: String? = null,
    val confirmations: List<ScanSource> = emptyList(),
    val message: RouteMessage? = null,
) {
    /** Tramo actual durante el guiado; `null` en otras fases. */
    val currentLeg: RouteLeg?
        get() = if (phase == RoutePhase.GUIDING) legs.getOrNull(currentLegIndex) else null

    /** Ruta de muestra que corresponde a las opciones elegidas; `null` si no hay ninguna. */
    val selectedRoute: DemoRoute?
        get() {
            val origin = originId ?: return null
            val destination = destinationId ?: return null
            return map?.let { demoRouteFor(it, origin, destination, avoidStairs) }
        }
}

/** Prefijo de los destinos que son aulas del horario sin nodo en el grafo (`aula:0.7`). */
const val ROOM_PREFIX = "aula:"

/** Identificador de nodo que tendría el aula [room] del horario: «2.2» → `aula_2_2`. */
fun roomNodeId(room: String): String = "aula_" + room.trim().replace('.', '_')

/** Ruta de muestra de [originId] a [destinationId]; `null` si esa variante no existe. */
fun demoRouteFor(map: IndoorMap, originId: String, destinationId: String, avoidStairs: Boolean): DemoRoute? =
    map.routes.firstOrNull { route ->
        route.originId == originId && route.destinationId == destinationId && route.accessible == avoidStairs
    }

/**
 * Ruta de muestra de la entrada al aula ([returnTrip] `false`) o del aula a la entrada (`true`).
 */
fun demoRouteFor(map: IndoorMap, returnTrip: Boolean, avoidStairs: Boolean): DemoRoute? {
    val entrance = map.building.entranceNodeId
    val classroom = map.building.defaultDestinationNodeId
    return if (returnTrip) {
        demoRouteFor(map, classroom, entrance, avoidStairs)
    } else {
        demoRouteFor(map, entrance, classroom, avoidStairs)
    }
}

/**
 * Lógica de la ruta interior de muestra. El usuario puede elegir cualquier punto de partida y
 * destino, pero solo hay rutas **fijas** (de la entrada al aula y la vuelta, por escalera o por
 * ascensor) escritas en `graph.json`: no se calcula ningún camino. Para el resto se avisa de que
 * no están en la demostración.
 *
 * Importante: la ruta **solo avanza** si el usuario confirma el punto
 * («Ya estoy aquí») o si se lee la etiqueta QR/NFC del punto esperado. Nunca avanza por una
 * estimación de sensores ni afirma saber dónde está el usuario.
 *
 * Integración con el lector y con el gesto (trabajo de otros miembros): cuando el lector QR/NFC
 * devuelva un [IdentifiedPoint], la pantalla debe llamar a [onPointIdentified]. Este `ViewModel`
 * no abre la cámara ni escucha sensores.
 *
 * @param mapSource Fuente del grafo (`assets/indoor/graph.json`).
 * @param initialOriginId Punto de partida propuesto (por ejemplo, la entrada al llegar desde la
 * navegación exterior). Si no es un nodo, se usa la entrada.
 * @param initialDestinationId Destino propuesto: un nodo (desde la ficha de un aula) o un aula
 * del horario con [ROOM_PREFIX]. Si falta, el aula de la demostración (o la entrada si se sale
 * del aula).
 */
class IndoorRouteViewModel(
    private val mapSource: IndoorMapSource,
    private val initialOriginId: String? = null,
    private val initialDestinationId: String? = null,
) : ViewModel() {

    private val _uiState = MutableStateFlow(IndoorRouteUiState())

    /** Estado observable por la pantalla. */
    val uiState: StateFlow<IndoorRouteUiState> = _uiState.asStateFlow()

    init {
        loadMap()
    }

    /** (Re)carga el mapa. Se llama al crear el `ViewModel` y desde el botón «Reintentar». */
    fun loadMap() {
        _uiState.update { it.copy(phase = RoutePhase.LOADING, loadProblems = emptyList()) }
        viewModelScope.launch {
            when (val result = mapSource.load()) {
                is IndoorMapResult.Loaded -> {
                    val map = result.map
                    val building = map.building
                    val origin = initialOriginId?.takeIf { map.node(it) != null } ?: building.entranceNodeId
                    val requestedRoom = initialDestinationId
                        ?.takeIf { it.startsWith(ROOM_PREFIX) }
                        ?.removePrefix(ROOM_PREFIX)
                        ?.takeIf { it.isNotBlank() }
                    // Un aula del horario que sí está en el grafo («2.2» → aula_2_2) es un nodo más.
                    val roomNode = requestedRoom?.let(::roomNodeId)?.takeIf { map.node(it) != null }
                    val room = requestedRoom.takeIf { roomNode == null }
                    val destination = when {
                        roomNode != null -> roomNode
                        room != null -> ROOM_PREFIX + room
                        initialDestinationId != null && map.node(initialDestinationId) != null -> initialDestinationId
                        origin == building.defaultDestinationNodeId -> building.entranceNodeId
                        else -> building.defaultDestinationNodeId
                    }
                    _uiState.update {
                        it.copy(
                            phase = RoutePhase.CHOOSING,
                            map = map,
                            originId = origin,
                            destinationId = destination,
                            extraRoom = room,
                        )
                    }
                }

                is IndoorMapResult.Invalid -> _uiState.update {
                    it.copy(phase = RoutePhase.LOAD_ERROR, loadProblems = result.problems)
                }
            }
        }
    }

    /** Cambia el punto de partida (cualquier nodo del grafo). */
    fun selectOrigin(nodeId: String) {
        if (_uiState.value.map?.node(nodeId) == null) return
        _uiState.update { it.copy(originId = nodeId, message = null) }
    }

    /** Cambia el destino: un nodo del grafo o el aula del horario con la que se abrió. */
    fun selectDestination(id: String) {
        val state = _uiState.value
        val valid = state.map?.node(id) != null || (state.extraRoom != null && id == ROOM_PREFIX + state.extraRoom)
        if (!valid) return
        _uiState.update { it.copy(destinationId = id, message = null) }
    }

    /** Intercambia origen y destino (para volver por donde se ha venido). */
    fun swapOriginAndDestination() {
        val state = _uiState.value
        // Un aula del horario no es un nodo del grafo: no puede ser punto de partida.
        if (state.destinationId?.startsWith(ROOM_PREFIX) == true) return
        _uiState.update { it.copy(originId = it.destinationId, destinationId = it.originId, message = null) }
    }

    /** Activa o desactiva la variante por el ascensor (sin escaleras). */
    fun setAvoidStairs(avoid: Boolean) {
        _uiState.update { it.copy(avoidStairs = avoid, message = null) }
    }

    /** Empieza el guiado por la ruta de muestra que corresponde a las opciones elegidas. */
    fun startRoute() {
        val state = _uiState.value
        val map = state.map ?: return
        if (state.originId == state.destinationId) {
            _uiState.update { it.copy(message = RouteMessage.SameOriginAndDestination) }
            return
        }
        val route = state.selectedRoute
        val legs = route?.let { DemoRouteLegs.build(map, it) }
        if (route == null || legs.isNullOrEmpty()) {
            _uiState.update { it.copy(message = RouteMessage.NotInDemo) }
            return
        }
        _uiState.update {
            it.copy(
                phase = RoutePhase.GUIDING,
                legs = legs,
                currentLegIndex = 0,
                arrivalPhotoAsset = route.stops.last().photoAsset,
                confirmations = emptyList(),
                message = null,
            )
        }
    }

    /** «Ya estoy aquí»: el usuario confirma a mano que ha llegado al siguiente punto. */
    fun confirmManually() {
        val target = _uiState.value.currentLeg?.to ?: return
        advance(target.id, ScanSource.MANUAL)
    }

    /**
     * Punto leído por QR o NFC. Solo avanza si es el siguiente punto de la ruta; si no, se
     * explica el error y el paso no cambia.
     */
    fun onPointIdentified(point: IdentifiedPoint) {
        val target = _uiState.value.currentLeg?.to ?: return
        if (point.id == target.id) {
            advance(target.id, point.source)
        } else {
            _uiState.update {
                it.copy(
                    message = RouteMessage.UnexpectedPoint(readId = point.id),
                )
            }
        }
    }

    /** Vuelve al tramo anterior (por ejemplo, si el usuario confirmó por error). */
    fun previousLeg() {
        _uiState.update { state ->
            if (state.phase != RoutePhase.GUIDING || state.currentLegIndex == 0) {
                state
            } else {
                state.copy(
                    currentLegIndex = state.currentLegIndex - 1,
                    confirmations = state.confirmations.dropLast(1),
                    message = null,
                )
            }
        }
    }

    /** Cancela la ruta y vuelve a la elección de origen y destino. */
    fun cancelRoute() {
        _uiState.update {
            it.copy(
                phase = RoutePhase.CHOOSING,
                legs = emptyList(),
                currentLegIndex = 0,
                confirmations = emptyList(),
                message = null,
            )
        }
    }

    /** Tras llegar: propone una nueva ruta empezando donde está ahora el usuario (la vuelta). */
    fun planNewRouteFromHere() {
        _uiState.update {
            it.copy(
                phase = RoutePhase.CHOOSING,
                originId = it.destinationId,
                destinationId = it.originId,
                legs = emptyList(),
                currentLegIndex = 0,
                confirmations = emptyList(),
                message = null,
            )
        }
    }

    /** La interfaz ya ha mostrado el aviso. */
    fun messageShown() {
        _uiState.update { it.copy(message = null) }
    }

    private fun advance(reachedId: String, source: ScanSource) {
        _uiState.update { state ->
            val confirmations = state.confirmations + source
            val nextIndex = state.currentLegIndex + 1
            val confirmed = RouteMessage.Confirmed(nodeId = reachedId, source = source)
            if (nextIndex >= state.legs.size) {
                state.copy(phase = RoutePhase.ARRIVED, confirmations = confirmations, message = confirmed)
            } else {
                state.copy(currentLegIndex = nextIndex, confirmations = confirmations, message = confirmed)
            }
        }
    }
}
