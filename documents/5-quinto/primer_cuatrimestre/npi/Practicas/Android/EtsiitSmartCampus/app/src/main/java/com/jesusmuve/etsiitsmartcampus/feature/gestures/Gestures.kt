package com.jesusmuve.etsiitsmartcampus.feature.gestures

import kotlin.math.abs
import kotlin.math.atan2
import kotlin.math.max
import kotlin.math.roundToInt

/** El botón y el gesto solicitan el mismo escaneo (sin abrir la cámara aquí). */
enum class ScanTrigger { GESTURE, BUTTON }

/** Se requieren DOS asentimientos completos (subida y regreso cada uno). */
const val G1_REQUIRED_PHASES = 2

/** Gesto G1: se reconoce solo con la inclinación del móvil, en grados. */
data class GestureConfig(
    val armDurationNs: Long = 70_000_000L,            // 70 ms para tomar la referencia
    val riseDegrees: Float = 33f,                    // objetivo ~45º; margen para hacerlo fácil
    val returnToleranceDegrees: Float = 20f,         // volver cerca del origen (+/-20º)
    val minDescentFraction: Float = 0.40f,           // descender de verdad desde el pico
    val maxInitialPitchDegrees: Float = 55f,         // postura inicial bastante permisiva
    val maxAbsolutePitchDegrees: Float = 85f,        // evita activar cerca de vertical
    val maxGravityX: Float = 8.7f,                   // tolerancia lateral amplia
    val maxArmDriftDegrees: Float = 13f,             // evita fijar el origen a mitad de movimiento
    val maxGestureDurationNs: Long = 9_000_000_000L, // 9 s para subir y volver
    val maxWaitForRiseNs: Long = 10_000_000_000L,    // si espera demasiado, re-calibra
    val maxSampleGapNs: Long = 650_000_000L,
) {
    init {
        require(riseDegrees > returnToleranceDegrees)
        require(returnToleranceDegrees >= 0f)
        require(minDescentFraction in 0f..1f)
        require(maxInitialPitchDegrees in 0f..85f)
    }
}

/** Vector de gravedad expresado en las coordenadas del móvil (m/s²). */
data class MotionSample(
    val timestampNs: Long,
    val gravityX: Float,
    val gravityY: Float,
    val gravityZ: Float,
)

/** Reconocedor del gesto: no depende de Android ni del hardware. */
class ScanGestureRecognizer(private val config: GestureConfig = GestureConfig()) {
    private enum class Stage { ARMING, WAITING_FOR_RISE, WAITING_FOR_RETURN, FIRED }

    private var stage = Stage.ARMING
    private var lastTimestampNs = 0L
    private var armStartedNs = 0L
    private var armAngleSum = 0f
    private var armSamples = 0
    private var armFirstAngle = 0f
    private var readySinceNs = 0L
    private var riseSinceNs = 0L
    private var initialAngleDegrees = 0f
    private var peakDifferenceDegrees = 0f
    private var phaseCount = 0

    val completedPhases: Int get() = phaseCount / 2

    fun reset() {
        stage = Stage.ARMING
        lastTimestampNs = 0L
        armStartedNs = 0L
        armAngleSum = 0f
        armSamples = 0
        armFirstAngle = 0f
        readySinceNs = 0L
        riseSinceNs = 0L
        initialAngleDegrees = 0f
        peakDifferenceDegrees = 0f
        phaseCount = 0
    }

    /** 0º = pantalla hacia arriba y horizontal; +45º = parte superior elevada. */
    fun pitchDegrees(sample: MotionSample): Int = measureDegrees(sample).roundToInt()

    /** Diferencia entre el ángulo actual y el inicial. */
    fun differenceDegrees(sample: MotionSample): Int? =
        if (stage == Stage.ARMING) null else (measureDegrees(sample) - initialAngleDegrees).roundToInt()

    /** Devuelve `true` una vez, al completar el segundo regreso al origen. */
    fun accept(sample: MotionSample): Boolean {
        if (stage == Stage.FIRED) return false
        val now = sample.timestampNs
        if (lastTimestampNs != 0L &&
            (now <= lastTimestampNs || now - lastTimestampNs > config.maxSampleGapNs)
        ) {
            reset()
        }
        lastTimestampNs = now

        val angle = measureDegrees(sample)
        val faceUp = sample.gravityZ.isFinite() && sample.gravityZ > 0.8f &&
            angle.isFinite() && abs(angle) <= config.maxAbsolutePitchDegrees &&
            sample.gravityX.isFinite() && abs(sample.gravityX) <= config.maxGravityX
        if (!faceUp) {
            // No permitir que ponerse vertical complete una fase accidentalmente.
            resetKeepingTimestamp(now)
            return false
        }

        when (stage) {
            Stage.ARMING -> {
                if (abs(angle) > config.maxInitialPitchDegrees) {
                    clearArming()
                } else if (armStartedNs == 0L ||
                    abs(angle - armFirstAngle) > config.maxArmDriftDegrees
                ) {
                    startArming(now, angle)
                } else {
                    armAngleSum += angle
                    armSamples++
                    if (now - armStartedNs >= config.armDurationNs) {
                        initialAngleDegrees = armAngleSum / armSamples
                        readySinceNs = now
                        stage = Stage.WAITING_FOR_RISE
                    }
                }
            }
            Stage.WAITING_FOR_RISE -> {
                if (now - readySinceNs > config.maxWaitForRiseNs ||
                    angle - initialAngleDegrees < -config.returnToleranceDegrees
                ) {
                    restartAt(now, angle)
                } else {
                    val difference = angle - initialAngleDegrees
                    if (difference >= config.riseDegrees) {
                        // Cuenta una subida internamente; aún no incrementa la UI.
                        phaseCount += 1
                        peakDifferenceDegrees = difference
                        riseSinceNs = now
                        stage = Stage.WAITING_FOR_RETURN
                    }
                }
            }
            Stage.WAITING_FOR_RETURN -> {
                if (now - riseSinceNs > config.maxGestureDurationNs) {
                    restartAt(now, angle)
                } else {
                    val difference = angle - initialAngleDegrees
                    peakDifferenceDegrees = max(peakDifferenceDegrees, difference)
                    // No contar una falsa vuelta por la amplitud del margen de retorno:
                    // además debe haber descendido un porcentaje del pico medido.
                    val descended = peakDifferenceDegrees - difference
                    if (abs(difference) <= config.returnToleranceDegrees &&
                        descended >= peakDifferenceDegrees * config.minDescentFraction
                    ) {
                        phaseCount += 1
                        if (phaseCount == G1_REQUIRED_PHASES * 2) {
                            stage = Stage.FIRED
                            return true
                        }
                        // Primer ciclo completado: esperar otra subida desde la referencia inicial.
                        readySinceNs = now
                        peakDifferenceDegrees = 0f
                        stage = Stage.WAITING_FOR_RISE
                    }
                }
            }
            Stage.FIRED -> Unit
        }
        return false
    }

    private fun measureDegrees(sample: MotionSample): Float =
        Math.toDegrees(atan2(sample.gravityY, sample.gravityZ).toDouble()).toFloat()

    private fun clearArming() {
        armStartedNs = 0L
        armAngleSum = 0f
        armSamples = 0
    }

    private fun startArming(now: Long, angle: Float) {
        armStartedNs = now
        armFirstAngle = angle
        armAngleSum = angle
        armSamples = 1
    }

    private fun restartAt(now: Long, angle: Float) {
        resetKeepingTimestamp(now)
        if (abs(angle) <= config.maxInitialPitchDegrees) startArming(now, angle)
    }

    private fun resetKeepingTimestamp(now: Long) {
        reset()
        lastTimestampNs = now
    }
}
