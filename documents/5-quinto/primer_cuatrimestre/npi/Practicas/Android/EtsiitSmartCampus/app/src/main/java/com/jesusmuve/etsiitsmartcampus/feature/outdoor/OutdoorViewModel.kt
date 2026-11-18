package com.jesusmuve.etsiitsmartcampus.feature.outdoor

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.jesusmuve.etsiitsmartcampus.core.data.IndoorMapResult
import com.jesusmuve.etsiitsmartcampus.core.data.IndoorMapSource
import com.jesusmuve.etsiitsmartcampus.core.model.Building
import com.jesusmuve.etsiitsmartcampus.core.model.IdentifiedPoint
import com.jesusmuve.etsiitsmartcampus.core.model.IndoorMap
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.flow.update
import kotlinx.coroutines.launch

/** Estado del permiso de ubicación tal como lo ve la pantalla. */
enum class LocationPermissionState {
    /** Aún no se ha preguntado (o no se sabe). */
    NOT_REQUESTED,

    /** Concedido (precisa o aproximada). */
    GRANTED,

    /** Denegado: la navegación con Google Maps sigue funcionando. */
    DENIED,
}

/**
 * Estado de la pantalla de navegación exterior.
 *
 * @property building Datos del edificio (destino y radio de llegada), `null` mientras carga.
 * @property locationAvailable `false` si la ubicación del sistema está desactivada.
 * @property tracking `true` mientras se escucha el GPS (pantalla visible y permiso concedido).
 * @property confirmedOriginNodeId Nodo confirmado escaneando su punto de control: la pantalla
 * abre la ruta interior desde él y llama a [OutdoorViewModel.onConfirmedOriginHandled].
 * @property scanNotInRoute `true` si se escaneó un punto que no pertenece a la ruta interior.
 */
data class OutdoorUiState(
    val building: Building? = null,
    val loadFailed: Boolean = false,
    val permission: LocationPermissionState = LocationPermissionState.NOT_REQUESTED,
    val locationAvailable: Boolean = true,
    val tracking: Boolean = false,
    val arrival: ArrivalStatus = ArrivalStatus.NoFix,
    val confirmedOriginNodeId: String? = null,
    val scanNotInRoute: Boolean = false,
)

/**
 * Lógica de la navegación exterior: destino en Google Maps y detección de llegada por GPS.
 *
 * El GPS solo se usa mientras la pantalla está visible: la pantalla llama a [onScreenVisible]
 * y [onScreenHidden] según su ciclo de vida. No se guarda ninguna posición: cada
 * lectura se convierte al momento en un [ArrivalStatus] y se descarta.
 *
 * @param mapSource Fuente del grafo: de él salen las coordenadas de la entrada y el radio.
 * @param tracker Acceso al GPS.
 */
class OutdoorViewModel(
    private val mapSource: IndoorMapSource,
    private val tracker: LocationTracker,
) : ViewModel() {

    private val _uiState = MutableStateFlow(
        OutdoorUiState(
            permission = if (tracker.hasPermission()) {
                LocationPermissionState.GRANTED
            } else {
                LocationPermissionState.NOT_REQUESTED
            },
        ),
    )

    /** Estado observable por la pantalla. */
    val uiState: StateFlow<OutdoorUiState> = _uiState.asStateFlow()

    private var evaluator: ArrivalEvaluator? = null
    private var indoorMap: IndoorMap? = null
    private var screenVisible = false

    private val trackerListener = object : LocationTrackerListener {
        override fun onFix(fix: GeoFix) {
            val currentEvaluator = evaluator ?: return
            _uiState.update { it.copy(arrival = currentEvaluator.evaluate(fix, it.arrival)) }
        }

        override fun onAvailabilityChanged(available: Boolean) {
            _uiState.update { it.copy(locationAvailable = available) }
        }
    }

    init {
        viewModelScope.launch {
            when (val result = mapSource.load()) {
                is IndoorMapResult.Loaded -> {
                    indoorMap = result.map
                    val building = result.map.building
                    evaluator = ArrivalEvaluator(
                        entranceLatitude = building.entranceLatitude,
                        entranceLongitude = building.entranceLongitude,
                        arrivalRadiusMeters = building.arrivalRadiusMeters,
                    )
                    _uiState.update { it.copy(building = building) }
                    // Si la pantalla ya estaba visible cuando terminó la carga, se arranca ahora.
                    updateTracking()
                }

                is IndoorMapResult.Invalid -> _uiState.update { it.copy(loadFailed = true) }
            }
        }
    }

    /** La pantalla pasa a primer plano (`Lifecycle.Event.ON_START`). */
    fun onScreenVisible() {
        screenVisible = true
        // El usuario pudo conceder el permiso desde Ajustes mientras la app estaba en segundo plano.
        if (tracker.hasPermission()) {
            _uiState.update { it.copy(permission = LocationPermissionState.GRANTED) }
        }
        updateTracking()
    }

    /** La pantalla deja de verse (`Lifecycle.Event.ON_STOP`): se libera el GPS. */
    fun onScreenHidden() {
        screenVisible = false
        updateTracking()
    }

    /** Resultado del diálogo de permisos del sistema. */
    fun onPermissionResult(granted: Boolean) {
        _uiState.update {
            it.copy(permission = if (granted) LocationPermissionState.GRANTED else LocationPermissionState.DENIED)
        }
        updateTracking()
    }

    /**
     * Punto leído con el lector (QR, NFC o código manual) desde esta pantalla.
     *
     * Es la forma fiable de confirmar que el usuario ya está dentro: el GPS solo puede sugerirlo
     * ([ArrivalStatus.ProbablyInside]). Si el ID es un nodo de la ruta interior (normalmente la
     * entrada), la ruta empieza en él; si no, se avisa y no se navega.
     */
    fun onPointIdentified(point: IdentifiedPoint) {
        val node = indoorMap?.node(point.id)
        _uiState.update {
            if (node != null) {
                it.copy(confirmedOriginNodeId = node.id, scanNotInRoute = false)
            } else {
                it.copy(scanNotInRoute = true)
            }
        }
    }

    /** La pantalla ya ha abierto la ruta interior desde el nodo confirmado. */
    fun onConfirmedOriginHandled() {
        _uiState.update { it.copy(confirmedOriginNodeId = null) }
    }

    /** La pantalla ya ha mostrado el aviso de punto ajeno a la ruta. */
    fun onScanMessageShown() {
        _uiState.update { it.copy(scanNotInRoute = false) }
    }

    private fun updateTracking() {
        val state = _uiState.value
        val shouldTrack = screenVisible &&
            evaluator != null &&
            state.permission == LocationPermissionState.GRANTED
        if (shouldTrack && !state.tracking) {
            _uiState.update { it.copy(tracking = true, arrival = ArrivalStatus.NoFix) }
            tracker.start(trackerListener)
        } else if (!shouldTrack && state.tracking) {
            tracker.stop()
            _uiState.update { it.copy(tracking = false) }
        }
    }

    override fun onCleared() {
        tracker.stop()
        super.onCleared()
    }
}
