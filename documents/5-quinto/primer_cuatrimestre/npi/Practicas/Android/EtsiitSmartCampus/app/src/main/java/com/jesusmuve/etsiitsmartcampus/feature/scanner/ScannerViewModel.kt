package com.jesusmuve.etsiitsmartcampus.feature.scanner

import androidx.lifecycle.ViewModel
import androidx.lifecycle.ViewModelProvider
import androidx.lifecycle.viewModelScope
import com.jesusmuve.etsiitsmartcampus.core.model.IdentifiedPoint
import com.jesusmuve.etsiitsmartcampus.core.model.ScanSource
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.flow.update
import kotlinx.coroutines.launch

/** Motivo por el que se ha rechazado la última lectura. */
enum class ScanRejection {
    /** Contenido sin forma de ID (URL, texto libre, vacío…). */
    MALFORMED,

    /** ID con formato correcto pero que no está en el catálogo. */
    UNKNOWN,
}

/**
 * Estado de la pantalla del lector.
 *
 * @property manualCode Texto actual del campo de código manual.
 * @property rejection Motivo del último rechazo, o `null` si no hay error que mostrar.
 * @property rejectedSource Origen de la lectura rechazada (para adaptar el mensaje).
 * @property identified Primer punto identificado con éxito. Una vez fijado, el lector ignora
 * cualquier lectura posterior y la pantalla debe entregarlo y cerrarse.
 */
data class ScannerUiState(
    val manualCode: String = "",
    val rejection: ScanRejection? = null,
    val rejectedSource: ScanSource? = null,
    val identified: IdentifiedPoint? = null,
)

/**
 * ViewModel del lector QR/NFC/manual.
 *
 * Recibe el contenido bruto de cualquier origen, lo pasa por [TagReader] y expone el resultado.
 * Solo se acepta la **primera** lectura válida, para no emitir eventos repetidos. No sabe nada
 * de CameraX, NFC ni `Context`; tampoco decide si el punto es el esperado en la ruta.
 */
class ScannerViewModel(private val tagReader: TagReader) : ViewModel() {

    private val _uiState = MutableStateFlow(ScannerUiState())
    val uiState: StateFlow<ScannerUiState> = _uiState.asStateFlow()

    /** `true` cuando ya hay un punto identificado y el lector debe dejar de analizar. */
    val isFinished: Boolean get() = _uiState.value.identified != null

    /** Procesa un contenido leído por QR, NFC o introducido a mano. */
    fun onPayloadRead(rawPayload: String, source: ScanSource) {
        if (isFinished) return
        viewModelScope.launch {
            val result = tagReader.identify(rawPayload, source)
            // Otra lectura pudo terminar antes mientras se consultaba el catálogo.
            if (isFinished) return@launch
            when (result) {
                is TagReadResult.Identified -> _uiState.update {
                    it.copy(identified = result.point, rejection = null, rejectedSource = null)
                }
                TagReadResult.Malformed -> reject(ScanRejection.MALFORMED, source)
                TagReadResult.Unknown -> reject(ScanRejection.UNKNOWN, source)
            }
        }
    }

    fun onManualCodeChange(code: String) {
        // Se limita la longitud del campo para que no se pueda pegar texto arbitrario largo.
        _uiState.update { it.copy(manualCode = code.take(TagReader.MAX_ID_LENGTH)) }
    }

    fun onManualCodeSubmit() {
        onPayloadRead(_uiState.value.manualCode, ScanSource.MANUAL)
    }

    /** Oculta el mensaje de error (p. ej. al reintentar). */
    fun onRejectionDismissed() {
        _uiState.update { it.copy(rejection = null, rejectedSource = null) }
    }

    private fun reject(reason: ScanRejection, source: ScanSource) {
        _uiState.update { it.copy(rejection = reason, rejectedSource = source) }
    }
}

/** Fábrica de [ScannerViewModel] para la DI manual. */
class ScannerViewModelFactory(private val tagReader: TagReader) : ViewModelProvider.Factory {

    @Suppress("UNCHECKED_CAST")
    override fun <T : ViewModel> create(modelClass: Class<T>): T {
        if (modelClass.isAssignableFrom(ScannerViewModel::class.java)) {
            return ScannerViewModel(tagReader) as T
        }
        throw IllegalArgumentException("Unknown ViewModel class: ${modelClass.name}")
    }
}
