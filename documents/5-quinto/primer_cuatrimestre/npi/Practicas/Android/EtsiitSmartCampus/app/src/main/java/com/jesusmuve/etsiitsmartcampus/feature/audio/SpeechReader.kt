package com.jesusmuve.etsiitsmartcampus.feature.audio

import android.content.Context
import android.speech.tts.TextToSpeech
import android.speech.tts.UtteranceProgressListener
import java.util.Locale

/** Qué está haciendo la lectura en voz alta. */
enum class SpeechState { IDLE, SPEAKING, PAUSED }

/**
 * Lectura en voz alta con el motor `TextToSpeech` de Android («Escuchar»), con **pausa**.
 *
 * Lee el texto que le pasa la pantalla en el idioma de la aplicación (ES/EN). Usa el volumen de
 * contenido multimedia del móvil y no lo cambia. Debe liberarse con [shutdown] al salir de la
 * pantalla. No hay reconocimiento de voz: eso es de la práctica de voz posterior.
 *
 * El motor de Android no sabe pausar a mitad de frase. Por eso el texto se trocea en frases
 * ([SpeechSegments]) y se encola una a una: al pausar se para y se recuerda la frase en curso; al
 * reanudar se vuelve a leer desde esa frase.
 *
 * @param onReadyChanged Se llama cuando el motor termina de iniciarse (`true` si se puede usar).
 * @param onStateChanged Se llama al empezar, pausar o terminar la lectura (puede ser desde otro hilo).
 */
class SpeechReader(
    context: Context,
    private val onReadyChanged: (Boolean) -> Unit = {},
    private val onStateChanged: (SpeechState) -> Unit = {},
) {

    private var ready = false
    private val tts: TextToSpeech = TextToSpeech(context.applicationContext) { status ->
        ready = status == TextToSpeech.SUCCESS
        onReadyChanged(ready)
    }

    private var sentences: List<String> = emptyList()
    private var languageTag = "es"

    /** Índice de la frase que se está leyendo (o por la que se reanuda). */
    @Volatile
    private var currentIndex = 0

    /** Distingue los avisos de una lectura de los de otra anterior ya cancelada. */
    @Volatile
    private var session = 0

    @Volatile
    var state: SpeechState = SpeechState.IDLE
        private set

    init {
        tts.setOnUtteranceProgressListener(
            object : UtteranceProgressListener() {
                override fun onStart(utteranceId: String) {
                    val (id, index) = parse(utteranceId) ?: return
                    if (id != session) return
                    currentIndex = index
                    updateState(SpeechState.SPEAKING)
                }

                override fun onDone(utteranceId: String) {
                    val (id, index) = parse(utteranceId) ?: return
                    if (id != session) return
                    if (index >= sentences.lastIndex) {
                        currentIndex = 0
                        updateState(SpeechState.IDLE)
                    } else {
                        currentIndex = index + 1
                    }
                }

                @Deprecated("Deprecated in Java")
                override fun onError(utteranceId: String) {
                    val (id, _) = parse(utteranceId) ?: return
                    if (id == session) updateState(SpeechState.IDLE)
                }
            },
        )
    }

    /**
     * Lee [text] desde el principio en el idioma [languageTag] (`es` o `en`), interrumpiendo lo
     * que estuviera leyendo.
     *
     * @return `false` si el motor no está listo o no tiene voz para ese idioma.
     */
    fun speak(text: String, languageTag: String): Boolean {
        if (!ready || text.isBlank()) return false
        this.languageTag = languageTag
        sentences = SpeechSegments.split(text)
        return speakFrom(0)
    }

    /** Pausa la lectura; [resume] sigue por la frase en la que se quedó. */
    fun pause() {
        if (state != SpeechState.SPEAKING) return
        session++ // los avisos de la lectura parada ya no cuentan
        tts.stop()
        updateState(SpeechState.PAUSED)
    }

    /** Sigue leyendo tras una pausa. */
    fun resume(): Boolean {
        if (state != SpeechState.PAUSED) return false
        return speakFrom(currentIndex)
    }

    /** Para del todo; la próxima lectura empieza desde el principio. */
    fun stop() {
        session++
        tts.stop()
        currentIndex = 0
        updateState(SpeechState.IDLE)
    }

    fun shutdown() {
        stop()
        tts.shutdown()
    }

    private fun speakFrom(index: Int): Boolean {
        if (!ready || sentences.isEmpty()) return false
        val locale = if (languageTag == "en") Locale.UK else Locale("es", "ES")
        val result = tts.setLanguage(locale)
        if (result == TextToSpeech.LANG_MISSING_DATA || result == TextToSpeech.LANG_NOT_SUPPORTED) return false
        session++
        tts.stop()
        currentIndex = index.coerceIn(0, sentences.lastIndex)
        for (i in currentIndex..sentences.lastIndex) {
            val mode = if (i == currentIndex) TextToSpeech.QUEUE_FLUSH else TextToSpeech.QUEUE_ADD
            tts.speak(sentences[i], mode, null, "$UTTERANCE_PREFIX$session:$i")
        }
        updateState(SpeechState.SPEAKING)
        return true
    }

    private fun updateState(newState: SpeechState) {
        if (state == newState) return
        state = newState
        onStateChanged(newState)
    }

    private fun parse(utteranceId: String): Pair<Int, Int>? {
        if (!utteranceId.startsWith(UTTERANCE_PREFIX)) return null
        val parts = utteranceId.removePrefix(UTTERANCE_PREFIX).split(':')
        val id = parts.getOrNull(0)?.toIntOrNull() ?: return null
        val index = parts.getOrNull(1)?.toIntOrNull() ?: return null
        return id to index
    }

    private companion object {
        const val UTTERANCE_PREFIX = "etsiit_listen_"
    }
}

/** Troceado del texto en frases para poder pausar y reanudar. No depende de Android. */
object SpeechSegments {

    private val sentenceEnd = Regex("(?<=[.!?…])\\s+")

    /** Divide [text] en frases no vacías; si no hay puntos, devuelve el texto entero. */
    fun split(text: String): List<String> =
        text.trim().split(sentenceEnd).map { it.trim() }.filter { it.isNotEmpty() }
}
