package com.jesusmuve.etsiitsmartcampus.feature.reservations

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import java.time.LocalDate
import java.time.LocalDateTime
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.flow.update
import kotlinx.coroutines.launch

/** Un tramo tal como lo pinta la pantalla. */
data class SlotUi(val startMinute: Int, val endMinute: Int, val status: SlotStatus)

/** Avisos puntuales (se muestran una vez en un snackbar). */
sealed interface ReservationMessage {
    data class Reserved(val reservation: Reservation) : ReservationMessage
    data class Cancelled(val reservation: Reservation) : ReservationMessage
    data class Rejected(val error: ReservationError) : ReservationMessage
}

/**
 * Estado de la pantalla de reservas.
 *
 * @property pending Tramo pulsado pendiente de confirmar en el diálogo.
 */
data class ReservationsUiState(
    val loadFailed: Boolean = false,
    val rooms: List<StudyRoom> = emptyList(),
    val days: List<LocalDate> = emptyList(),
    val selectedDay: LocalDate? = null,
    val selectedRoomId: String? = null,
    val slots: List<SlotUi> = emptyList(),
    val upcoming: List<Reservation> = emptyList(),
    val maxSlotsPerDay: Int = 0,
    val slotMinutes: Int = 0,
    val pending: Reservation? = null,
    val message: ReservationMessage? = null,
)

/**
 * Reserva de salas de estudio de la biblioteca (**demostración**).
 *
 * Aplica [ReservationRules] sobre las reservas del propio usuario. Sin servidor no conoce la
 * ocupación real de las salas; la pantalla lo advierte. No recibe `Context`.
 *
 * @param catalog Salas y normas; `null` si el fichero de datos no se pudo leer.
 * @param now Hora actual.
 */
class ReservationsViewModel(
    private val catalog: StudyRoomsCatalog?,
    private val repository: ReservationRepository,
    private val now: () -> LocalDateTime = LocalDateTime::now,
) : ViewModel() {

    private val rules = catalog?.let(::ReservationRules)
    private var mine: List<Reservation> = emptyList()

    private val _uiState = MutableStateFlow(initialState())
    val uiState: StateFlow<ReservationsUiState> = _uiState.asStateFlow()

    init {
        if (rules != null) {
            viewModelScope.launch {
                repository.observeReservations().collect { reservations ->
                    mine = reservations
                    _uiState.update { recompute(it) }
                }
            }
        }
    }

    fun selectDay(day: LocalDate) {
        _uiState.update { recompute(it.copy(selectedDay = day)) }
    }

    fun selectRoom(roomId: String) {
        _uiState.update { recompute(it.copy(selectedRoomId = roomId)) }
    }

    /** Pulsar un tramo libre abre la confirmación; uno no disponible explica el motivo. */
    fun onSlotClick(startMinute: Int) {
        val state = _uiState.value
        val currentRules = rules ?: return
        val room = state.selectedRoomId ?: return
        val day = state.selectedDay ?: return
        val candidate = Reservation(room, day, startMinute)
        val error = currentRules.validate(candidate, mine, now())
        _uiState.update {
            if (error == null) it.copy(pending = candidate) else it.copy(message = ReservationMessage.Rejected(error))
        }
    }

    fun dismissPending() {
        _uiState.update { it.copy(pending = null) }
    }

    /** Confirma la reserva pendiente (vuelve a validar por si ha cambiado la hora). */
    fun confirmPending() {
        val candidate = _uiState.value.pending ?: return
        val currentRules = rules ?: return
        val error = currentRules.validate(candidate, mine, now())
        _uiState.update { it.copy(pending = null) }
        if (error != null) {
            _uiState.update { it.copy(message = ReservationMessage.Rejected(error)) }
            return
        }
        viewModelScope.launch {
            repository.add(candidate)
            _uiState.update { it.copy(message = ReservationMessage.Reserved(candidate)) }
        }
    }

    fun cancel(reservation: Reservation) {
        viewModelScope.launch {
            repository.remove(reservation)
            _uiState.update { it.copy(message = ReservationMessage.Cancelled(reservation)) }
        }
    }

    fun messageShown() {
        _uiState.update { it.copy(message = null) }
    }

    private fun initialState(): ReservationsUiState {
        val currentRules = rules ?: return ReservationsUiState(loadFailed = true)
        val days = currentRules.bookableDays(now().toLocalDate())
        val base = ReservationsUiState(
            rooms = catalog?.rooms.orEmpty(),
            days = days,
            selectedDay = days.firstOrNull(),
            selectedRoomId = catalog?.rooms?.firstOrNull()?.id,
            maxSlotsPerDay = catalog?.maxSlotsPerDay ?: 0,
            slotMinutes = catalog?.slotMinutes ?: 0,
        )
        return recompute(base)
    }

    private fun recompute(state: ReservationsUiState): ReservationsUiState {
        val currentRules = rules ?: return state
        val slotMinutes = catalog?.slotMinutes ?: 0
        val room = state.selectedRoomId
        val day = state.selectedDay
        val current = now()
        val slots = if (room == null || day == null) {
            emptyList()
        } else {
            currentRules.slotStarts().map { start ->
                SlotUi(start, start + slotMinutes, currentRules.status(room, day, start, mine, current))
            }
        }
        return state.copy(slots = slots, upcoming = currentRules.upcoming(mine, current))
    }
}
