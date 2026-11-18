package com.jesusmuve.etsiitsmartcampus.core.model

/** Etiquetas de idioma soportadas por la app: español e inglés. */
object LanguageTag {
    const val SPANISH = "es"
    const val ENGLISH = "en"

    /**
     * Convierte un idioma cualquiera (p. ej. `es-ES`) en uno de los de la app.
     *
     * @param tag Tag de idioma, p. ej. `es`, `es-ES`, `en`, `en-US`.
     * @return [ENGLISH] o [SPANISH].
     */
    fun normalize(tag: String?): String {
        val normalized = tag?.trim()?.lowercase().orEmpty()
        return if (normalized.startsWith(ENGLISH)) ENGLISH else SPANISH
    }
}
