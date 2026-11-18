package com.jesusmuve.etsiitsmartcampus.core.model

import kotlinx.serialization.Serializable

/**
 * Mapa interior de la ETSIIT como **grafo de nodos** (`assets/indoor/graph.json`).
 *
 * - Un [IndoorNode] es un punto de referencia reconocible del edificio (entrada, escalera,
 *   pasillo, aula) con su foto y su posición aproximada en el plano de su planta.
 * - Un [IndoorEdge] une dos nodos y lleva la instrucción que se muestra al usuario para ir de
 *   uno a otro, en español e inglés.
 *
 * El `id` de cada nodo es también el contenido exacto de su etiqueta QR/NFC,
 * de modo que el lector de etiquetas y la ruta comparten identificadores.
 *
 * @property version Versión del formato del fichero, para detectar ficheros antiguos.
 * @property building Datos del edificio, incluida la entrada usada por la navegación exterior.
 * @property floors Plantas que aparecen en la ruta, cada una con su plano.
 * @property nodes Nodos del grafo.
 * @property edges Conexiones entre nodos, con la instrucción de cada sentido.
 * @property routes Rutas de muestra fijas (ida y vuelta, por escalera o por ascensor).
 */
@Serializable
data class IndoorMap(
    val version: Int,
    val building: Building,
    val floors: List<Floor>,
    val nodes: List<IndoorNode>,
    val edges: List<IndoorEdge>,
    val routes: List<DemoRoute> = emptyList(),
) {
    /** Busca un nodo por su identificador; `null` si no existe. */
    fun node(id: String): IndoorNode? = nodes.firstOrNull { it.id == id }

    /**
     * Tramo que lleva de [fromId] a [toId], en el sentido en que se recorre; `null` si no hay
     * ninguno (o solo existe en el otro sentido).
     */
    fun connection(fromId: String, toId: String): DirectedEdge? {
        edges.firstOrNull { it.from == fromId && it.to == toId }?.let { return DirectedEdge(it, reversed = false) }
        return edges.firstOrNull { it.from == toId && it.to == fromId && it.isBidirectional }
            ?.let { DirectedEdge(it, reversed = true) }
    }

    /** Busca una planta por su identificador; `null` si no existe. */
    fun floor(id: String): Floor? = floors.firstOrNull { it.id == id }
}

/**
 * Datos del edificio.
 *
 * @property entranceLatitude Latitud de la entrada principal (coordenadas publicadas por la
 * ETSIIT); se usa para abrir Google Maps y para comprobar con el GPS si el usuario ha llegado.
 * @property arrivalRadiusMeters Distancia a la entrada por debajo de la cual se ofrece empezar la
 * ruta interior. Se ajusta con pruebas reales.
 * @property entranceNodeId Nodo del grafo que corresponde a la entrada: origen de la ruta interior
 * cuando el usuario llega desde la navegación exterior.
 * @property defaultDestinationNodeId Destino que se propone por defecto (aula de demostración).
 * @property provisional `true` mientras las posiciones y las fotos sean de prueba: la interfaz lo
 * indica para no presentarlas como datos oficiales.
 * @property mapsQuery Texto con el que Google Maps busca el destino (nombre del centro). Si se
 * usaran solo las coordenadas, Maps mostraría el lugar con nombre más cercano (p. ej. el comedor
 * vecino) en lugar de la ETSIIT. `null` = usar las coordenadas.
 * @property planUpBearingDegrees Rumbo real (grados desde el norte geográfico, sentido horario)
 * hacia el que apunta la parte de **arriba** de las imágenes de los planos. En los renders del
 * modelo 3D, arriba queda el pico del edificio, que apunta a unos 290° (medido en la entrada
 * comparando la brújula con la dirección real del primer tramo). Con él se pasan
 * las posiciones del plano a direcciones reales para la brújula de «¿Voy bien?».
 * @property planDepthFactor Factor de acortamiento de la profundidad: los planos son vistas 3D
 * inclinadas, así que una distancia «hacia arriba» en la imagen mide en el suelo `1 / factor`
 * veces más. Medido con el bloque cuadrado de la planta baja (0,67).
 * @property planAspectRatio Proporción ancho / alto de las imágenes de los planos, para que una
 * misma distancia en X y en Y normalizadas valga lo mismo.
 * @property floorHeightMeters Altura aproximada entre plantas; el barómetro la usa para estimar
 * cuántas plantas se han subido o bajado.
 * @property mapsPlaceId Identificador del lugar en Google Maps («place ID»). Si existe, Maps abre
 * exactamente esa ficha, en el idioma del móvil; tiene prioridad sobre [mapsQuery].
 */
@Serializable
data class Building(
    val nameEs: String,
    val nameEn: String,
    val addressEs: String,
    val addressEn: String,
    val entranceLatitude: Double,
    val entranceLongitude: Double,
    val arrivalRadiusMeters: Double,
    val entranceNodeId: String,
    val defaultDestinationNodeId: String,
    val provisional: Boolean = true,
    val mapsQuery: String? = null,
    val mapsPlaceId: String? = null,
    val planUpBearingDegrees: Double = DEFAULT_PLAN_UP_BEARING,
    val planDepthFactor: Double = DEFAULT_PLAN_DEPTH_FACTOR,
    val planAspectRatio: Double = DEFAULT_PLAN_ASPECT_RATIO,
    val floorHeightMeters: Double = DEFAULT_FLOOR_HEIGHT_METERS,
) {
    companion object {
        /**
         * Rumbo (grados desde el norte, sentido horario) hacia el que mira la parte de arriba de los
         * planos: unos 290° (el pico del edificio apunta al noroeste). Medido en la entrada con la
         * brújula del móvil el 13/11/2026.
         */
        const val DEFAULT_PLAN_UP_BEARING = 290.0

        /** Acortamiento vertical de los planos por la perspectiva del render 3D. */
        const val DEFAULT_PLAN_DEPTH_FACTOR = 0.67

        /** Ancho / alto de las imágenes de los planos (1024 × 768). */
        const val DEFAULT_PLAN_ASPECT_RATIO = 4.0 / 3.0

        /** Altura aproximada entre plantas, para el barómetro. */
        const val DEFAULT_FLOOR_HEIGHT_METERS = 4.0
    }
}

/**
 * Planta del edificio.
 *
 * @property planAsset Ruta dentro de `assets/` de la imagen del plano de la planta.
 * @property level Número de planta (0 = baja). El barómetro lo usa para saber cuántas plantas
 * hay que subir o bajar en un tramo.
 */
@Serializable
data class Floor(
    val id: String,
    val nameEs: String,
    val nameEn: String,
    val planAsset: String,
    val level: Int = 0,
)

/**
 * Nodo del grafo: un punto de referencia físico.
 *
 * @property photoAsset Ruta dentro de `assets/` de la foto del lugar; puede no existir todavía
 * (la interfaz muestra entonces un aviso de «foto pendiente»).
 * @property planX Posición horizontal en el plano de su planta, normalizada entre 0 y 1.
 * @property planY Posición vertical en el plano de su planta, normalizada entre 0 y 1.
 */
@Serializable
data class IndoorNode(
    val id: String,
    val floorId: String,
    val nameEs: String,
    val nameEn: String,
    val descriptionEs: String,
    val descriptionEn: String,
    val photoAsset: String? = null,
    val planX: Float,
    val planY: Float,
)

/** Tipo de tramo: andando, por escalera o en ascensor. */
@Serializable
enum class EdgeKind { WALK, STAIRS, ELEVATOR }

/**
 * Conexión entre dos nodos.
 *
 * Si tiene instrucciones inversas, se puede recorrer en los dos sentidos; si no, solo de
 * [from] a [to]. La instrucción depende del sentido («sube» frente a «baja»), por eso cada
 * sentido lleva la suya.
 *
 * @property bearingDegrees Rumbo real del tramo (grados desde el norte) si se quiere fijar a
 * mano. Si es `null`, se calcula con las posiciones de los dos nodos en el plano.
 */
@Serializable
data class IndoorEdge(
    val from: String,
    val to: String,
    val kind: EdgeKind = EdgeKind.WALK,
    val instructionEs: String,
    val instructionEn: String,
    val reverseInstructionEs: String? = null,
    val reverseInstructionEn: String? = null,
    val bearingDegrees: Double? = null,
) {
    /** `true` si el tramo se puede recorrer también de [to] a [from]. */
    val isBidirectional: Boolean
        get() = reverseInstructionEs != null && reverseInstructionEn != null
}

/**
 * Tramo del grafo recorrido en un sentido concreto: [reversed] es `true` si se va de
 * `edge.to` a `edge.from` (se usan entonces las instrucciones inversas).
 */
data class DirectedEdge(val edge: IndoorEdge, val reversed: Boolean)

/**
 * Ruta de muestra **fija**, escrita a mano en `graph.json`. No se calcula nada: la app recorre
 * sus paradas en orden.
 *
 * @property id Identificador de la ruta (por ejemplo, `ida` o `vuelta_ascensor`).
 * @property accessible `true` si no usa escaleras (va por el ascensor); se elige con «Evitar
 * escaleras».
 * @property stops Paradas en orden, del origen al destino. Entre cada dos seguidas tiene que
 * haber un tramo en el grafo.
 */
@Serializable
data class DemoRoute(
    val id: String,
    val accessible: Boolean = false,
    val stops: List<RouteStop>,
) {
    /** Nodo de salida. */
    val originId: String? get() = stops.firstOrNull()?.nodeId

    /** Nodo de llegada. */
    val destinationId: String? get() = stops.lastOrNull()?.nodeId
}

/**
 * Parada de una ruta de muestra.
 *
 * @property photoAsset Foto que se enseña mientras el usuario **está en este punto** (el punto
 * rojo del plano), tal como se ve en el sentido de la marcha: por eso la ida y la vuelta pueden
 * llevar fotos distintas del mismo nodo. En la última parada es la foto de la llegada. `null` =
 * sin foto todavía.
 */
@Serializable
data class RouteStop(
    val nodeId: String,
    val photoAsset: String? = null,
)
