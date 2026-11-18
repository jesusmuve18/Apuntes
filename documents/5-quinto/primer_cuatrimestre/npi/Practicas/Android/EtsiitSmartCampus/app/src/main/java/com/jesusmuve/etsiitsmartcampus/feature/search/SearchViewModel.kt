package com.jesusmuve.etsiitsmartcampus.feature.search

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.jesusmuve.etsiitsmartcampus.core.data.CampusCatalogResult
import com.jesusmuve.etsiitsmartcampus.core.data.CampusLookupResult
import com.jesusmuve.etsiitsmartcampus.core.data.CampusRepository
import com.jesusmuve.etsiitsmartcampus.core.data.UserPreferencesRepository
import com.jesusmuve.etsiitsmartcampus.core.model.Space
import com.jesusmuve.etsiitsmartcampus.ui.navigation.CampusRoutes
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.flow.update
import kotlinx.coroutines.launch

/**
 * ViewModel de búsqueda de espacios.
 *
 * @param campusRepository Catálogo de espacios.
 * @param userPreferencesRepository Preferencias del usuario (último destino y nombre).
 */
class SearchViewModel(
    private val campusRepository: CampusRepository,
    private val userPreferencesRepository: UserPreferencesRepository,
) : ViewModel() {

    private val _uiState = MutableStateFlow(createState(query = ""))
    val uiState: StateFlow<SearchUiState> = _uiState.asStateFlow()

    init {
        observeLastDestination()
        observeUserName()
    }

    /** Nombre de la bienvenida, para saludar en el inicio. */
    private fun observeUserName() {
        viewModelScope.launch {
            userPreferencesRepository.observeUserName().collect { name ->
                _uiState.update { current -> current.copy(userName = name) }
            }
        }
    }

    /** Actualiza el texto buscado y la lista. */
    fun onQueryChange(query: String) {
        _uiState.update { current ->
            current.copy(query = query, results = resultsForQuery(query))
        }
    }

    /** Guarda el espacio elegido como último destino y abre su ficha. */
    fun onSpaceSelected(space: Space, onNavigateToSpace: (String) -> Unit) {
        viewModelScope.launch {
            userPreferencesRepository.saveLastDestinationSpaceId(space.id)
        }
        onNavigateToSpace(CampusRoutes.spaceRoute(space.id))
    }

    /**
     * Abre la ficha del último destino.
     */
    fun onLastDestinationSelected(onNavigateToSpace: (String) -> Unit) {
        val destination = _uiState.value.lastDestination ?: return
        onNavigateToSpace(CampusRoutes.spaceRoute(destination.id))
    }

    private fun observeLastDestination() {
        viewModelScope.launch {
            userPreferencesRepository.observeLastDestinationSpaceId().collect { spaceId ->
                val destination = resolveLastDestination(spaceId)
                _uiState.update { current -> current.copy(lastDestination = destination) }
                // El espacio ya no existe en el catálogo: se olvida.
                if (!spaceId.isNullOrBlank() && destination == null) {
                    userPreferencesRepository.clearLastDestinationSpaceId()
                }
            }
        }
    }

    private fun resolveLastDestination(spaceId: String?): Space? {
        if (spaceId.isNullOrBlank()) return null
        return when (val lookup = campusRepository.findSpaceById(spaceId)) {
            is CampusLookupResult.Found -> lookup.space
            CampusLookupResult.NotFound -> null
        }
    }

    private fun createState(query: String): SearchUiState = SearchUiState(
        query = query,
        results = resultsForQuery(query),
        lastDestination = null,
    )

    private fun resultsForQuery(query: String): SearchResults {
        return when (val catalogState = campusRepository.catalogState()) {
            is CampusCatalogResult.Failure -> SearchResults.CatalogError(catalogState.error)
            is CampusCatalogResult.Success -> {
                val spaces = if (query.isBlank()) {
                    campusRepository.getSpaces()
                } else {
                    campusRepository.searchSpaces(query)
                }
                if (query.isNotBlank() && spaces.isEmpty()) {
                    SearchResults.QueryEmpty
                } else {
                    SearchResults.Spaces(spaces)
                }
            }
        }
    }
}
