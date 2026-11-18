package com.jesusmuve.etsiitsmartcampus.core.data

/**
 * Error al cargar, validar o consultar el catálogo. Se devuelve como dato en lugar de lanzar una
 * excepción.
 */
sealed interface CatalogError {

    /**
     * Fallo de lectura/serialización o parseo del JSON de entrada.
     *
     * @property detail Descripción breve del problema, para depurar.
     */
    data class Parse(val detail: String) : CatalogError

    /**
     * Fallo de validación del contenido tras un parseo correcto (IDs, referencias, bilingüismo,
     * invariantes de ruta/destino, etc.).
     *
     * @property reason Motivo legible de la validación fallida.
     * @property field Campo o concepto afectado cuando aplica; `null` si es una regla global.
     */
    data class Validation(val reason: String, val field: String? = null) : CatalogError

    /**
     * Identificador solicitado que no existe en el catálogo o no es resoluble.
     *
     * @property id Identificador que no se encontró.
     */
    data class NotFound(val id: String) : CatalogError
}
