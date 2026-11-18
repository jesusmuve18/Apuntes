package com.jesusmuve.etsiitsmartcampus.core.data

import com.jesusmuve.etsiitsmartcampus.core.model.Space

/** Resultado de buscar un espacio por su identificador.
 */
sealed interface CampusLookupResult {

    /**
     * El identificador solicitado se resolvió a un espacio del catálogo.
     *
     * @property space Espacio encontrado.
     */
    data class Found(val space: Space) : CampusLookupResult

    /** El identificador solicitado no existe en el catálogo. */
    data object NotFound : CampusLookupResult
}
