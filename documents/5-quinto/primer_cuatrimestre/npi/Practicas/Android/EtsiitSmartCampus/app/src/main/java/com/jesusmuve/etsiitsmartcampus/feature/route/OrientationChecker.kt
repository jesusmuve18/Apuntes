package com.jesusmuve.etsiitsmartcampus.feature.route

import kotlin.math.abs
import kotlin.math.atan2
import kotlin.math.cos
import kotlin.math.sin

/** Resultado de «¿Voy bien?». La interfaz siempre lo acompaña de texto. */
enum class OrientationResult {
    /** El móvil apunta aproximadamente hacia donde está el siguiente punto. */
    ALIGNED,

    /** El móvil apunta en otra dirección: hay que girarse. */
    NOT_ALIGNED,

    /** No se puede comprobar: tramo sin rumbo, sin sensor o con lectura poco fiable. */
    UNAVAILABLE,
}

/** Hacia dónde hay que girar para encarar el siguiente punto. */
enum class TurnHint { LEFT, RIGHT, TURN_AROUND }

/**
 * Umbrales de «¿Voy bien?»: se ajustan con pruebas en el móvil real y no se presentan
 * como valores universales.
 *
 * @property toleranceDegrees Diferencia máxima entre hacia dónde apunta el móvil y el rumbo del
 * tramo para darlo por bueno. Pequeña a propósito (20°): con más margen, «vas bien» dejaría de
 * significar algo en un pasillo con varias salidas.
 * @property turnAroundDegrees A partir de esta diferencia se pide «date la vuelta» en lugar de girar.
 */
data class OrientationConfig(
    val toleranceDegrees: Float = 20f,
    val turnAroundDegrees: Float = 150f,
) {
    init {
        require(toleranceDegrees in 0f..180f)
        require(turnAroundDegrees in toleranceDegrees..180f)
    }
}

/**
 * Comprobación «¿Voy bien?» (//).
 *
 * Compara el **rumbo real del tramo** (calculado con las posiciones de los dos puntos en el plano,
 * ver [PlanBearing]) con **hacia dónde apunta el móvil** (brújula, ya corregida al norte
 * geográfico). Así la flecha señala hacia dónde está de verdad el siguiente punto, como en la vida
 * real, en lugar de dar por buena cualquier dirección en la que se pulse «Calibrar».
 *
 * No sabe dónde está el usuario: solo compara direcciones, que llegan ya calculadas.
 */
class OrientationChecker(private val config: OrientationConfig = OrientationConfig()) {

    val toleranceDegrees: Float get() = config.toleranceDegrees

    /**
     * Compara [headingDegrees] (hacia dónde apunta el móvil) con [targetDegrees] (rumbo del tramo).
     */
    fun check(targetDegrees: Float?, headingDegrees: Float?, reliable: Boolean): OrientationResult {
        val deviation = signedDeviationDegrees(targetDegrees, headingDegrees)
        if (deviation == null || !reliable) return OrientationResult.UNAVAILABLE
        return if (abs(deviation) <= config.toleranceDegrees) {
            OrientationResult.ALIGNED
        } else {
            OrientationResult.NOT_ALIGNED
        }
    }

    /**
     * Desviación **con signo** del móvil respecto al rumbo del tramo, en (-180, 180]: positiva si el
     * móvil apunta a la derecha de donde hay que ir (hay que girar a la izquierda) y negativa si
     * apunta a la izquierda. `null` si falta alguno de los dos rumbos.
     */
    fun signedDeviationDegrees(targetDegrees: Float?, headingDegrees: Float?): Float? {
        if (targetDegrees == null || headingDegrees == null) return null
        if (!targetDegrees.isFinite() || !headingDegrees.isFinite()) return null
        val d = normalize(headingDegrees) - normalize(targetDegrees)
        return when {
            d > 180f -> d - 360f
            d <= -180f -> d + 360f
            else -> d
        }
    }

    /** Hacia dónde girar para encarar el siguiente punto, o `null` si ya se va bien. */
    fun turnHint(targetDegrees: Float?, headingDegrees: Float?): TurnHint? {
        val d = signedDeviationDegrees(targetDegrees, headingDegrees) ?: return null
        return when {
            abs(d) <= config.toleranceDegrees -> null
            abs(d) >= config.turnAroundDegrees -> TurnHint.TURN_AROUND
            d > 0f -> TurnHint.LEFT
            else -> TurnHint.RIGHT
        }
    }

    companion object {
        /** Lleva un ángulo cualquiera a [0, 360). */
        fun normalize(degrees: Float): Float {
            val r = degrees % 360f
            return if (r < 0f) r + 360f else r
        }
    }
}

/**
 * Suaviza el rumbo de la brújula para que la flecha no tiemble.
 *
 * Promedia en forma de vector (seno y coseno), no el número del ángulo: así el paso por el norte
 * (359° → 0°) no produce saltos raros. [factor] entre 0 y 1: cuanto más pequeño, más suave y lento.
 */
class HeadingSmoother(private val factor: Float = 0.25f) {

    private var sinSum = 0.0
    private var cosSum = 0.0
    private var initialized = false

    init {
        require(factor in 0f..1f)
    }

    /** Añade una lectura y devuelve el rumbo suavizado en [0, 360). */
    fun add(headingDegrees: Float): Float {
        val rad = Math.toRadians(headingDegrees.toDouble())
        if (!initialized) {
            sinSum = sin(rad)
            cosSum = cos(rad)
            initialized = true
        } else {
            sinSum += factor * (sin(rad) - sinSum)
            cosSum += factor * (cos(rad) - cosSum)
        }
        return OrientationChecker.normalize(Math.toDegrees(atan2(sinSum, cosSum)).toFloat())
    }

    /** Olvida las lecturas anteriores (al volver a calibrar). */
    fun reset() {
        initialized = false
    }
}
