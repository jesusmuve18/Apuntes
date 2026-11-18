package com.jesusmuve.etsiitsmartcampus.feature.scanner

import com.jesusmuve.etsiitsmartcampus.core.model.IdentifiedPoint
import com.jesusmuve.etsiitsmartcampus.core.model.ScanSource

/**
 * Catálogo de IDs que el lector reconoce (nodos del grafo interior y espacios de `campus.json`).
 */
fun interface PointCatalog {
    /** `true` si [id] (ya normalizado) es un punto conocido del campus. */
    suspend fun contains(id: String): Boolean
}

/**
 * Resultado de intentar identificar un punto a partir del contenido leído de un QR, una
 * etiqueta NFC o un código introducido a mano.
 */
sealed interface TagReadResult {

    /** El contenido es un ID con formato válido y existe en el catálogo local. */
    data class Identified(val point: IdentifiedPoint) : TagReadResult

    /**
     * El contenido no tiene forma de ID (vacío, demasiado largo, URL, caracteres no permitidos…).
     * No se guarda ni se muestra el contenido: solo se informa del rechazo.
     */
    data object Malformed : TagReadResult

    /** El contenido tiene forma de ID pero no existe en el catálogo ("QR no reconocido"). */
    data object Unknown : TagReadResult
}

/**
 * Lector unificado de etiquetas: QR, NFC y código manual comparten el mismo
 * identificador lógico y pasan por esta misma validación.
 *
 * @param catalog Catálogo local contra el que se validan los IDs.
 */
class TagReader(private val catalog: PointCatalog) {

    /**
     * Identifica el punto correspondiente a [rawPayload].
     *
     * @param rawPayload Texto leído tal cual (valor del QR, texto NDEF o código manual).
     * @param source Origen de la lectura.
     */
    suspend fun identify(rawPayload: String, source: ScanSource): TagReadResult {
        val id = normalizeId(rawPayload) ?: return TagReadResult.Malformed
        return if (catalog.contains(id)) {
            TagReadResult.Identified(IdentifiedPoint(id, source))
        } else {
            TagReadResult.Unknown
        }
    }

    companion object {
        /** Longitud máxima de un ID; lo que sea más largo se rechaza. */
        const val MAX_ID_LENGTH = 64

        private val ID_PATTERN = Regex("^[A-Za-z0-9_.-]+$")

        /**
         * Normaliza el contenido leído a un candidato a ID, o devuelve `null` si no tiene forma
         * de ID. Aplica `trim()` (igual que el repositorio) y solo admite letras ASCII, dígitos,
         * `_`, `-` y `.`, lo que descarta URLs, espacios internos, saltos de línea, etc.
         */
        fun normalizeId(rawPayload: String): String? {
            if (rawPayload.length > MAX_ID_LENGTH * 4) return null
            val trimmed = rawPayload.trim()
            if (trimmed.isEmpty() || trimmed.length > MAX_ID_LENGTH) return null
            if (!ID_PATTERN.matches(trimmed)) return null
            return trimmed
        }
    }
}
