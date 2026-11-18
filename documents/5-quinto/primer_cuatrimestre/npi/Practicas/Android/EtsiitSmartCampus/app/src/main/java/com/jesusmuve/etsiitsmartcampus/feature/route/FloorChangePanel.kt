package com.jesusmuve.etsiitsmartcampus.feature.route

import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Card
import androidx.compose.material3.CardDefaults
import androidx.compose.material3.LinearProgressIndicator
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.DisposableEffect
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.semantics.LiveRegionMode
import androidx.compose.ui.semantics.heading
import androidx.compose.ui.semantics.liveRegion
import androidx.compose.ui.semantics.semantics
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.unit.dp
import androidx.lifecycle.Lifecycle
import androidx.lifecycle.LifecycleEventObserver
import androidx.lifecycle.compose.LocalLifecycleOwner
import com.jesusmuve.etsiitsmartcampus.R
import com.jesusmuve.etsiitsmartcampus.ui.components.Haptics
import kotlinx.coroutines.delay
import kotlinx.coroutines.isActive
import kotlin.math.abs
import kotlin.math.roundToInt

/**
 * Detección de **cambio de planta** con el barómetro, en los tramos de escalera o ascensor.
 *
 * Al empezar el tramo toma la presión de referencia; mientras el usuario sube o baja, muestra el
 * avance y, cuando la altura corresponde a la planta de destino, vibra y lo dice. **No avanza la
 * ruta por su cuenta**: el usuario sigue confirmando con «Ya estoy aquí» o con el QR/NFC.
 * Si el móvil no tiene barómetro, lo explica y todo lo demás funciona igual.
 *
 * @param legKey Identifica el tramo: al cambiar, se toma una referencia nueva.
 * @param levelChange Plantas que hay que subir (positivo) o bajar (negativo).
 * @param floorHeightMeters Altura aproximada entre plantas.
 * @param targetFloorName Nombre de la planta de destino, ya en el idioma de la interfaz.
 */
@Composable
fun FloorChangePanel(
    legKey: Any,
    levelChange: Int,
    floorHeightMeters: Double,
    targetFloorName: String,
    modifier: Modifier = Modifier,
) {
    val context = LocalContext.current
    val lifecycleOwner = LocalLifecycleOwner.current
    val sensor = remember(context) { BarometerSensor(context) }
    val detector = remember(legKey) { FloorChangeDetector(levelChange, floorHeightMeters) }
    var reading by remember(legKey) {
        mutableStateOf(FloorChangeReading(FloorChangeStatus.CALIBRATING, 0.0, 0))
    }

    // El barómetro solo escucha con la pantalla en primer plano.
    DisposableEffect(lifecycleOwner, sensor) {
        val observer = LifecycleEventObserver { _, event ->
            when (event) {
                Lifecycle.Event.ON_RESUME -> sensor.start()
                Lifecycle.Event.ON_PAUSE -> sensor.stop()
                else -> Unit
            }
        }
        lifecycleOwner.lifecycle.addObserver(observer)
        if (lifecycleOwner.lifecycle.currentState.isAtLeast(Lifecycle.State.RESUMED)) sensor.start()
        onDispose {
            lifecycleOwner.lifecycle.removeObserver(observer)
            sensor.stop()
        }
    }

    // Lee el barómetro unas 5 veces por segundo y pasa cada lectura nueva al detector.
    LaunchedEffect(legKey) {
        var lastSample = -1L
        while (isActive) {
            val count = sensor.sampleCount
            val pressure = sensor.pressureHpa
            if (pressure != null && count != lastSample) {
                lastSample = count
                val updated = detector.onPressure(pressure)
                if (updated.status == FloorChangeStatus.REACHED && reading.status != FloorChangeStatus.REACHED) {
                    Haptics.shortPulse(context)
                }
                reading = updated
            }
            delay(POLL_MS)
        }
    }

    val reached = reading.status == FloorChangeStatus.REACHED
    Card(
        modifier = modifier.fillMaxWidth(),
        colors = if (reached) {
            CardDefaults.cardColors(containerColor = ReachedGreen.copy(alpha = 0.12f))
        } else {
            CardDefaults.cardColors()
        },
    ) {
        Column(
            modifier = Modifier.padding(16.dp),
            verticalArrangement = Arrangement.spacedBy(8.dp),
        ) {
            Text(
                text = stringResource(R.string.floor_change_title),
                style = MaterialTheme.typography.titleMedium,
                modifier = Modifier.semantics { heading() },
            )
            if (!sensor.available) {
                Text(stringResource(R.string.floor_change_no_sensor), style = MaterialTheme.typography.bodyMedium)
                return@Column
            }

            val expectedMeters = abs(levelChange * floorHeightMeters)
            val climbedMeters = abs(reading.metersChanged)
            val goingUp = levelChange > 0
            val statusText = when (reading.status) {
                FloorChangeStatus.CALIBRATING -> stringResource(R.string.floor_change_calibrating)
                FloorChangeStatus.WAITING -> stringResource(
                    if (goingUp) R.string.floor_change_waiting_up else R.string.floor_change_waiting_down,
                    targetFloorName,
                )
                FloorChangeStatus.IN_PROGRESS -> stringResource(
                    if (goingUp) R.string.floor_change_progress_up else R.string.floor_change_progress_down,
                    climbedMeters.roundToInt(),
                )
                FloorChangeStatus.REACHED -> stringResource(R.string.floor_change_reached, targetFloorName)
                FloorChangeStatus.WRONG_DIRECTION -> stringResource(
                    if (goingUp) R.string.floor_change_wrong_should_go_up else R.string.floor_change_wrong_should_go_down,
                )
            }
            Text(
                text = statusText,
                style = if (reached) MaterialTheme.typography.titleMedium else MaterialTheme.typography.bodyLarge,
                fontWeight = if (reached) FontWeight.Bold else FontWeight.Normal,
                color = when (reading.status) {
                    FloorChangeStatus.REACHED -> ReachedGreen
                    FloorChangeStatus.WRONG_DIRECTION -> MaterialTheme.colorScheme.error
                    else -> MaterialTheme.colorScheme.onSurface
                },
                modifier = Modifier.semantics { liveRegion = LiveRegionMode.Polite },
            )
            if (reading.status != FloorChangeStatus.CALIBRATING && reading.status != FloorChangeStatus.WRONG_DIRECTION) {
                val progress = if (expectedMeters > 0.0) (climbedMeters / expectedMeters).toFloat() else 0f
                LinearProgressIndicator(
                    progress = { if (reading.status == FloorChangeStatus.WAITING) 0f else progress.coerceIn(0f, 1f) },
                    modifier = Modifier.fillMaxWidth(),
                )
            }
            Text(stringResource(R.string.floor_change_hint), style = MaterialTheme.typography.bodySmall)
        }
    }
}

/** Verde de «planta alcanzada», con contraste suficiente sobre fondo claro. */
private val ReachedGreen = Color(0xFF1F7A4D)

private const val POLL_MS = 200L
