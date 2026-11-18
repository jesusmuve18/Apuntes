package com.jesusmuve.etsiitsmartcampus.feature.scanner

/**
 * Decodificación de payloads NDEF sencillos a texto plano.
 *
 * Formatos aceptados (los que escribe cualquier app tipo "NFC Tools" al grabar "Texto"):
 * - Registro Well-Known Text (`TNF=0x01`, tipo `"T"`).
 * - Registro MIME `text/plain` (`TNF=0x02`).
 *
 * Cualquier otro registro (URI, Smart Poster, aplicaciones…) se ignora: el lector nunca abre
 * URLs ni lanza intents a partir de una etiqueta.
 */
object NdefPayloadDecoder {

    const val TNF_WELL_KNOWN: Short = 0x01
    const val TNF_MIME_MEDIA: Short = 0x02

    private val RTD_TEXT = byteArrayOf('T'.code.toByte())
    private val MIME_TEXT_PLAIN = "text/plain".toByteArray(Charsets.US_ASCII)

    /**
     * Devuelve el texto del registro si es de un tipo aceptado, o `null` en otro caso.
     *
     * @param tnf Type Name Format del registro.
     * @param type Tipo del registro.
     * @param payload Payload del registro.
     */
    fun decodeRecord(tnf: Short, type: ByteArray, payload: ByteArray): String? = when {
        tnf == TNF_WELL_KNOWN && type.contentEquals(RTD_TEXT) -> decodeTextRecordPayload(payload)
        tnf == TNF_MIME_MEDIA && type.contentEquals(MIME_TEXT_PLAIN) -> payload.toString(Charsets.UTF_8)
        else -> null
    }

    /**
     * Decodifica el payload de un registro NDEF Text (NFC Forum RTD Text):
     * byte de estado (bit 7 = UTF-16, bits 0-5 = longitud del código de idioma), código de
     * idioma y, a continuación, el texto.
     */
    fun decodeTextRecordPayload(payload: ByteArray): String? {
        if (payload.isEmpty()) return null
        val status = payload[0].toInt()
        val isUtf16 = status and 0x80 != 0
        val languageLength = status and 0x3F
        val textStart = 1 + languageLength
        if (textStart > payload.size) return null
        val charset = if (isUtf16) Charsets.UTF_16 else Charsets.UTF_8
        return String(payload, textStart, payload.size - textStart, charset)
    }
}
