package com.jesusmuve.etsiitsmartcampus.core.data

import com.jesusmuve.etsiitsmartcampus.core.model.Space

/** Acceso al catálogo de espacios del campus. */
interface CampusRepository {

    /** Devuelve el estado de la carga/validación del catálogo. */
    fun catalogState(): CampusCatalogResult<LoadedCampusCatalog>

    /** Devuelve los espacios del catálogo cargado. */
    fun getSpaces(): List<Space>

    /**
     * Busca espacios por texto sobre campos relevantes (nombre y descripción, ES/EN).
     *
     * @param query Texto de búsqueda introducido por el usuario.
     */
    fun searchSpaces(query: String): List<Space>

    /**
     * Obtiene un espacio por su identificador estable ([Space.id]/checkpoint).
     *
     * @param id Identificador del espacio o punto de control.
     * @return [CampusLookupResult.Found] si existe o [CampusLookupResult.NotFound] si no existe o
     * el catálogo no se ha podido cargar.
     */
    fun findSpaceById(id: String): CampusLookupResult
}
