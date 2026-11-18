package com.jesusmuve.etsiitsmartcampus.core.model

/**
 * Punto identificado mediante QR, NFC o código/confirmación manual.
 *
 * @property id Identificador del punto.
 * @property source Origen de la identificación ([ScanSource.QR], [ScanSource.NFC] o
 * [ScanSource.MANUAL]).
 */
data class IdentifiedPoint(
    val id: String,
    val source: ScanSource,
)
