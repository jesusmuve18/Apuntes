package com.jesusmuve.etsiitsmartcampus.feature.outdoor

import android.content.ActivityNotFoundException
import android.content.Context
import android.content.Intent
import android.net.Uri
import android.provider.Settings

/**
 * Abre aplicaciones externas: Google Maps para la ruta exterior y los ajustes de ubicación.
 *
 * Se usa la URL universal de Google Maps (`https://www.google.com/maps/dir/?api=1`):
 * - No necesita clave de API ni facturación, ni el permiso `INTERNET` en nuestra app: la
 *   conexión la hace Google Maps.
 * - Si la app de Google Maps está instalada se abre directamente en modo «a pie»; si no, la
 *   misma URL se abre en el navegador.
 */
object MapsLauncher {

    private const val GOOGLE_MAPS_PACKAGE = "com.google.android.apps.maps"

    /**
     * Construye la URL de indicaciones a pie (formato «Maps URLs» de Google).
     *
     * @param query Nombre del destino. Si se da, Maps lo busca por nombre y muestra la ETSIIT
     * como destino; si es `null`, se usan las coordenadas.
     * @param placeId Identificador de Google Maps del lugar. Si se da, Maps abre exactamente esa
     * ficha (el nombre de [query] queda solo como texto de apoyo).
     */
    fun walkingDirectionsUri(
        latitude: Double,
        longitude: Double,
        query: String? = null,
        placeId: String? = null,
    ): Uri {
        val builder = Uri.parse("https://www.google.com/maps/dir/").buildUpon()
            .appendQueryParameter("api", "1")
            .appendQueryParameter("destination", query?.takeIf { it.isNotBlank() } ?: "$latitude,$longitude")
        placeId?.takeIf { it.isNotBlank() }?.let { builder.appendQueryParameter("destination_place_id", it) }
        return builder.appendQueryParameter("travelmode", "walking").build()
    }

    /**
     * Abre las indicaciones a pie hasta la entrada.
     *
     * @return `false` si no hay ninguna app capaz de abrir la ruta (sin Maps ni navegador); la
     * pantalla lo comunica al usuario en lugar de fallar (criterio de robustez del guion).
     */
    fun openWalkingDirections(
        context: Context,
        latitude: Double,
        longitude: Double,
        query: String? = null,
        placeId: String? = null,
    ): Boolean {
        val uri = walkingDirectionsUri(latitude, longitude, query, placeId)
        val mapsIntent = Intent(Intent.ACTION_VIEW, uri).setPackage(GOOGLE_MAPS_PACKAGE)
        return tryStart(context, mapsIntent) || tryStart(context, Intent(Intent.ACTION_VIEW, uri))
    }

    /** Abre la pantalla del sistema para activar la ubicación. */
    fun openLocationSettings(context: Context): Boolean =
        tryStart(context, Intent(Settings.ACTION_LOCATION_SOURCE_SETTINGS))

    /** Abre los ajustes de la app, para conceder un permiso denegado de forma permanente. */
    fun openAppSettings(context: Context): Boolean = tryStart(
        context,
        Intent(Settings.ACTION_APPLICATION_DETAILS_SETTINGS, Uri.fromParts("package", context.packageName, null)),
    )

    private fun tryStart(context: Context, intent: Intent): Boolean = try {
        context.startActivity(intent)
        true
    } catch (e: ActivityNotFoundException) {
        false
    }
}
