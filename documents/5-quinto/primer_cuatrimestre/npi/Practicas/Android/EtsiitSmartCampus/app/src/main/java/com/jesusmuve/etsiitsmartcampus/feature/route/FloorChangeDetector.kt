package com.jesusmuve.etsiitsmartcampus.feature.route

import kotlin.math.abs
import kotlin.math.max
import kotlin.math.pow
import kotlin.math.roundToInt
import kotlin.math.sign

/** Qué se sabe del cambio de planta en un tramo de escalera o ascensor. */
enum class FloorChangeStatus {
    /** Aún se está tomando la presión de referencia. */
    CALIBRATING,

    /** Referencia tomada; todavía no se nota cambio de altura. */
    WAITING,

    /** Se nota que el usuario sube o baja en la dirección correcta, pero aún no ha llegado. */
    IN_PROGRESS,

    /** La altura ganada o perdida corresponde a la planta de destino. */
    REACHED,

    /** Se ha cambiado de altura en sentido contrario al de la ruta. */
    WRONG_DIRECTION,
}

/**
 * Estado del barómetro en un momento dado.
 *
 * @property metersChanged Metros subidos (positivo) o bajados (negativo) desde el inicio del tramo.
 * @property floorsChanged [metersChanged] redondeado a plantas.
 */
data class FloorChangeReading(
    val status: FloorChangeStatus,
    val metersChanged: Double,
    val floorsChanged: Int,
)

/**
 * Detecta un cambio de planta con el **barómetro** (`TYPE_PRESSURE`): al subir, la presión del
 * aire baja (unos 0,12 hPa por metro cerca del nivel del mar).
 *
 * Funcionamiento:
 * 1. Al empezar el tramo se promedian las primeras [baselineSamples] lecturas como referencia.
 * 2. Cada lectura nueva se suaviza (el barómetro tiene ruido de unas décimas de metro) y se pasa a
 *    diferencia de altura con la fórmula barométrica internacional.
 * 3. Se compara con lo esperado: `plantas a subir × altura de planta`. Basta con alcanzar
 *    [reachFraction] de esa altura para dar la planta por alcanzada (los descansillos y la
 *    deriva del sensor hacen que nunca cuadre exacto).
 *
 * Nunca hace avanzar la ruta por sí solo: la pantalla avisa y el usuario confirma.
 *
 * @param expectedLevels Plantas a subir (positivo) o bajar (negativo) en el tramo.
 * @param floorHeightMeters Altura aproximada entre plantas.
 */
class FloorChangeDetector(
    private val expectedLevels: Int,
    private val floorHeightMeters: Double,
    private val baselineSamples: Int = DEFAULT_BASELINE_SAMPLES,
    private val smoothing: Double = DEFAULT_SMOOTHING,
    private val reachFraction: Double = DEFAULT_REACH_FRACTION,
) {
    init {
        require(floorHeightMeters > 0.0)
        require(baselineSamples >= 1)
        require(smoothing in 0.0..1.0)
        require(reachFraction in 0.0..1.0)
    }

    private var baselineSum = 0.0
    private var baselineCount = 0
    private var baselineHpa: Double? = null
    private var smoothedHpa: Double? = null

    /** Añade una lectura de presión (hPa) y devuelve el estado actualizado. */
    fun onPressure(hPa: Float): FloorChangeReading {
        if (!hPa.isFinite() || hPa <= 0f) return reading(FloorChangeStatus.CALIBRATING, 0.0)
        val baseline = baselineHpa
        if (baseline == null) {
            baselineSum += hPa
            baselineCount++
            if (baselineCount >= baselineSamples) {
                val average = baselineSum / baselineCount
                baselineHpa = average
                smoothedHpa = average
                return reading(FloorChangeStatus.WAITING, 0.0)
            }
            return reading(FloorChangeStatus.CALIBRATING, 0.0)
        }
        val previous = smoothedHpa ?: hPa.toDouble()
        val smoothed = previous + smoothing * (hPa - previous)
        smoothedHpa = smoothed
        val meters = altitudeDifferenceMeters(baseline, smoothed)
        return reading(classify(meters), meters)
    }

    /** Vuelve a tomar la referencia (p. ej. si el usuario empieza el tramo de nuevo). */
    fun reset() {
        baselineSum = 0.0
        baselineCount = 0
        baselineHpa = null
        smoothedHpa = null
    }

    private fun classify(meters: Double): FloorChangeStatus {
        val expectedMeters = expectedLevels * floorHeightMeters
        val goingRightWay = expectedLevels != 0 && sign(meters) == sign(expectedMeters)
        return when {
            goingRightWay && abs(meters) >= max(MIN_REACH_METERS, reachFraction * abs(expectedMeters)) ->
                FloorChangeStatus.REACHED
            !goingRightWay && abs(meters) >= max(MIN_REACH_METERS, reachFraction * floorHeightMeters) ->
                FloorChangeStatus.WRONG_DIRECTION
            goingRightWay && abs(meters) >= MOVING_METERS -> FloorChangeStatus.IN_PROGRESS
            else -> FloorChangeStatus.WAITING
        }
    }

    private fun reading(status: FloorChangeStatus, meters: Double) = FloorChangeReading(
        status = status,
        metersChanged = meters,
        floorsChanged = (meters / floorHeightMeters).roundToInt(),
    )

    companion object {
        /** Lecturas para la referencia (~1 s con el barómetro a ritmo normal). */
        const val DEFAULT_BASELINE_SAMPLES = 5

        /** Suavizado de la presión: filtra el ruido sin retrasar demasiado el aviso. */
        const val DEFAULT_SMOOTHING = 0.2

        /** Fracción de la altura esperada que basta para dar la planta por alcanzada. */
        const val DEFAULT_REACH_FRACTION = 0.6

        /** Por debajo de este cambio no se avisa de nada (ruido, abrir una puerta, el clima…). */
        const val MIN_REACH_METERS = 2.0

        /** Cambio a partir del cual se considera que el usuario ya está subiendo o bajando. */
        const val MOVING_METERS = 1.0

        /**
         * Diferencia de altura (m) entre dos presiones, con la fórmula barométrica internacional
         * (la misma que `SensorManager.getAltitude`): positiva si [currentHpa] está más alto.
         */
        fun altitudeDifferenceMeters(referenceHpa: Double, currentHpa: Double): Double =
            44_330.0 * (1.0 - (currentHpa / referenceHpa).pow(1.0 / 5.255))
    }
}
