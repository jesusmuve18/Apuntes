package com.jesusmuve.etsiitsmartcampus.feature.schedule

import kotlinx.serialization.SerializationException
import kotlinx.serialization.Serializable
import kotlinx.serialization.json.Json

/** Asignatura del horario: siglas y nombre completo (ES/EN). */
@Serializable
data class Subject(
    val code: String,
    val nameEs: String,
    val nameEn: String,
)

/**
 * Una clase del horario.
 *
 * @property day Día de la semana, 1 = lunes … 5 = viernes.
 * @property start Hora de inicio «HH:mm».
 * @property end Hora de fin «HH:mm».
 * @property subject Siglas de la asignatura ([Subject.code]).
 * @property room Aula, p. ej. «0.7».
 * @property subgroup Subgrupo de prácticas (A1, A2, A3) o `null` si es para todo el grupo.
 */
@Serializable
data class ClassSession(
    val day: Int,
    val start: String,
    val end: String,
    val subject: String,
    val room: String,
    val subgroup: String? = null,
)

/** Un cuatrimestre: sus asignaturas y sus clases. */
@Serializable
data class Semester(
    val id: String,
    val subjects: List<Subject>,
    val classes: List<ClassSession>,
) {
    fun subject(code: String): Subject? = subjects.firstOrNull { it.code == code }
}

/**
 * Horario de un grupo (`assets/schedule/timetable.json`).
 *
 * @property subgroups Subgrupos de prácticas del grupo, en orden.
 */
@Serializable
data class Timetable(
    val groupEs: String,
    val groupEn: String,
    val subgroups: List<String>,
    val semesters: List<Semester>,
) {

    /**
     * Clases de un día, ordenadas por hora. Con [subgroup] `null` salen todas; con un subgrupo,
     * las de todo el grupo más las de ese subgrupo.
     */
    fun classesFor(semesterIndex: Int, day: Int, subgroup: String?): List<ClassSession> {
        val semester = semesters.getOrNull(semesterIndex) ?: return emptyList()
        return semester.classes
            .filter { it.day == day && (subgroup == null || it.subgroup == null || it.subgroup == subgroup) }
            .sortedWith(compareBy({ minuteOfDay(it.start) }, { it.subgroup.orEmpty() }))
    }

    companion object {
        const val FIRST_DAY = 1
        const val LAST_DAY = 5

        private val json = Json { ignoreUnknownKeys = false }

        /** Convierte «HH:mm» en minutos desde medianoche. */
        fun minuteOfDay(time: String): Int {
            val (h, m) = time.trim().split(":").map { it.toInt() }
            return h * 60 + m
        }

        /**
         * Lee y valida el JSON. Devuelve `null` si no se puede leer o no es coherente (sin
         * cuatrimestres, día fuera de lunes–viernes, asignatura o subgrupo desconocidos, hora
         * mal escrita o al revés), para que la pantalla muestre un error controlado.
         */
        fun parse(text: String): Timetable? {
            val timetable = try {
                json.decodeFromString(serializer(), text)
            } catch (e: SerializationException) {
                return null
            } catch (e: IllegalArgumentException) {
                return null
            }
            val valid = try {
                timetable.semesters.isNotEmpty() && timetable.semesters.all { semester ->
                    semester.classes.all { session ->
                        session.day in FIRST_DAY..LAST_DAY &&
                            semester.subject(session.subject) != null &&
                            (session.subgroup == null || session.subgroup in timetable.subgroups) &&
                            minuteOfDay(session.end) > minuteOfDay(session.start)
                    }
                }
            } catch (e: RuntimeException) {
                false // hora con formato incorrecto
            }
            return if (valid) timetable else null
        }
    }
}
