package com.jesusmuve.etsiitsmartcampus.feature.reservations

import androidx.datastore.core.DataStore
import androidx.datastore.preferences.core.Preferences
import androidx.datastore.preferences.core.edit
import androidx.datastore.preferences.core.stringSetPreferencesKey
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.map

/**
 * Dónde se guardan las reservas. Solo en este móvil: es una **demostración** sin servidor, así
 * que nadie más las ve ni la biblioteca las recibe.
 */
interface ReservationRepository {
    fun observeReservations(): Flow<List<Reservation>>
    suspend fun add(reservation: Reservation)
    suspend fun remove(reservation: Reservation)
}

/** Implementación con DataStore Preferences (un conjunto de textos `sala|fecha|minuto`). */
class DataStoreReservationRepository(
    private val dataStore: DataStore<Preferences>,
) : ReservationRepository {

    override fun observeReservations(): Flow<List<Reservation>> =
        dataStore.data.map { prefs -> prefs[KEY].orEmpty().mapNotNull(Reservation::decode) }

    override suspend fun add(reservation: Reservation) {
        dataStore.edit { prefs -> prefs[KEY] = prefs[KEY].orEmpty() + reservation.encode() }
    }

    override suspend fun remove(reservation: Reservation) {
        dataStore.edit { prefs -> prefs[KEY] = prefs[KEY].orEmpty() - reservation.encode() }
    }

    private companion object {
        val KEY = stringSetPreferencesKey("study_room_reservations")
    }
}
