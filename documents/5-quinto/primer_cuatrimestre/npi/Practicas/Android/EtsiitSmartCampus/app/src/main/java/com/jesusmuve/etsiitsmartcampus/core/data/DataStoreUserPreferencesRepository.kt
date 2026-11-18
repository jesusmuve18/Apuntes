package com.jesusmuve.etsiitsmartcampus.core.data

import androidx.datastore.core.DataStore
import androidx.datastore.preferences.core.Preferences
import androidx.datastore.preferences.core.edit
import androidx.datastore.preferences.core.stringPreferencesKey
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.map

/** [UserPreferencesRepository] guardado con DataStore Preferences.
 */
class DataStoreUserPreferencesRepository(
    private val dataStore: DataStore<Preferences>,
) : UserPreferencesRepository {

    override fun observeLastDestinationSpaceId(): Flow<String?> =
        dataStore.data.map { preferences -> preferences[LAST_DESTINATION_SPACE_ID_KEY] }

    override suspend fun saveLastDestinationSpaceId(spaceId: String) {
        val normalizedSpaceId = spaceId.trim()
        dataStore.edit { preferences ->
            preferences[LAST_DESTINATION_SPACE_ID_KEY] = normalizedSpaceId
        }
    }

    override suspend fun clearLastDestinationSpaceId() {
        dataStore.edit { preferences ->
            preferences.remove(LAST_DESTINATION_SPACE_ID_KEY)
        }
    }

    override fun observeUserName(): Flow<String?> =
        dataStore.data.map { preferences -> preferences[USER_NAME_KEY] }

    override suspend fun saveUserName(name: String) {
        val normalized = UserPreferencesRepository.normalizeUserName(name)
        dataStore.edit { preferences ->
            if (normalized == null) preferences.remove(USER_NAME_KEY) else preferences[USER_NAME_KEY] = normalized
        }
    }

    private companion object {
        /** Nombre de bienvenida (solo para saludar; no es una cuenta). */
        val USER_NAME_KEY = stringPreferencesKey("user_name")

        /**
         * Último espacio consultado ([com.jesusmuve.etsiitsmartcampus.core.model.Space.id]).
         */
        val LAST_DESTINATION_SPACE_ID_KEY = stringPreferencesKey("last_destination_space_id")
    }
}
