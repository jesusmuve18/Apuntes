package com.jesusmuve.etsiitsmartcampus.feature.search

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.jesusmuve.etsiitsmartcampus.core.data.CampusLookupResult
import com.jesusmuve.etsiitsmartcampus.core.data.CampusRepository
import com.jesusmuve.etsiitsmartcampus.core.data.UserPreferencesRepository
import com.jesusmuve.etsiitsmartcampus.core.model.Space
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.launch

/** Estado de la ficha de un espacio. */
sealed interface SpaceDetailUiState {

    /**
     * El espacio existe en el catálogo.
     *
     * @property hasIndoorRoute `true` si el espacio tiene ruta interior («Ruta interior hasta aquí»).
     */
    data class Found(val space: Space, val hasIndoorRoute: Boolean) : SpaceDetailUiState

    /** El identificador recibido no existe en el catálogo. */
    data object NotFound : SpaceDetailUiState
}

/** Ficha de un espacio. */
class SpaceDetailViewModel(
    campusRepository: CampusRepository,
    private val userPreferencesRepository: UserPreferencesRepository,
    spaceId: String,
) : ViewModel() {

    private val _uiState = MutableStateFlow(resolve(campusRepository, spaceId))

    /** Estado observable por la pantalla. */
    val uiState: StateFlow<SpaceDetailUiState> = _uiState.asStateFlow()

    init {
        val state = _uiState.value
        if (state is SpaceDetailUiState.Found) {
            viewModelScope.launch {
                userPreferencesRepository.saveLastDestinationSpaceId(state.space.id)
            }
        }
    }

    private fun resolve(repository: CampusRepository, spaceId: String): SpaceDetailUiState =
        when (val lookup = repository.findSpaceById(spaceId)) {
            is CampusLookupResult.Found -> SpaceDetailUiState.Found(
                space = lookup.space,
                hasIndoorRoute = lookup.space.hasRoute,
            )
            CampusLookupResult.NotFound -> SpaceDetailUiState.NotFound
        }
}
