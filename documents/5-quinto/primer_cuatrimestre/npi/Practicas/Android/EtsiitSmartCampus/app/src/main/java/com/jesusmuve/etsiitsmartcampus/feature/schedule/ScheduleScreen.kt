package com.jesusmuve.etsiitsmartcampus.feature.schedule

import androidx.annotation.StringRes
import androidx.compose.foundation.horizontalScroll
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.layout.width
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.verticalScroll
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Check
import androidx.compose.material3.Card
import androidx.compose.material3.CardDefaults
import androidx.compose.material3.FilterChip
import androidx.compose.material3.Icon
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.semantics.heading
import androidx.compose.ui.semantics.semantics
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.unit.dp
import androidx.lifecycle.compose.collectAsStateWithLifecycle
import com.jesusmuve.etsiitsmartcampus.R
import com.jesusmuve.etsiitsmartcampus.ui.components.CampusHeader
import com.jesusmuve.etsiitsmartcampus.ui.components.localized

/**
 * «Consultar horarios»: horario del grupo por cuatrimestre, día y subgrupo de prácticas. Cada
 * clase muestra la hora, la asignatura (nombre completo y siglas), el aula y para quién es; al
 * pulsarla se abre la ruta interior hasta su aula.
 *
 * @param onOpenRoute Abre la ruta hacia el aula indicada (p. ej. «2.2»).
 */
@Composable
fun ScheduleScreen(
    viewModel: ScheduleViewModel,
    onBack: () -> Unit,
    modifier: Modifier = Modifier,
    onOpenRoute: (room: String) -> Unit = {},
) {
    val state by viewModel.uiState.collectAsStateWithLifecycle()

    Scaffold(modifier = modifier) { innerPadding ->
        Column(
            modifier = Modifier
                .padding(innerPadding)
                .fillMaxSize()
                .verticalScroll(rememberScrollState())
                .padding(16.dp),
            verticalArrangement = Arrangement.spacedBy(14.dp),
        ) {
            CampusHeader(title = stringResource(R.string.schedule_title), onBack = onBack)
            val timetable = state.timetable
            if (state.loadFailed || timetable == null) {
                Text(stringResource(R.string.schedule_load_error), color = MaterialTheme.colorScheme.error)
                return@Column
            }

            Text(
                localized(timetable.groupEs, timetable.groupEn),
                style = MaterialTheme.typography.titleMedium,
                color = MaterialTheme.colorScheme.primary,
            )

            SectionTitle(R.string.schedule_semester)
            ChipRow {
                timetable.semesters.indices.forEach { index ->
                    SelectChip(
                        label = stringResource(if (index == 0) R.string.schedule_semester_1 else R.string.schedule_semester_2),
                        selected = index == state.semesterIndex,
                        onClick = { viewModel.selectSemester(index) },
                    )
                }
            }

            SectionTitle(R.string.schedule_subgroup)
            ChipRow {
                SelectChip(
                    label = stringResource(R.string.schedule_subgroup_all),
                    selected = state.subgroup == null,
                    onClick = { viewModel.selectSubgroup(null) },
                )
                timetable.subgroups.forEach { subgroup ->
                    SelectChip(
                        label = subgroup,
                        selected = subgroup == state.subgroup,
                        onClick = { viewModel.selectSubgroup(subgroup) },
                    )
                }
            }

            SectionTitle(R.string.schedule_day)
            ChipRow {
                (Timetable.FIRST_DAY..Timetable.LAST_DAY).forEach { day ->
                    SelectChip(
                        label = stringResource(dayName(day)),
                        selected = day == state.day,
                        onClick = { viewModel.selectDay(day) },
                    )
                }
            }

            if (state.classes.isEmpty()) {
                Text(stringResource(R.string.schedule_none), color = MaterialTheme.colorScheme.onSurfaceVariant)
            }
            val semester = timetable.semesters.getOrNull(state.semesterIndex)
            if (state.classes.isNotEmpty()) {
                Text(
                    stringResource(R.string.schedule_tap_hint),
                    style = MaterialTheme.typography.bodySmall,
                    color = MaterialTheme.colorScheme.onSurfaceVariant,
                )
            }
            state.classes.forEach { session ->
                ClassCard(session, semester?.subject(session.subject), onClick = { onOpenRoute(session.room) })
            }
        }
    }
}

@Composable
private fun ClassCard(session: ClassSession, subject: Subject?, onClick: () -> Unit) {
    val scheme = MaterialTheme.colorScheme
    val wholeGroup = session.subgroup == null
    Card(
        onClick = onClick,
        colors = CardDefaults.cardColors(
            containerColor = if (wholeGroup) scheme.primaryContainer else scheme.surfaceVariant.copy(alpha = 0.6f),
        ),
        modifier = Modifier.fillMaxWidth(),
    ) {
        Row(modifier = Modifier.padding(14.dp), verticalAlignment = Alignment.CenterVertically) {
            Column(modifier = Modifier.width(64.dp)) {
                Text(session.start, style = MaterialTheme.typography.titleMedium, fontWeight = FontWeight.SemiBold)
                Text(session.end, style = MaterialTheme.typography.bodySmall, color = scheme.onSurfaceVariant)
            }
            Column(modifier = Modifier.weight(1f)) {
                val name = subject?.let { localized(it.nameEs, it.nameEn) } ?: session.subject
                Text("$name (${session.subject})", style = MaterialTheme.typography.titleSmall)
                // Para quién es la clase se dice con texto, no solo con el color de la tarjeta.
                val who = session.subgroup?.let { stringResource(R.string.schedule_subgroup_label, it) }
                    ?: stringResource(R.string.schedule_whole_group)
                Text(
                    stringResource(R.string.schedule_room, session.room) + " · " + who,
                    style = MaterialTheme.typography.bodyMedium,
                    color = scheme.onSurfaceVariant,
                )
                Text(
                    stringResource(R.string.schedule_how_to_get_there),
                    style = MaterialTheme.typography.labelLarge,
                    color = scheme.primary,
                    modifier = Modifier.padding(top = 4.dp),
                )
            }
        }
    }
}

@Composable
private fun ChipRow(content: @Composable () -> Unit) {
    Row(
        horizontalArrangement = Arrangement.spacedBy(8.dp),
        modifier = Modifier.horizontalScroll(rememberScrollState()),
    ) { content() }
}

@Composable
private fun SelectChip(label: String, selected: Boolean, onClick: () -> Unit) {
    FilterChip(
        selected = selected,
        onClick = onClick,
        label = { Text(label) },
        leadingIcon = if (selected) {
            { Icon(Icons.Filled.Check, contentDescription = null, modifier = Modifier.size(18.dp)) }
        } else {
            null
        },
    )
}

@Composable
private fun SectionTitle(@StringRes res: Int) {
    Text(
        text = stringResource(res),
        style = MaterialTheme.typography.titleMedium,
        fontWeight = FontWeight.SemiBold,
        modifier = Modifier.semantics { heading() },
    )
}

@StringRes
private fun dayName(day: Int): Int = when (day) {
    1 -> R.string.schedule_day_1
    2 -> R.string.schedule_day_2
    3 -> R.string.schedule_day_3
    4 -> R.string.schedule_day_4
    else -> R.string.schedule_day_5
}
