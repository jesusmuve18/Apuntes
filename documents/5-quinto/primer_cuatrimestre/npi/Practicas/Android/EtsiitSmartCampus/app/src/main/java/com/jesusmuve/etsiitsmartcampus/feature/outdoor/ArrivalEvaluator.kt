package com.jesusmuve.etsiitsmartcampus.feature.outdoor

import kotlin.math.asin
import kotlin.math.cos
import kotlin.math.min
import kotlin.math.pow
import kotlin.math.sin
import kotlin.math.sqrt

/**
 * Posición recibida del GPS, independiente de `android.location.Location`.
 *
 * @property accuracyMeters Radio de incertidumbre que informa el sistema (68 % de confianza).
 */
data class GeoFix(
    val latitude: Double,
    val longitude: Double,
    val accuracyMeters: Float,
)

/** Situación del usuario respecto a la entrada de la ETSIIT según el GPS. */
sealed interface ArrivalStatus {
    /** Aún no hay ninguna posición. */
    data object NoFix : ArrivalStatus

    /** Hay posición, pero demasiado imprecisa para decidir (p. ej. ubicación aproximada). */
    data class LowAccuracy(val distanceMeters: Double, val accuracyMeters: Float) : ArrivalStatus

    /** Lejos de la entrada. */
    data class Away(val distanceMeters: Double, val accuracyMeters: Float) : ArrivalStatus

    /** Junto a la entrada: se ofrece empezar la ruta interior. */
    data class AtEntrance(val distanceMeters: Double, val accuracyMeters: Float) : ArrivalStatus

    /**
     * Ya había llegado a la entrada y después la señal se ha vuelto imprecisa, que es lo que
     * ocurre al entrar en un edificio. **No es una confirmación**: es una suposición razonable
     * que la pantalla presenta como tal y que se confirma escaneando el punto de la entrada
     *.
     */
    data class ProbablyInside(val distanceMeters: Double, val accuracyMeters: Float) : ArrivalStatus

    /** `true` si el GPS ya ha situado al usuario en la entrada (o, probablemente, dentro). */
    val hasArrived: Boolean get() = this is AtEntrance || this is ProbablyInside
}

/**
 * Decide si el usuario está junto a la entrada a partir de la posición GPS.
 *
 * Criterios (los valores se ajustan con pruebas en el móvil real):
 * - Si la precisión es peor que [maxAccuracyMeters], no se decide ([ArrivalStatus.LowAccuracy]).
 * - Se considera «en la entrada» si la distancia es menor que [arrivalRadiusMeters].
 * - **Histéresis:** una vez en la entrada, solo se sale si la distancia supera
 *   `arrivalRadiusMeters * exitFactor`. Así el aviso no parpadea cuando el GPS oscila unos
 *   metros alrededor del límite.
 * - **Entrada al edificio:** dentro de un edificio el GPS pierde satélites y la precisión
 *   empeora. Si el usuario ya estaba en la entrada y la señal se vuelve imprecisa sin que la
 *   posición se aleje del radio de salida, se pasa a [ArrivalStatus.ProbablyInside] en lugar de
 *   perder la llegada. El GPS no puede distinguir con fiabilidad dentro de fuera; la
 *   confirmación real es escanear el punto de control de la entrada.
 *
 */
class ArrivalEvaluator(
    private val entranceLatitude: Double,
    private val entranceLongitude: Double,
    private val arrivalRadiusMeters: Double,
    private val maxAccuracyMeters: Float = DEFAULT_MAX_ACCURACY_METERS,
    private val exitFactor: Double = DEFAULT_EXIT_FACTOR,
) {

    /**
     * Calcula el nuevo estado a partir de [fix] y del estado anterior (para la histéresis).
     */
    fun evaluate(fix: GeoFix, previous: ArrivalStatus): ArrivalStatus {
        val distance = GeoMath.distanceMeters(
            fix.latitude, fix.longitude, entranceLatitude, entranceLongitude,
        )
        val exitLimit = arrivalRadiusMeters * exitFactor
        if (fix.accuracyMeters > maxAccuracyMeters) {
            // Señal degradada justo después de llegar: lo normal al cruzar la puerta.
            return if (previous.hasArrived && distance <= exitLimit) {
                ArrivalStatus.ProbablyInside(distance, fix.accuracyMeters)
            } else {
                ArrivalStatus.LowAccuracy(distance, fix.accuracyMeters)
            }
        }
        val limit = if (previous.hasArrived) exitLimit else arrivalRadiusMeters
        return if (distance <= limit) {
            ArrivalStatus.AtEntrance(distance, fix.accuracyMeters)
        } else {
            ArrivalStatus.Away(distance, fix.accuracyMeters)
        }
    }

    companion object {
        /**
         * Precisión mínima aceptable: con más incertidumbre no se puede hablar de «la puerta».
         * Va a la par del radio de llegada (20 m): en exterior el GPS suele dar 5–15 m.
         */
        const val DEFAULT_MAX_ACCURACY_METERS = 30f

        /** Margen de salida de la histéresis (50 % más que el radio de llegada). */
        const val DEFAULT_EXIT_FACTOR = 1.5
    }
}

/** Utilidades geográficas puras. */
object GeoMath {

    /** Radio medio de la Tierra (m), el mismo que usan las fórmulas habituales de haversine. */
    private const val EARTH_RADIUS_METERS = 6_371_008.8

    /**
     * Distancia en metros entre dos puntos por la fórmula de haversine (suficiente a la escala
     * de un campus: el error es muy inferior a la precisión del GPS).
     */
    fun distanceMeters(lat1: Double, lon1: Double, lat2: Double, lon2: Double): Double {
        val phi1 = Math.toRadians(lat1)
        val phi2 = Math.toRadians(lat2)
        val dPhi = Math.toRadians(lat2 - lat1)
        val dLambda = Math.toRadians(lon2 - lon1)
        val a = sin(dPhi / 2).pow(2) + cos(phi1) * cos(phi2) * sin(dLambda / 2).pow(2)
        return 2 * EARTH_RADIUS_METERS * asin(min(1.0, sqrt(a)))
    }
}
