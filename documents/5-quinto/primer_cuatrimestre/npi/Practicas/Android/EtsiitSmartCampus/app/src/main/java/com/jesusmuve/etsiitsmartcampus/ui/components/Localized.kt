package com.jesusmuve.etsiitsmartcampus.ui.components

import androidx.compose.runtime.Composable
import androidx.compose.runtime.ReadOnlyComposable
import androidx.compose.ui.platform.LocalConfiguration

/** `true` si la interfaz está en inglés. */
@Composable
@ReadOnlyComposable
fun isEnglishUi(): Boolean =
    LocalConfiguration.current.locales[0]?.language == "en"

/** Elige el texto en el idioma de la interfaz. */
@Composable
@ReadOnlyComposable
fun localized(es: String, en: String): String = if (isEnglishUi()) en else es
