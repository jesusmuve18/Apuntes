package com.jesusmuve.etsiitsmartcampus.core.data

import com.jesusmuve.etsiitsmartcampus.core.model.LocalizedText
import com.jesusmuve.etsiitsmartcampus.core.model.Space
import java.util.Locale
import kotlinx.serialization.Serializable
import kotlinx.serialization.SerializationException
import kotlinx.serialization.json.Json

/** Carga, valida y consulta el catálogo del campus (JSON leído con `kotlinx.serialization`).
 *
 * @property source De dónde se leen los JSON.
 * @property json Lector JSON. No admite claves desconocidas, para detectar errores en los datos.
 */
class JsonCampusRepository(
    private val source: CampusCatalogSource,
    private val json: Json = Json { ignoreUnknownKeys = false },
) : CampusRepository {

    // El catálogo se carga una sola vez y se reutiliza en las consultas siguientes.
    private val catalogResult: CampusCatalogResult<LoadedCampusCatalog> by lazy { loadInternal() }

    /**
     * Carga (o devuelve el estado ya calculado), normaliza y valida el catálogo desde [source].
     *
     * @return [CampusCatalogResult.Success] con un [LoadedCampusCatalog] válido, o
     * [CampusCatalogResult.Failure] con el [CatalogError] correspondiente.
     */
    fun load(): CampusCatalogResult<LoadedCampusCatalog> = catalogResult

    override fun catalogState(): CampusCatalogResult<LoadedCampusCatalog> = catalogResult

    override fun getSpaces(): List<Space> = loadedCatalog()?.spaces.orEmpty()

    override fun searchSpaces(query: String): List<Space> {
        val catalog = loadedCatalog() ?: return emptyList()
        val normalizedQuery = query.trim()
        // Consulta vacía: no hay resultados.
        if (normalizedQuery.isBlank()) return emptyList()
        val queryForMatch = normalizedQuery.lowercase(Locale.ROOT)
        return catalog.spaces.filter { space -> space.matches(queryForMatch) }
    }

    override fun findSpaceById(id: String): CampusLookupResult {
        val catalog = loadedCatalog() ?: return CampusLookupResult.NotFound
        // Se quitan los espacios sobrantes, igual que al leer un QR, NFC o código manual.
        val normalizedId = id.trim()
        val space = catalog.spaces.firstOrNull { it.id == normalizedId }
        return if (space != null) CampusLookupResult.Found(space) else CampusLookupResult.NotFound
    }

    private fun loadedCatalog(): LoadedCampusCatalog? = when (val result = catalogResult) {
        is CampusCatalogResult.Success -> result.value
        is CampusCatalogResult.Failure -> null
    }

    private fun loadInternal(): CampusCatalogResult<LoadedCampusCatalog> {
        val spaces = when (val attempt = parseSpaces()) {
            is ParseAttempt.Ok -> attempt.value
            is ParseAttempt.Fail -> return CampusCatalogResult.Failure(attempt.error)
        }
        val normalizedSpaces = spaces.map { it.normalized() }
        val validationError = validate(normalizedSpaces)
        if (validationError != null) {
            return CampusCatalogResult.Failure(validationError)
        }
        return CampusCatalogResult.Success(LoadedCampusCatalog(normalizedSpaces))
    }

    private fun parseSpaces(): ParseAttempt<List<Space>> = try {
        val payload = json.decodeFromString(CampusPayload.serializer(), source.readCampusJson())
        ParseAttempt.Ok(payload.spaces)
    } catch (e: SerializationException) {
        // Si el JSON no se puede leer, se devuelve un error en lugar de lanzar la excepción.
        ParseAttempt.Fail(CatalogError.Parse(parseErrorDetail("campus.json", e)))
    }


    /** Mensaje de error breve para depurar. */
    private fun parseErrorDetail(sourceName: String, e: SerializationException): String {
        val raw = e.message
            ?.lineSequence()
            ?.firstOrNull()
            ?.trim()
            .orEmpty()
            .ifBlank { "error de serialización" }
        val sanitized = raw.take(MAX_PARSE_DETAIL_LENGTH)
        return "$sourceName: $sanitized"
    }

    /**
     * Comprueba que el catálogo es coherente (IDs únicos, textos en los dos idiomas y un único
     * espacio con ruta). Devuelve `null` si todo es válido o el primer error encontrado.
     */
    private fun validate(spaces: List<Space>): CatalogError.Validation? {
        val duplicatedSpaceId = spaces.groupingBy { it.id }.eachCount().entries.firstOrNull { it.value > 1 }
        if (duplicatedSpaceId != null) {
            return CatalogError.Validation(
                reason = "ID de espacio duplicado: ${duplicatedSpaceId.key}",
                field = "id",
            )
        }


        spaces.forEach { space ->
            localizedBlankError(space.name, "Space.name (${space.id})")?.let { return it }
            localizedBlankError(space.description, "Space.description (${space.id})")?.let { return it }
            if (space.id.isBlank()) {
                return CatalogError.Validation("El ID de espacio no puede estar vacío", field = "id")
            }
        }

        val routeSpaces = spaces.filter { it.hasRoute }
        if (routeSpaces.size != 1) {
            return CatalogError.Validation(
                reason = "Debe existir exactamente un espacio con hasRoute=true; encontrados: ${routeSpaces.size}",
                field = "hasRoute",
            )
        }
        return null
    }

    private fun localizedBlankError(text: LocalizedText, fieldName: String): CatalogError.Validation? {
        // Se valida tras trim(): un texto solo con espacios no es contenido bilingüe válido.
        if (text.es.trim().isBlank()) {
            return CatalogError.Validation(
                reason = "El texto 'es' no puede estar vacío tras trim() en $fieldName",
                field = fieldName,
            )
        }
        if (text.en.trim().isBlank()) {
            return CatalogError.Validation(
                reason = "El texto 'en' no puede estar vacío tras trim() en $fieldName",
                field = fieldName,
            )
        }
        return null
    }

    private fun Space.normalized(): Space = copy(
        id = id.trim(),
        name = name.normalized(),
        description = description.normalized(),
        type = type.trim(),
    )

    private fun Space.matches(queryForMatch: String): Boolean {
        // Busca sin distinguir mayúsculas en el nombre y la descripción, en los dos idiomas.
        val candidates = listOf(name.es, name.en, description.es, description.en)
        return candidates.any { it.lowercase(Locale.ROOT).contains(queryForMatch) }
    }

    private fun LocalizedText.normalized(): LocalizedText = copy(es = es.trim(), en = en.trim())

    /** Estructura de `campus.json`. */
    @Serializable
    private data class CampusPayload(val spaces: List<Space>)

    /** Resultado de leer uno de los JSON. */
    private sealed interface ParseAttempt<out T> {
        data class Ok<T>(val value: T) : ParseAttempt<T>
        data class Fail(val error: CatalogError.Parse) : ParseAttempt<Nothing>
    }

    private companion object {

        /** Longitud máxima del mensaje de error. */
        const val MAX_PARSE_DETAIL_LENGTH = 160
    }
}
