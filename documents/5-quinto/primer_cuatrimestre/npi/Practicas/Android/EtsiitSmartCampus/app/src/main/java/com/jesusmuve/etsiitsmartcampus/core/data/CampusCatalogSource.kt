package com.jesusmuve.etsiitsmartcampus.core.data

/** Origen del JSON del catálogo de espacios (los assets de la app). */
interface CampusCatalogSource {

    /** Devuelve el contenido de `campus.json`; el repositorio lo valida al cargarlo. */
    fun readCampusJson(): String
}
