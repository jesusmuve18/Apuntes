package com.jesusmuve.etsiitsmartcampus.core.data

import android.content.res.AssetManager

/**
 * Lee los JSON del catálogo desde los assets de la app.
 *
 * @property assetManager Gestor de assets de Android.
 */
class AssetCampusCatalogSource(
    private val assetManager: AssetManager,
) : CampusCatalogSource {

    /**
     * Lee el contenido completo de [CAMPUS_ASSET] desde assets.
     *
     * @return JSON del catálogo de espacios como cadena UTF-8.
     * @throws java.io.IOException si no se puede abrir o leer el asset.
     */
    override fun readCampusJson(): String = readAsset(CAMPUS_ASSET)

    // Lee el fichero completo en UTF-8 y cierra el stream.
    private fun readAsset(name: String): String =
        assetManager.open(name).use { stream ->
            stream.bufferedReader(Charsets.UTF_8).readText()
        }

    private companion object {
        /** Nombre del asset del catálogo de espacios. */
        const val CAMPUS_ASSET = "campus.json"
    }
}
