package com.jesusmuve.etsiitsmartcampus.feature.search

import com.jesusmuve.etsiitsmartcampus.core.data.CatalogError
import com.jesusmuve.etsiitsmartcampus.core.model.Space

/**
 * Estado inmutable de la pantalla de búsqueda.
 *
 * @property query Texto actual del campo de búsqueda.
 * @property results Lista que se muestra: todos los espacios, los que coinciden, ninguno o error.
 * @property lastDestination Último espacio consultado, o `null` si no hay ninguno.
 */
data class SearchUiState(
    val query: String = "",
    val results: SearchResults = SearchResults.Spaces(items = emptyList()),
    val lastDestination: Space? = null,
    val userName: String? = null,
)

/**
 * Contenido de la lista de la búsqueda.
 */
sealed interface SearchResults {

    /**
     * Espacios a mostrar: todos si no se ha escrito nada, o los que coinciden con la búsqueda.
     */
    data class Spaces(val items: List<Space>) : SearchResults

    /** Se ha escrito algo pero no hay coincidencias. */
    data object QueryEmpty : SearchResults

    /** El catálogo no se ha podido cargar. */
    data class CatalogError(val error: com.jesusmuve.etsiitsmartcampus.core.data.CatalogError) : SearchResults
}
