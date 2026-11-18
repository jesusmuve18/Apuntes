package com.jesusmuve.etsiitsmartcampus.core.model

import kotlinx.serialization.Serializable

/**
 * Espacio del campus registrado en el catálogo local.
 *
 * @property id Identificador del espacio; es también el contenido de su QR o etiqueta NFC.
 * @property name Nombre bilingüe del espacio.
 * @property floor Planta del espacio (entero; p. ej. 0 para planta baja).
 * @property description Descripción bilingüe del espacio.
 * @property type Tipo de espacio (p. ej. `classroom` o `laboratory`).
 * @property hasRoute Indica si este espacio ofrece la ruta de demostración.
 */
@Serializable
data class Space(
    val id: String,
    val name: LocalizedText,
    val floor: Int,
    val description: LocalizedText,
    val type: String,
    val hasRoute: Boolean,
)
