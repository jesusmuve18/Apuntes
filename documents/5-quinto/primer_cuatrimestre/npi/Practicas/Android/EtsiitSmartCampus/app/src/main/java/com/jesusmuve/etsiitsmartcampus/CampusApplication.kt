package com.jesusmuve.etsiitsmartcampus

import android.app.Application
import android.content.Context
import androidx.datastore.core.DataStore
import androidx.datastore.preferences.core.Preferences
import androidx.datastore.preferences.preferencesDataStore

/** Clase `Application` de ETSIIT Smart Campus. */
class CampusApplication : Application() {

    /** Contenedor de dependencias compartidas de la app, creado al arrancar. */
    val container: AppContainer by lazy { AppContainer(this) }

    /** Preferencias del usuario (DataStore), una sola instancia para toda la app. */
    val userPreferencesDataStore: DataStore<Preferences> by preferencesDataStore(
        name = "user_preferences",
    )
}
