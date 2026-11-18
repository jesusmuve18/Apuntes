package com.jesusmuve.etsiitsmartcampus.core.data

import kotlinx.coroutines.flow.Flow

/** Preferencias del usuario guardadas en el móvil. */
interface UserPreferencesRepository {

    /** Identificador del último espacio consultado. */
    fun observeLastDestinationSpaceId(): Flow<String?>

    /**
     * Guarda el último espacio consultado (se quitan los espacios sobrantes del [spaceId]).
     */
    suspend fun saveLastDestinationSpaceId(spaceId: String)

    /** Elimina la preferencia de último destino. */
    suspend fun clearLastDestinationSpaceId()

    /**
     * Nombre con el que saludar al usuario (no es una cuenta). `null` si no ha dado ninguno.
     */
    fun observeUserName(): Flow<String?>

    /**
     * Guarda el nombre recortado a [MAX_USER_NAME_LENGTH] caracteres; si queda vacío, lo borra.
     */
    suspend fun saveUserName(name: String)

    companion object {
        /** Longitud máxima del nombre de bienvenida. */
        const val MAX_USER_NAME_LENGTH = 30

        /** Normaliza el nombre: sin espacios sobrantes y con longitud máxima; vacío → `null`. */
        fun normalizeUserName(name: String): String? =
            name.trim().replace(Regex("\\s+"), " ").take(MAX_USER_NAME_LENGTH).trim().ifEmpty { null }
    }
}
