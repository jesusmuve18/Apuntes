package com.jesusmuve.etsiitsmartcampus.feature.reservations

import java.time.LocalDate
import java.time.LocalDateTime

/**
 * Reserva de una sala para un tramo (guardada solo en este móvil).
 *
 * @property startMinute Minuto del día en que empieza el tramo (p. ej. 10:30 → 630).
 */
data class Reservation(
    val roomId: String,
    val date: LocalDate,
    val startMinute: Int,
) {
    /** Forma compacta para guardarla en DataStore: `sala|2026-11-13|630`. */
    fun encode(): String = "$roomId|$date|$startMinute"

    companion object {
        /** Lee una reserva guardada; `null` si está corrupta (se ignora). */
        fun decode(value: String): Reservation? {
            val parts = value.split("|")
            if (parts.size != 3 || parts[0].isBlank()) return null
            val date = runCatching { LocalDate.parse(parts[1]) }.getOrNull() ?: return null
            val minute = parts[2].toIntOrNull() ?: return null
            return Reservation(parts[0], date, minute)
        }
    }
}

/** Estado de un tramo en la pantalla. */
enum class SlotStatus {
    /** Se puede reservar. */
    FREE,

    /** Ya es una reserva mía en esta sala. */
    MINE,

    /** Ya tengo otra sala reservada a esta hora. */
    BUSY_ELSEWHERE,

    /** Ya ha empezado o pasado. */
    PAST,

    /** He llegado al máximo de tramos de ese día. */
    DAILY_LIMIT,
}

/** Motivo por el que no se ha podido reservar. */
enum class ReservationError { PAST, OVERLAP, DAILY_LIMIT, ALREADY_MINE, UNKNOWN_SLOT }

/**
 * Normas de reserva. No hay servidor: solo se comprueban las
 * reservas del propio usuario, no la ocupación real de la sala.
 */
class ReservationRules(private val catalog: StudyRoomsCatalog) {

    /** Días reservables: hoy y los siguientes, hasta [StudyRoomsCatalog.daysAhead]. */
    fun bookableDays(today: LocalDate): List<LocalDate> =
        (0 until catalog.daysAhead).map { today.plusDays(it.toLong()) }

    /** Minutos de inicio de los tramos de un día. */
    fun slotStarts(): List<Int> =
        generateSequence(catalog.openMinute) { it + catalog.slotMinutes }
            .takeWhile { it + catalog.slotMinutes <= catalog.closeMinute }
            .toList()

    fun status(
        roomId: String,
        date: LocalDate,
        startMinute: Int,
        mine: List<Reservation>,
        now: LocalDateTime,
    ): SlotStatus {
        if (mine.any { it.roomId == roomId && it.date == date && it.startMinute == startMinute }) {
            return SlotStatus.MINE
        }
        if (isPast(date, startMinute, now)) return SlotStatus.PAST
        if (mine.any { it.date == date && it.startMinute == startMinute }) return SlotStatus.BUSY_ELSEWHERE
        if (mine.count { it.date == date } >= catalog.maxSlotsPerDay) return SlotStatus.DAILY_LIMIT
        return SlotStatus.FREE
    }

    /** Devuelve `null` si se puede reservar, o el motivo si no. */
    fun validate(candidate: Reservation, mine: List<Reservation>, now: LocalDateTime): ReservationError? {
        if (catalog.room(candidate.roomId) == null || candidate.startMinute !in slotStarts()) {
            return ReservationError.UNKNOWN_SLOT
        }
        return when (status(candidate.roomId, candidate.date, candidate.startMinute, mine, now)) {
            SlotStatus.FREE -> null
            SlotStatus.MINE -> ReservationError.ALREADY_MINE
            SlotStatus.PAST -> ReservationError.PAST
            SlotStatus.BUSY_ELSEWHERE -> ReservationError.OVERLAP
            SlotStatus.DAILY_LIMIT -> ReservationError.DAILY_LIMIT
        }
    }

    /** Reservas que aún no han terminado, ordenadas por fecha y hora. */
    fun upcoming(mine: List<Reservation>, now: LocalDateTime): List<Reservation> =
        mine.filter { !hasEnded(it, now) }.sortedWith(compareBy({ it.date }, { it.startMinute }))

    private fun isPast(date: LocalDate, startMinute: Int, now: LocalDateTime): Boolean =
        date.atStartOfDay().plusMinutes(startMinute.toLong()) <= now

    private fun hasEnded(reservation: Reservation, now: LocalDateTime): Boolean =
        reservation.date.atStartOfDay()
            .plusMinutes((reservation.startMinute + catalog.slotMinutes).toLong()) <= now
}
