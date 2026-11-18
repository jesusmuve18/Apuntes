package com.jesusmuve.etsiitsmartcampus.core.data

import com.jesusmuve.etsiitsmartcampus.core.model.Space

/**
 * Catálogo del campus ya cargado y validado.
 *
 * @property spaces Espacios del campus.
 */
data class LoadedCampusCatalog(
    val spaces: List<Space>,
)
