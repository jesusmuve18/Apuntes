package com.jesusmuve.etsiitsmartcampus.feature.route

import com.jesusmuve.etsiitsmartcampus.core.model.Building
import com.jesusmuve.etsiitsmartcampus.core.model.EdgeKind
import com.jesusmuve.etsiitsmartcampus.core.model.IndoorNode
import kotlin.math.atan2
import kotlin.math.hypot

/**
 * Convierte posiciones del plano en **rumbos reales** (grados desde el norte, sentido horario),
 * para que la brújula de «¿Voy bien?» indique hacia dónde está de verdad el siguiente punto.
 *
 * Los planos son renders 3D vistos en diagonal desde delante del edificio, no planos cenitales:
 * - La parte de **arriba** de la imagen apunta, en la realidad, al rumbo
 *   [Building.planUpBearingDegrees] (el pico del edificio, ~290°).
 * - La profundidad está **acortada**: un tramo vertical en la imagen es en el suelo
 *   `1 / planDepthFactor` veces más largo. Sin esta corrección, los tramos «hacia arriba» del
 *   plano saldrían torcidos hacia los lados.
 * - Las coordenadas de los nodos están normalizadas (0..1) en una imagen más ancha que alta, así
 *   que la X se multiplica por [Building.planAspectRatio].
 *
 */
object PlanBearing {

    /**
     * Separación mínima entre dos nodos (en unidades de alto de imagen, ya corregida) para que
     * tenga sentido hablar de una dirección. Por debajo, el rumbo no es fiable.
     */
    private const val MIN_SEPARATION = 0.01

    /**
     * Rumbo real para ir de [from] a [to], o `null` si no se puede calcular: nodos en distinta
     * planta (escalera, ascensor) o demasiado juntos en el plano.
     */
    fun between(from: IndoorNode, to: IndoorNode, building: Building): Double? {
        if (from.floorId != to.floorId) return null
        // Derecha en el suelo = derecha en la imagen (corrigiendo la proporción de la imagen).
        val right = (to.planX - from.planX) * building.planAspectRatio
        // Hacia delante en el suelo = hacia arriba en la imagen, deshaciendo el acortamiento.
        val forward = -(to.planY - from.planY) / building.planDepthFactor
        if (hypot(right, forward) < MIN_SEPARATION) return null
        val relativeToPlanUp = Math.toDegrees(atan2(right, forward))
        return normalize(building.planUpBearingDegrees + relativeToPlanUp)
    }

    /**
     * Rumbo de un tramo: el fijado a mano en el JSON ([fixedBearing]) si existe y, si no, el
     * calculado con el plano. Los tramos de escalera y ascensor no tienen rumbo útil: el camino
     * lo marca la propia escalera, así que devuelven `null` y la brújula no se muestra.
     */
    fun forLeg(
        from: IndoorNode,
        to: IndoorNode,
        kind: EdgeKind,
        fixedBearing: Double?,
        building: Building,
    ): Double? = when {
        kind != EdgeKind.WALK -> null
        fixedBearing != null -> normalize(fixedBearing)
        else -> between(from, to, building)
    }

    /** Lleva un ángulo cualquiera a [0, 360). */
    fun normalize(degrees: Double): Double {
        val r = degrees % 360.0
        return if (r < 0.0) r + 360.0 else r
    }
}
