package com.jesusmuve.etsiitsmartcampus.feature.scanner

import android.app.Activity
import android.content.Context
import android.nfc.NfcAdapter
import android.nfc.Tag
import android.nfc.tech.Ndef

/** Disponibilidad de NFC en el dispositivo. */
enum class NfcAvailability {
    /** El móvil no tiene NFC: solo QR o código manual. */
    UNSUPPORTED,

    /** Hay NFC pero está desactivado en ajustes. */
    DISABLED,

    /** NFC listo para leer etiquetas. */
    READY,
}

/**
 * Lector NFC en modo lector (`NfcAdapter.enableReaderMode`) para etiquetas NDEF sencillas.
 *
 * Solo está activo mientras la pantalla del lector está en primer plano ([enable]/[disable]);
 * el resto de la app no conoce NFC. Lee el primer registro de texto de la etiqueta y lo
 * entrega en bruto a [onPayload]; la validación del ID la hace [TagReader].
 *
 * @param onPayload Se invoca en el hilo principal con el texto leído de la etiqueta.
 */
class NfcTagReader(private val onPayload: (String) -> Unit) {

    /** Activa el modo lector sobre [activity]. Debe llamarse con la actividad en `RESUMED`. */
    fun enable(activity: Activity) {
        val adapter = NfcAdapter.getDefaultAdapter(activity) ?: return
        if (!adapter.isEnabled) return
        val flags = NfcAdapter.FLAG_READER_NFC_A or
            NfcAdapter.FLAG_READER_NFC_B or
            NfcAdapter.FLAG_READER_NFC_F or
            NfcAdapter.FLAG_READER_NFC_V
        adapter.enableReaderMode(
            activity,
            { tag -> readTag(tag)?.let { text -> activity.runOnUiThread { onPayload(text) } } },
            flags,
            null,
        )
    }

    /** Desactiva el modo lector. Seguro de llamar aunque no se haya activado. */
    fun disable(activity: Activity) {
        NfcAdapter.getDefaultAdapter(activity)?.disableReaderMode(activity)
    }

    private fun readTag(tag: Tag): String? {
        // El sistema ya ha leído el mensaje NDEF al descubrir la etiqueta: no hace falta conectar.
        val message = Ndef.get(tag)?.cachedNdefMessage ?: return null
        return message.records.firstNotNullOfOrNull { record ->
            NdefPayloadDecoder.decodeRecord(record.tnf, record.type, record.payload)
        }
    }

    companion object {
        /** Comprueba si hay NFC y si está activado. */
        fun availability(context: Context): NfcAvailability {
            val adapter = NfcAdapter.getDefaultAdapter(context) ?: return NfcAvailability.UNSUPPORTED
            return if (adapter.isEnabled) NfcAvailability.READY else NfcAvailability.DISABLED
        }
    }
}
