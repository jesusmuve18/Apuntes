package com.jesusmuve.etsiitsmartcampus

import android.content.Context
import android.content.res.AssetManager
import androidx.datastore.core.DataStore
import androidx.datastore.preferences.core.Preferences
import com.jesusmuve.etsiitsmartcampus.core.data.AssetCampusCatalogSource
import com.jesusmuve.etsiitsmartcampus.core.data.AssetIndoorMapRepository
import com.jesusmuve.etsiitsmartcampus.core.data.CampusCatalogSource
import com.jesusmuve.etsiitsmartcampus.core.data.CampusLookupResult
import com.jesusmuve.etsiitsmartcampus.core.data.CampusRepository
import com.jesusmuve.etsiitsmartcampus.core.data.DataStoreUserPreferencesRepository
import com.jesusmuve.etsiitsmartcampus.core.data.IndoorMapResult
import com.jesusmuve.etsiitsmartcampus.core.data.IndoorMapSource
import com.jesusmuve.etsiitsmartcampus.core.data.JsonCampusRepository
import com.jesusmuve.etsiitsmartcampus.core.data.UserPreferencesRepository
import com.jesusmuve.etsiitsmartcampus.feature.outdoor.AndroidLocationTracker
import com.jesusmuve.etsiitsmartcampus.feature.outdoor.LocationTracker
import com.jesusmuve.etsiitsmartcampus.feature.reservations.DataStoreReservationRepository
import com.jesusmuve.etsiitsmartcampus.feature.reservations.ReservationRepository
import com.jesusmuve.etsiitsmartcampus.feature.reservations.StudyRoomsCatalog
import com.jesusmuve.etsiitsmartcampus.feature.schedule.Timetable
import java.io.IOException
import com.jesusmuve.etsiitsmartcampus.feature.scanner.PointCatalog
import com.jesusmuve.etsiitsmartcampus.feature.scanner.TagReader

/**
 * Crea y guarda las dependencias compartidas de la app (inyección de dependencias manual).
 *
 * @param appContext Contexto de la aplicación.
 */
class AppContainer(private val appContext: Context) {

    /**
     * Grafo de la navegación interior (`assets/indoor/graph.json`). Una sola instancia: el
     * fichero se lee una vez y lo comparten la ruta interior, la navegación exterior y el lector.
     */
    val indoorMapSource: IndoorMapSource by lazy { AssetIndoorMapRepository(appContext) }

    /**
     * Catálogo de espacios (`campus.json`), compartido por la búsqueda, la ficha y
     * el lector. Se lee y valida una sola vez por proceso.
     */
    val campusRepository: CampusRepository by lazy {
        createCampusRepository(createCampusCatalogSource(appContext.assets))
    }

    /**
     * Salas de estudio y normas de reserva (`assets/library/study_rooms.json`, datos de ejemplo).
     * `null` si el fichero no se puede leer o no es válido: la pantalla muestra un error.
     */
    val studyRoomsCatalog: StudyRoomsCatalog? by lazy {
        try {
            appContext.assets.open(STUDY_ROOMS_ASSET).bufferedReader(Charsets.UTF_8).use { it.readText() }
                .let(StudyRoomsCatalog::parse)
        } catch (e: IOException) {
            null
        }
    }

    /**
     * Horario del grupo (`assets/schedule/timetable.json`). `null` si el fichero no se puede leer
     * o no es válido: la pantalla muestra un error.
     */
    val timetable: Timetable? by lazy {
        try {
            appContext.assets.open(TIMETABLE_ASSET).bufferedReader(Charsets.UTF_8).use { it.readText() }
                .let(Timetable::parse)
        } catch (e: IOException) {
            null
        }
    }

    /** Reservas de la demostración, guardadas solo en este móvil. */
    fun createReservationRepository(dataStore: DataStore<Preferences>): ReservationRepository =
        DataStoreReservationRepository(dataStore)

    /**
     * Crea un acceso al GPS para una pantalla. Cada `ViewModel` tiene el suyo y lo libera en
     * `onCleared`, de modo que el GPS nunca queda encendido sin una pantalla que lo use.
     */
    fun createLocationTracker(): LocationTracker = AndroidLocationTracker(appContext)

    /**
     * Crea la fuente de catálogo que lee `campus.json` desde assets.
     *
     * @param assetManager Gestor de assets de la app.
     */
    fun createCampusCatalogSource(assetManager: AssetManager): CampusCatalogSource =
        AssetCampusCatalogSource(assetManager)

    /**
     * Crea el repositorio del catálogo a partir de [source].
     */
    fun createCampusRepository(source: CampusCatalogSource): CampusRepository =
        JsonCampusRepository(source)

    /**
     * Crea el repositorio de preferencias de usuario respaldado por DataStore.
     *
     * @param dataStore DataStore de preferencias de la app.
     */
    fun createUserPreferencesRepository(
        dataStore: DataStore<Preferences>,
    ): UserPreferencesRepository = DataStoreUserPreferencesRepository(dataStore)

    /** Crea el lector unificado de etiquetas (QR, NFC y código manual). */
    fun createTagReader(campusRepository: CampusRepository): TagReader = TagReader(
        PointCatalog { id ->
            val map = (indoorMapSource.load() as? IndoorMapResult.Loaded)?.map
            map?.node(id) != null || campusRepository.findSpaceById(id) is CampusLookupResult.Found
        },
    )

    private companion object {
        const val STUDY_ROOMS_ASSET = "library/study_rooms.json"
        const val TIMETABLE_ASSET = "schedule/timetable.json"
    }
}
