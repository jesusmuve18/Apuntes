package com.jesusmuve.etsiitsmartcampus.feature.scanner

import androidx.lifecycle.SavedStateHandle
import com.jesusmuve.etsiitsmartcampus.core.model.IdentifiedPoint
import com.jesusmuve.etsiitsmartcampus.core.model.ScanSource
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.map

/**
 * Contrato para devolver el [IdentifiedPoint] del lector a la pantalla que lo abrió, usando el
 * `SavedStateHandle` de la entrada anterior de la pila de navegación (patrón estándar de
 * Navigation Compose para "resultados" de pantalla).
 *
 * Uso típico:
 * - El `NavHost`, al recibir el punto del lector: `previousBackStackEntry.savedStateHandle
 *.putScannerResult(point)` y `popBackStack()`.
 * - La pantalla de ruta: observa `savedStateHandle.scannerResultFlow()`, entrega el punto a su
 *   ViewModel (que decide si es el paso esperado) y llama a `clearScannerResult()`.
 *
 * Se guarda como un único `String` (`"QR|space-entrance"`) para que sea serializable en el
 * `Bundle` sin tipos propios. Los IDs válidos nunca contienen `|` (ver [TagReader.normalizeId]).
 */
object ScannerResult {
    internal const val KEY = "scanner_result"
    private const val SEPARATOR = '|'

    internal fun encode(point: IdentifiedPoint): String = "${point.source.name}$SEPARATOR${point.id}"

    internal fun decode(value: String?): IdentifiedPoint? {
        if (value == null) return null
        val separatorIndex = value.indexOf(SEPARATOR)
        if (separatorIndex <= 0) return null
        val source = ScanSource.entries.firstOrNull { it.name == value.substring(0, separatorIndex) }
            ?: return null
        val id = value.substring(separatorIndex + 1)
        if (id.isEmpty()) return null
        return IdentifiedPoint(id, source)
    }
}

/** Publica el resultado del lector para la pantalla anterior. */
fun SavedStateHandle.putScannerResult(point: IdentifiedPoint) {
    this[ScannerResult.KEY] = ScannerResult.encode(point)
}

/** Flujo con el último resultado pendiente del lector (`null` si no hay ninguno). */
fun SavedStateHandle.scannerResultFlow(): Flow<IdentifiedPoint?> =
    getStateFlow<String?>(ScannerResult.KEY, null).map(ScannerResult::decode)

/** Marca el resultado como consumido para no procesarlo dos veces (p. ej. al rotar). */
fun SavedStateHandle.clearScannerResult() {
    remove<String>(ScannerResult.KEY)
}
