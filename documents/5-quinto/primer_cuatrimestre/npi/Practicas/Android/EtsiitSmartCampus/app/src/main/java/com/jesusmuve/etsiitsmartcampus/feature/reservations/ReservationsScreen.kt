package com.jesusmuve.etsiitsmartcampus.feature.reservations

import androidx.compose.foundation.BorderStroke
import androidx.compose.foundation.horizontalScroll
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.heightIn
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.verticalScroll
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Check
import androidx.compose.material.icons.filled.Info
import androidx.compose.material3.AlertDialog
import androidx.compose.material3.Button
import androidx.compose.material3.Card
import androidx.compose.material3.CardDefaults
import androidx.compose.material3.FilterChip
import androidx.compose.material3.Icon
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.OutlinedButton
import androidx.compose.material3.Scaffold
import androidx.compose.material3.SnackbarHost
import androidx.compose.material3.SnackbarHostState
import androidx.compose.material3.Text
import androidx.compose.material3.TextButton
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.remember
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalConfiguration
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.semantics.heading
import androidx.compose.ui.semantics.semantics
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.unit.dp
import androidx.lifecycle.compose.collectAsStateWithLifecycle
import com.jesusmuve.etsiitsmartcampus.R
import com.jesusmuve.etsiitsmartcampus.ui.components.CampusHeader
import com.jesusmuve.etsiitsmartcampus.ui.components.localized
import java.time.LocalDate
import java.time.format.DateTimeFormatter
import java.util.Locale

/**
 * Reserva de salas de estudio de la biblioteca (**demostración**): elegir día, sala y hora,
 * confirmar y ver o cancelar las propias reservas. Un aviso fijo explica que no hay servidor y
 * que la reserva solo queda en este móvil.
 */
@Composable
fun ReservationsScreen(
    viewModel: ReservationsViewModel,
    onBack: () -> Unit,
    modifier: Modifier = Modifier,
) {
    val state by viewModel.uiState.collectAsStateWithLifecycle()
    val snackbarHostState = remember { SnackbarHostState() }
    val locale = LocalConfiguration.current.locales[0] ?: Locale.getDefault()

    val messageText = state.message?.let { messageText(it, state, locale) }
    LaunchedEffect(state.message) {
        if (messageText != null) {
            snackbarHostState.showSnackbar(messageText)
            viewModel.messageShown()
        }
    }

    Scaffold(modifier = modifier, snackbarHost = { SnackbarHost(snackbarHostState) }) { innerPadding ->
        Column(
            modifier = Modifier
                .padding(innerPadding)
                .fillMaxSize()
                .verticalScroll(rememberScrollState())
                .padding(16.dp),
            verticalArrangement = Arrangement.spacedBy(14.dp),
        ) {
            CampusHeader(title = stringResource(R.string.reservations_title), onBack = onBack)
            DemoNotice()
            if (state.loadFailed) {
                Text(stringResource(R.string.reservations_load_error), color = MaterialTheme.colorScheme.error)
                return@Column
            }

            SectionTitle(R.string.reservations_day)
            Row(
                horizontalArrangement = Arrangement.spacedBy(8.dp),
                modifier = Modifier.horizontalScroll(rememberScrollState()),
            ) {
                state.days.forEachIndexed { index, day ->
                    FilterChip(
                        selected = day == state.selectedDay,
                        onClick = { viewModel.selectDay(day) },
                        label = { Text(dayLabel(day, index == 0, locale)) },
                        leadingIcon = if (day == state.selectedDay) {
                            { Icon(Icons.Filled.Check, contentDescription = null, modifier = Modifier.size(18.dp)) }
                        } else {
                            null
                        },
                    )
                }
            }

            SectionTitle(R.string.reservations_room)
            state.rooms.forEach { room ->
                RoomCard(room, selected = room.id == state.selectedRoomId, onClick = { viewModel.selectRoom(room.id) })
            }

            SectionTitle(R.string.reservations_time)
            Text(
                stringResource(R.string.reservations_rules, state.slotMinutes, state.maxSlotsPerDay),
                style = MaterialTheme.typography.bodySmall,
                color = MaterialTheme.colorScheme.onSurfaceVariant,
            )
            state.slots.chunked(3).forEach { row ->
                Row(horizontalArrangement = Arrangement.spacedBy(8.dp)) {
                    row.forEach { slot ->
                        SlotButton(slot, onClick = { viewModel.onSlotClick(slot.startMinute) }, modifier = Modifier.weight(1f))
                    }
                    repeat(3 - row.size) { Spacer(Modifier.weight(1f)) }
                }
            }

            SectionTitle(R.string.reservations_mine)
            if (state.upcoming.isEmpty()) {
                Text(stringResource(R.string.reservations_none), color = MaterialTheme.colorScheme.onSurfaceVariant)
            }
            state.upcoming.forEach { reservation ->
                MyReservationCard(reservation, state, locale, onCancel = { viewModel.cancel(reservation) })
            }
        }
    }

    state.pending?.let { pending ->
        AlertDialog(
            onDismissRequest = viewModel::dismissPending,
            title = { Text(stringResource(R.string.reservations_confirm_title)) },
            text = { Text(describe(pending, state, locale)) },
            confirmButton = { Button(onClick = viewModel::confirmPending) { Text(stringResource(R.string.reservations_confirm)) } },
            dismissButton = { TextButton(onClick = viewModel::dismissPending) { Text(stringResource(R.string.reservations_cancel_dialog)) } },
        )
    }
}

@Composable
private fun DemoNotice() {
    val scheme = MaterialTheme.colorScheme
    Card(colors = CardDefaults.cardColors(containerColor = scheme.secondaryContainer)) {
        Row(modifier = Modifier.padding(16.dp)) {
            Icon(Icons.Filled.Info, contentDescription = null, tint = scheme.onSecondaryContainer)
            Spacer(Modifier.size(12.dp))
            Text(
                stringResource(R.string.reservations_demo_notice),
                style = MaterialTheme.typography.bodyMedium,
                color = scheme.onSecondaryContainer,
            )
        }
    }
}

@Composable
private fun SectionTitle(res: Int) {
    Text(
        text = stringResource(res),
        style = MaterialTheme.typography.titleMedium,
        fontWeight = FontWeight.SemiBold,
        modifier = Modifier
            .padding(top = 4.dp)
            .semantics { heading() },
    )
}

@Composable
private fun RoomCard(room: StudyRoom, selected: Boolean, onClick: () -> Unit) {
    val scheme = MaterialTheme.colorScheme
    Card(
        onClick = onClick,
        colors = CardDefaults.cardColors(
            containerColor = if (selected) scheme.primaryContainer else scheme.surfaceVariant.copy(alpha = 0.55f),
        ),
        border = if (selected) BorderStroke(2.dp, scheme.primary) else null,
        modifier = Modifier.fillMaxWidth(),
    ) {
        Row(modifier = Modifier.padding(14.dp), verticalAlignment = Alignment.CenterVertically) {
            Column(modifier = Modifier.weight(1f)) {
                Text(localized(room.nameEs, room.nameEn), style = MaterialTheme.typography.titleMedium)
                Text(
                    stringResource(R.string.reservations_capacity, room.capacity) + " · " +
                        localized(room.descriptionEs, room.descriptionEn),
                    style = MaterialTheme.typography.bodySmall,
                    color = scheme.onSurfaceVariant,
                )
            }
            // El estado «seleccionada» también se dice con texto, no solo con color.
            if (selected) {
                Icon(Icons.Filled.Check, contentDescription = stringResource(R.string.reservations_selected), tint = scheme.primary)
            }
        }
    }
}

@Composable
private fun SlotButton(slot: SlotUi, onClick: () -> Unit, modifier: Modifier = Modifier) {
    val time = formatMinute(slot.startMinute)
    val label = when (slot.status) {
        SlotStatus.FREE -> time
        SlotStatus.MINE -> stringResource(R.string.reservations_slot_mine, time)
        SlotStatus.PAST -> stringResource(R.string.reservations_slot_past, time)
        SlotStatus.BUSY_ELSEWHERE -> stringResource(R.string.reservations_slot_busy, time)
        SlotStatus.DAILY_LIMIT -> stringResource(R.string.reservations_slot_limit, time)
    }
    val buttonModifier = modifier.heightIn(min = 52.dp)
    when (slot.status) {
        SlotStatus.FREE -> OutlinedButton(onClick = onClick, modifier = buttonModifier) { Text(label) }
        SlotStatus.MINE -> Button(onClick = onClick, modifier = buttonModifier) { Text(label) }
        // No disponibles: se pueden pulsar para saber por qué, pero se ven atenuados.
        else -> TextButton(onClick = onClick, modifier = buttonModifier) {
            Text(label, color = MaterialTheme.colorScheme.onSurfaceVariant.copy(alpha = 0.7f), style = MaterialTheme.typography.bodySmall)
        }
    }
}

@Composable
private fun MyReservationCard(reservation: Reservation, state: ReservationsUiState, locale: Locale, onCancel: () -> Unit) {
    Card(colors = CardDefaults.cardColors(containerColor = MaterialTheme.colorScheme.tertiaryContainer)) {
        Row(modifier = Modifier.padding(14.dp), verticalAlignment = Alignment.CenterVertically) {
            Text(
                describe(reservation, state, locale),
                style = MaterialTheme.typography.bodyMedium,
                color = MaterialTheme.colorScheme.onTertiaryContainer,
                modifier = Modifier.weight(1f),
            )
            TextButton(onClick = onCancel) { Text(stringResource(R.string.reservations_cancel_reservation)) }
        }
    }
}

@Composable
private fun describe(reservation: Reservation, state: ReservationsUiState, locale: Locale): String {
    val room = state.rooms.firstOrNull { it.id == reservation.roomId }
    val roomName = room?.let { localized(it.nameEs, it.nameEn) } ?: reservation.roomId
    return stringResource(
        R.string.reservations_description,
        roomName,
        longDate(reservation.date, locale),
        formatMinute(reservation.startMinute),
        formatMinute(reservation.startMinute + state.slotMinutes),
    )
}

@Composable
private fun messageText(message: ReservationMessage, state: ReservationsUiState, locale: Locale): String = when (message) {
    is ReservationMessage.Reserved -> stringResource(R.string.reservations_msg_reserved, describe(message.reservation, state, locale))
    is ReservationMessage.Cancelled -> stringResource(R.string.reservations_msg_cancelled, describe(message.reservation, state, locale))
    is ReservationMessage.Rejected -> when (message.error) {
        ReservationError.PAST -> stringResource(R.string.reservations_err_past)
        ReservationError.OVERLAP -> stringResource(R.string.reservations_err_overlap)
        ReservationError.DAILY_LIMIT -> stringResource(R.string.reservations_err_limit, state.maxSlotsPerDay)
        ReservationError.ALREADY_MINE -> stringResource(R.string.reservations_err_mine)
        ReservationError.UNKNOWN_SLOT -> stringResource(R.string.reservations_err_unknown)
    }
}

@Composable
private fun dayLabel(day: LocalDate, isToday: Boolean, locale: Locale): String =
    if (isToday) {
        stringResource(R.string.reservations_today)
    } else {
        day.format(DateTimeFormatter.ofPattern("EEE d", locale)).replaceFirstChar { it.titlecase(locale) }
    }

private fun longDate(day: LocalDate, locale: Locale): String =
    day.format(DateTimeFormatter.ofPattern("EEEE d MMMM", locale))

private fun formatMinute(minute: Int): String = "%02d:%02d".format(minute / 60, minute % 60)
