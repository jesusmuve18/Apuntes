package com.jesusmuve.etsiitsmartcampus.core.model

import kotlinx.serialization.Serializable

/**
 * Texto en español e inglés.
 *
 * @property es Texto en español.
 * @property en Texto en inglés.
 */
@Serializable
data class LocalizedText(
    val es: String,
    val en: String,
)

/**
 * Devuelve el texto en el idioma indicado.
 *
 * @param languageTag Idioma, p. ej. `es`, `en` o `es-ES`.
 * @return [es] si el idioma normalizado es español; [en] si es inglés.
 */
fun LocalizedText.resolve(languageTag: String): String =
    when (LanguageTag.normalize(languageTag)) {
        LanguageTag.ENGLISH -> en
        else -> es
    }
