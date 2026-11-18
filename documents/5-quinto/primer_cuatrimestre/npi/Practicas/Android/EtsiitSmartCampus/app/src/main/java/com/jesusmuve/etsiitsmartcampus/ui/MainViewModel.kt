package com.jesusmuve.etsiitsmartcampus.ui

import androidx.lifecycle.ViewModel
import com.jesusmuve.etsiitsmartcampus.ui.navigation.CampusRoutes
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow

/**
 * Estado inmutable de la pantalla principal.
 *
 * @property currentDestination Pantalla actual (p. ej. [CampusRoutes.SEARCH]).
 */
data class MainUiState(
    val currentDestination: String,
)

/** ViewModel de la actividad principal: guarda en qué pantalla está el usuario.
 */
class MainViewModel : ViewModel() {

    /** Estado interno, modificable solo desde esta clase. */
    private val _uiState = MutableStateFlow(
        MainUiState(currentDestination = CampusRoutes.SEARCH),
    )

    /** Estado observable por la interfaz: destino actual de navegación. */
    val uiState: StateFlow<MainUiState> = _uiState.asStateFlow()

    /**
     * Actualiza el destino actual cuando el `NavHost` notifica un cambio de pantalla.
     *
     * @param route Identificador de la ruta a la que se ha navegado (p. ej.
     * [CampusRoutes.SETTINGS]).
     */
    fun onDestinationChanged(route: String) {
        _uiState.value = MainUiState(currentDestination = route)
    }
}
