package com.jesusmuve.etsiitsmartcampus.feature.outdoor

import android.Manifest
import android.annotation.SuppressLint
import android.content.Context
import android.content.pm.PackageManager
import android.location.Location
import android.location.LocationListener
import android.location.LocationManager
import android.os.Bundle
import android.os.Looper
import androidx.core.content.ContextCompat

/** Eventos que emite un [LocationTracker]. */
interface LocationTrackerListener {
    /** Nueva posición disponible. */
    fun onFix(fix: GeoFix)

    /** Cambia la disponibilidad de la ubicación del sistema (GPS desactivado, etc.). */
    fun onAvailabilityChanged(available: Boolean)
}

/**
 * Seguimiento de la posición GPS. La pantalla y el `ViewModel` solo conocen esta interfaz
 *; la implementación es [AndroidLocationTracker].
 *
 * Debe atarse al ciclo de vida: [start] al mostrarse la pantalla y [stop] al
 * ocultarse. Nada escucha el GPS en segundo plano.
 */
interface LocationTracker {
    /** `true` si la app tiene permiso de ubicación (precisa o aproximada). */
    fun hasPermission(): Boolean

    /** Empieza a recibir posiciones. No hace nada si falta el permiso. */
    fun start(listener: LocationTrackerListener)

    /** Deja de recibir posiciones y libera el GPS. Se puede llamar varias veces. */
    fun stop()
}

/**
 * Implementación con `LocationManager` del sistema: no necesita Google Play Services ni
 * Internet, así que funciona en cualquier móvil ( solo admite ubicación, no red).
 *
 * Usa el proveedor GPS y, si está activo, el de red como apoyo para la primera posición. Se
 * pide una actualización cada [MIN_INTERVAL_MS] o cada [MIN_DISTANCE_M]: suficiente para
 * detectar que alguien llega andando, sin gastar batería de más.
 *
 * @param appContext Contexto de la aplicación.
 */
class AndroidLocationTracker(private val appContext: Context) : LocationTracker {

    private val locationManager: LocationManager? =
        ContextCompat.getSystemService(appContext, LocationManager::class.java)

    private var activeListener: LocationListener? = null

    override fun hasPermission(): Boolean =
        isGranted(Manifest.permission.ACCESS_FINE_LOCATION) ||
            isGranted(Manifest.permission.ACCESS_COARSE_LOCATION)

    // El permiso se comprueba justo antes con hasPermission(); lint no lo deduce a través de
    // la llamada a otra función, de ahí la anotación.
    @SuppressLint("MissingPermission")
    override fun start(listener: LocationTrackerListener) {
        stop()
        val manager = locationManager
        if (manager == null || !hasPermission()) {
            listener.onAvailabilityChanged(false)
            return
        }

        val providers = listOf(LocationManager.GPS_PROVIDER, LocationManager.NETWORK_PROVIDER)
            .filter { provider -> isProviderEnabledSafely(manager, provider) }
        listener.onAvailabilityChanged(providers.isNotEmpty())

        val systemListener = object : LocationListener {
            override fun onLocationChanged(location: Location) {
                listener.onFix(location.toGeoFix())
            }

            override fun onProviderEnabled(provider: String) {
                listener.onAvailabilityChanged(true)
            }

            override fun onProviderDisabled(provider: String) {
                val stillAvailable = listOf(LocationManager.GPS_PROVIDER, LocationManager.NETWORK_PROVIDER)
                    .any { isProviderEnabledSafely(manager, it) }
                listener.onAvailabilityChanged(stillAvailable)
            }

            // Necesario en API < 29, donde este método no tiene implementación por defecto.
            @Deprecated("Deprecated in Java")
            override fun onStatusChanged(provider: String?, status: Int, extras: Bundle?) = Unit
        }
        activeListener = systemListener

        try {
            // Se registran ambos proveedores aunque estén apagados: si el usuario activa la
            // ubicación con la pantalla abierta, llegan posiciones sin reiniciar nada.
            listOf(LocationManager.GPS_PROVIDER, LocationManager.NETWORK_PROVIDER)
                .filter { manager.allProviders.contains(it) }
                .forEach { provider ->
                    manager.requestLocationUpdates(
                        provider, MIN_INTERVAL_MS, MIN_DISTANCE_M, systemListener, Looper.getMainLooper(),
                    )
                }
            // Última posición conocida: da una respuesta inmediata mientras llega la primera.
            providers
                .mapNotNull { manager.getLastKnownLocation(it) }
                .filter { System.currentTimeMillis() - it.time < MAX_LAST_KNOWN_AGE_MS }
                .minByOrNull { it.accuracy }
                ?.let { listener.onFix(it.toGeoFix()) }
        } catch (e: SecurityException) {
            // El usuario retiró el permiso entre la comprobación y el registro.
            stop()
            listener.onAvailabilityChanged(false)
        }
    }

    override fun stop() {
        val listener = activeListener ?: return
        locationManager?.removeUpdates(listener)
        activeListener = null
    }

    private fun isGranted(permission: String): Boolean =
        ContextCompat.checkSelfPermission(appContext, permission) == PackageManager.PERMISSION_GRANTED

    private fun isProviderEnabledSafely(manager: LocationManager, provider: String): Boolean =
        try {
            manager.isProviderEnabled(provider)
        } catch (e: IllegalArgumentException) {
            // El proveedor no existe en este dispositivo.
            false
        }

    private fun Location.toGeoFix(): GeoFix = GeoFix(
        latitude = latitude,
        longitude = longitude,
        // Sin dato de precisión se asume una muy mala, para no anunciar una llegada dudosa.
        accuracyMeters = if (hasAccuracy()) accuracy else Float.MAX_VALUE,
    )

    private companion object {
        const val MIN_INTERVAL_MS = 2_000L
        const val MIN_DISTANCE_M = 2f
        const val MAX_LAST_KNOWN_AGE_MS = 2 * 60 * 1_000L
    }
}
