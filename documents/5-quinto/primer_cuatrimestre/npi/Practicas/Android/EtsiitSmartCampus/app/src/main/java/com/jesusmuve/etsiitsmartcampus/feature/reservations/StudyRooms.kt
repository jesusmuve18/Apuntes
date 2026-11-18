package com.jesusmuve.etsiitsmartcampus.feature.reservations

import kotlinx.serialization.SerializationException
import kotlinx.serialization.Serializable
import kotlinx.serialization.json.Json

/** Sala de estudio de la biblioteca (datos de ejemplo de `assets/library/study_rooms.json`). */
@Serializable
data class StudyRoom(
    val id: String,
    val nameEs: String,
    val nameEn: String,
    val capacity: Int,
    val descriptionEs: String,
    val descriptionEn: String,
)

/**
 * Salas y normas de reserva.
 *
 * @property openTime Hora de apertura «HH:mm».
 * @property closeTime Hora de cierre «HH:mm» (el último tramo termina a esta hora).
 * @property slotMinutes Duración de cada tramo reservable.
 * @property daysAhead Días que se pueden reservar contando hoy.
 * @property maxSlotsPerDay Tramos que puede reservar una persona en un mismo día.
 */
@Serializable
data class StudyRoomsCatalog(
    val note: String = "",
    val openTime: String,
    val closeTime: String,
    val slotMinutes: Int,
    val daysAhead: Int,
    val maxSlotsPerDay: Int,
    val rooms: List<StudyRoom>,
) {
    /** Minuto del día de apertura y cierre (p. ej. 08:30 → 510). */
    val openMinute: Int get() = minuteOfDay(openTime)
    val closeMinute: Int get() = minuteOfDay(closeTime)

    fun room(id: String): StudyRoom? = rooms.firstOrNull { it.id == id }

    companion object {
        private val json = Json { ignoreUnknownKeys = false }

        /** Convierte «HH:mm» en minutos desde medianoche. */
        fun minuteOfDay(time: String): Int {
            val (h, m) = time.trim().split(":").map { it.toInt() }
            return h * 60 + m
        }

        /**
         * Lee y valida el JSON. Devuelve `null` si no se puede leer o los datos no son coherentes
         * (sin salas, horario al revés, tramos que no caben…), para mostrar un error controlado.
         */
        fun parse(text: String): StudyRoomsCatalog? {
            val catalog = try {
                json.decodeFromString(serializer(), text)
            } catch (e: SerializationException) {
                return null
            } catch (e: IllegalArgumentException) {
                return null
            }
            val valid = try {
                catalog.rooms.isNotEmpty() &&
                    catalog.rooms.map { it.id }.toSet().size == catalog.rooms.size &&
                    catalog.slotMinutes > 0 &&
                    catalog.daysAhead > 0 &&
                    catalog.maxSlotsPerDay > 0 &&
                    catalog.closeMinute - catalog.openMinute >= catalog.slotMinutes
            } catch (e: RuntimeException) {
                false // hora con formato incorrecto
            }
            return if (valid) catalog else null
        }
    }
}
