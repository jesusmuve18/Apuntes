package com.jesusmuve.etsiitsmartcampus.ui.navigation

import java.net.URLEncoder

/**
 * Rutas de navegación de la app. Son identificadores internos, no textos de la interfaz.
 */
object CampusRoutes {
    /** Bienvenida al abrir la app: logo y nombre opcional para saludar (sin cuentas). */
    const val WELCOME = "welcome"

    /** Inicio: búsqueda de aula. */
    const val SEARCH = "search"

    /** Nombre del argumento de ruta que identifica el espacio en la ficha. */
    const val ARG_SPACE_ID = "spaceId"

    /** Patrón de ruta de la ficha con identificador obligatorio. */
    const val SPACE_PATTERN = "space/{spaceId}"

    /** Ruta guiada por pasos. */
    const val ROUTE = "route"

    /**
     * Lector de puntos de control QR/NFC/manual. Se abre desde la ruta (botón
     * «Escanear» o gesto G1) y devuelve el punto por `SavedStateHandle` (ver `ScannerResult`).
     */
    const val SCANNER = "scanner"

    /** Reserva de salas de estudio de la biblioteca (demostración, solo en el móvil). */
    const val RESERVATIONS = "reservations"

    /** «Consultar horarios»: horario del grupo por cuatrimestre, día y subgrupo. */
    const val SCHEDULE = "schedule"

    /** Ajustes y ayuda. */
    const val SETTINGS = "settings"

    /**
     * Construye la ruta de ficha para un [spaceId] concreto, codificando el identificador
     * para uso seguro en el path de Navigation Compose.
     *
     * @param spaceId Identificador del espacio (`Space.id`). Si no existe, la ficha lo indica.
     */
    fun spaceRoute(spaceId: String): String = "space/${encodePathSegment(spaceId)}"

    /**
     * Codifica el identificador para usarlo en la ruta.
     */
    private fun encodePathSegment(value: String): String =
        URLEncoder.encode(value, "UTF-8").replace("+", "%20")
    /** Navegación exterior: cómo llegar a la ETSIIT con Google Maps y aviso de llegada por GPS. */
    const val OUTDOOR = "outdoor"

    /** Nombre del argumento opcional con el nodo de origen de la ruta interior. */
    const val ROUTE_ORIGIN_ARG = "origin"

    /** Nombre del argumento opcional con el nodo de destino de la ruta interior. */
    const val ROUTE_DESTINATION_ARG = "dest"

    /**
     * Patrón de la ruta interior, con origen y destino opcionales.
     */
    const val ROUTE_PATTERN =
        "$ROUTE?$ROUTE_ORIGIN_ARG={$ROUTE_ORIGIN_ARG}&$ROUTE_DESTINATION_ARG={$ROUTE_DESTINATION_ARG}"

    /** Ruta interior empezando en el nodo [originNodeId] (p. ej. la entrada, al llegar con el GPS). */
    fun routeFrom(originNodeId: String): String =
        "$ROUTE?$ROUTE_ORIGIN_ARG=${encodePathSegment(originNodeId)}"

    /**
     * Ruta interior hacia el aula [room] del horario (p. ej. «2.2»). La pantalla de ruta la busca
     * en el grafo; si no está, la muestra como destino sin ruta en la demostración.
     */
    fun routeToRoom(room: String): String = routeTo("aula:$room")

    /** Ruta interior hacia el nodo [destinationNodeId] (p. ej. desde la ficha de un aula). */
    fun routeTo(destinationNodeId: String): String =
        "$ROUTE?$ROUTE_DESTINATION_ARG=${encodePathSegment(destinationNodeId)}"
}
