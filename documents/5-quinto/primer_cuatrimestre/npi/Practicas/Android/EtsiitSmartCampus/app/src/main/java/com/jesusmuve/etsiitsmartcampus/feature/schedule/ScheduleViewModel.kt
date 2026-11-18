package com.jesusmuve.etsiitsmartcampus.feature.schedule

import androidx.lifecycle.ViewModel
import java.time.DayOfWeek
import java.time.LocalDate
import java.time.Month
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow

/**
 * Estado de «Consultar horarios».
 *
 * @property semesterIndex Cuatrimestre elegido (0 = primero).
 * @property day Día elegido, 1 = lunes … 5 = viernes.
 * @property subgroup Subgrupo elegido o `null` para ver todos.
 * @property classes Clases de ese día con esos filtros, por hora.
 */
data class ScheduleUiState(
    val loadFailed: Boolean = false,
    val timetable: Timetable? = null,
    val semesterIndex: Int = 0,
    val day: Int = Timetable.FIRST_DAY,
    val subgroup: String? = null,
    val classes: List<ClassSession> = emptyList(),
)

/**
 * Horario del grupo: elegir cuatrimestre, día y subgrupo de prácticas. No recibe `Context`
 *.
 *
 * Al abrir, propone el cuatrimestre y el día de hoy (de septiembre a enero el primero; el fin de
 * semana, el lunes).
 *
 * @param timetable Horario; `null` si el fichero de datos no se pudo leer.
 * @param today Fecha de hoy.
 */
class ScheduleViewModel(
    private val timetable: Timetable?,
    today: LocalDate = LocalDate.now(),
) : ViewModel() {

    private val _uiState = MutableStateFlow(
        if (timetable == null) {
            ScheduleUiState(loadFailed = true)
        } else {
            build(
                semesterIndex = defaultSemester(today).coerceAtMost(timetable.semesters.lastIndex),
                day = defaultDay(today),
                subgroup = null,
            )
        },
    )
    val uiState: StateFlow<ScheduleUiState> = _uiState.asStateFlow()

    fun selectSemester(index: Int) = update(semesterIndex = index)

    fun selectDay(day: Int) = update(day = day)

    /** `null` = todos los subgrupos. */
    fun selectSubgroup(subgroup: String?) = update(subgroup = subgroup, keepSubgroup = false)

    private fun update(
        semesterIndex: Int? = null,
        day: Int? = null,
        subgroup: String? = null,
        keepSubgroup: Boolean = true,
    ) {
        val current = _uiState.value
        if (current.timetable == null) return
        _uiState.value = build(
            semesterIndex = semesterIndex ?: current.semesterIndex,
            day = day ?: current.day,
            subgroup = if (keepSubgroup) current.subgroup else subgroup,
        )
    }

    private fun build(semesterIndex: Int, day: Int, subgroup: String?): ScheduleUiState {
        val table = timetable ?: return ScheduleUiState(loadFailed = true)
        val semester = semesterIndex.coerceIn(0, table.semesters.lastIndex)
        val safeDay = day.coerceIn(Timetable.FIRST_DAY, Timetable.LAST_DAY)
        val safeSubgroup = subgroup?.takeIf { it in table.subgroups }
        return ScheduleUiState(
            timetable = table,
            semesterIndex = semester,
            day = safeDay,
            subgroup = safeSubgroup,
            classes = table.classesFor(semester, safeDay, safeSubgroup),
        )
    }

    companion object {
        /** De septiembre a enero, primer cuatrimestre; de febrero a agosto, segundo. */
        fun defaultSemester(today: LocalDate): Int = when (today.month) {
            Month.SEPTEMBER, Month.OCTOBER, Month.NOVEMBER, Month.DECEMBER, Month.JANUARY -> 0
            else -> 1
        }

        /** Hoy si es de lunes a viernes; el fin de semana, el lunes. */
        fun defaultDay(today: LocalDate): Int = when (today.dayOfWeek) {
            DayOfWeek.SATURDAY, DayOfWeek.SUNDAY -> Timetable.FIRST_DAY
            else -> today.dayOfWeek.value
        }
    }
}
