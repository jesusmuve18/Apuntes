package com.jesusmuve.etsiitsmartcampus.core.model

/** Origen de identificación de un punto del campus. */
enum class ScanSource {
    /** Identificador leído mediante un código QR. */
    QR,

    /** Identificador leído mediante una etiqueta NFC (NDEF). */
    NFC,

    /** Identificador introducido o confirmado manualmente por el usuario. */
    MANUAL,
}
