package com.jesusmuve.etsiitsmartcampus.core.data

/**
 * Resultado de cargar o consultar el catálogo: éxito con un valor o fallo con su error.
 *
 * @param T Tipo del valor producido cuando la operación tiene éxito.
 */
sealed interface CampusCatalogResult<out T> {

    /**
     * Operación completada con éxito.
     *
     * @property value Valor resultante válido.
     */
    data class Success<T>(val value: T) : CampusCatalogResult<T>

    /**
     * La operación ha fallado.
     *
     * @property error Causa del fallo.
     */
    data class Failure(val error: CatalogError) : CampusCatalogResult<Nothing>
}
