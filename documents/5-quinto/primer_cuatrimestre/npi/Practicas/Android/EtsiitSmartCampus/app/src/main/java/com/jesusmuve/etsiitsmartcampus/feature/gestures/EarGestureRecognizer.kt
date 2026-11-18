package com.jesusmuve.etsiitsmartcampus.feature.gestures

import kotlin.math.abs

/**
 * Umbrales de «acercar el móvil a la oreja» (: en un único objeto de configuración).
 *
 * @property minHoldNs Tiempo seguido cerca y en vertical antes de leer en voz alta.
 * @property minVerticalGravity Componente mínima de la gravedad sobre el eje largo del móvil
 * (|gy|, m/s²): el móvil está de pie, como al hablar por teléfono.
 * @property maxFaceGravity Componente máxima sobre el eje perpendicular a la pantalla (|gz|):
 * descarta el móvil tumbado boca abajo sobre una mesa, que también tapa la proximidad.
 */
data class EarGestureConfig(
    val minHoldNs: Long = 350_000_000L,
    val minVerticalGravity: Float = 6.0f,
    val maxFaceGravity: Float = 7.0f,
)

/** Lectura combinada de proximidad y gravedad (: sin `SensorEvent`). */
data class EarSample(
    val timestampNs: Long,
    val near: Boolean,
    val gravityY: Float,
    val gravityZ: Float,
)

/**
 * Reconoce el gesto de acercar el móvil a la oreja para escuchar la información («Escuchar»).
 *
 * Dispara **una vez** cuando el sensor de proximidad dice «cerca» y el móvil está casi vertical
 * durante [EarGestureConfig.minHoldNs]. No vuelve a disparar hasta que se aparta el móvil
 * (proximidad «lejos»), para no repetir la lectura mientras se escucha.
 */
class EarGestureRecognizer(private val config: EarGestureConfig = EarGestureConfig()) {

    private var holdSinceNs: Long? = null
    private var fired = false

    /** Devuelve `true` en el instante en que se reconoce el gesto. */
    fun accept(sample: EarSample): Boolean {
        if (!sample.near) {
            // Al apartar el móvil se rearma: la próxima vez que se acerque volverá a leer.
            holdSinceNs = null
            fired = false
            return false
        }
        val upright = abs(sample.gravityY) >= config.minVerticalGravity &&
            abs(sample.gravityZ) <= config.maxFaceGravity
        if (!upright || fired) {
            holdSinceNs = null
            return false
        }
        val since = holdSinceNs ?: sample.timestampNs.also { holdSinceNs = it }
        if (sample.timestampNs - since >= config.minHoldNs) {
            fired = true
            holdSinceNs = null
            return true
        }
        return false
    }

    fun reset() {
        holdSinceNs = null
        fired = false
    }
}
