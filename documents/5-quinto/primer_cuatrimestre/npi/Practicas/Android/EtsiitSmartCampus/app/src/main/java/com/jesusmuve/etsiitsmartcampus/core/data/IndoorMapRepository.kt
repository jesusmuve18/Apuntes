package com.jesusmuve.etsiitsmartcampus.core.data

import android.content.Context
import com.jesusmuve.etsiitsmartcampus.core.model.IndoorMap
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.sync.Mutex
import kotlinx.coroutines.sync.withLock
import kotlinx.coroutines.withContext
import java.io.IOException

/**
 * Fuente del mapa interior. Las pantallas y los `ViewModel` dependen de esta interfaz y no de
 * los `assets`.
 */
interface IndoorMapSource {
    /** Carga (o devuelve ya cargado) el mapa interior. Nunca lanza: los fallos van en el resultado. */
    suspend fun load(): IndoorMapResult
}

/** Resultado de cargar el mapa: datos válidos o la lista de problemas encontrados. */
sealed interface IndoorMapResult {
    /** Mapa leído y validado. */
    data class Loaded(val map: IndoorMap) : IndoorMapResult

    /** El fichero no existe, no se puede leer o sus datos no son coherentes. */
    data class Invalid(val problems: List<String>) : IndoorMapResult
}

/**
 * Lee `assets/indoor/graph.json` una sola vez y guarda el resultado en memoria.
 *
 * La lectura se hace en `Dispatchers.IO` para no bloquear la interfaz. Recibe el `Context` de la
 * aplicación por constructor: ningún `ViewModel` lo toca.
 *
 * @param appContext Contexto de la aplicación (no de una `Activity`, para no retenerla).
 * @param assetPath Ruta del fichero dentro de `assets/`.
 */
class AssetIndoorMapRepository(
    private val appContext: Context,
    private val assetPath: String = DEFAULT_ASSET_PATH,
) : IndoorMapSource {

    private val mutex = Mutex()
    private var cached: IndoorMapResult? = null

    override suspend fun load(): IndoorMapResult = mutex.withLock {
        cached ?: readAndParse().also { result ->
            // Solo se guarda un mapa válido: si falla, se reintenta en la siguiente carga.
            if (result is IndoorMapResult.Loaded) cached = result
        }
    }

    private suspend fun readAndParse(): IndoorMapResult = withContext(Dispatchers.IO) {
        try {
            val text = appContext.assets.open(assetPath).bufferedReader(Charsets.UTF_8)
                .use { it.readText() }
            IndoorMapParser.parse(text)
        } catch (e: IOException) {
            IndoorMapResult.Invalid(listOf("No se puede leer $assetPath: ${e.message}"))
        }
    }

    companion object {
        /** Ubicación por defecto del grafo de la ruta interior. */
        const val DEFAULT_ASSET_PATH = "indoor/graph.json"
    }
}
