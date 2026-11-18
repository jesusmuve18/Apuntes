package com.jesusmuve.etsiitsmartcampus.ui.welcome

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.jesusmuve.etsiitsmartcampus.core.data.UserPreferencesRepository
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.flow.first
import kotlinx.coroutines.flow.update
import kotlinx.coroutines.launch

/**
 * Estado de la bienvenida.
 *
 * @property savedName Nombre guardado de una visita anterior (para «Hola de nuevo»), o `null`.
 * @property nameInput Texto del campo «¿Cómo te llamas?».
 */
data class WelcomeUiState(
    val savedName: String? = null,
    val nameInput: String = "",
)

/**
 * Bienvenida al abrir la app: personalización local sencilla, **sin cuentas ni contraseñas**.
 * El nombre es opcional, solo sirve para saludar y se guarda en el
 * propio móvil (DataStore).
 */
class WelcomeViewModel(
    private val userPreferencesRepository: UserPreferencesRepository,
) : ViewModel() {

    private val _uiState = MutableStateFlow(WelcomeUiState())
    val uiState: StateFlow<WelcomeUiState> = _uiState.asStateFlow()

    init {
        viewModelScope.launch {
            val saved = userPreferencesRepository.observeUserName().first()
            _uiState.update { it.copy(savedName = saved, nameInput = saved.orEmpty()) }
        }
    }

    fun onNameChange(name: String) {
        _uiState.update { it.copy(nameInput = name.take(UserPreferencesRepository.MAX_USER_NAME_LENGTH)) }
    }

    /** Guarda el nombre (vacío = no guardar ninguno) y continúa con [onDone]. */
    fun onEnter(onDone: () -> Unit) {
        viewModelScope.launch {
            userPreferencesRepository.saveUserName(_uiState.value.nameInput)
            onDone()
        }
    }
}
