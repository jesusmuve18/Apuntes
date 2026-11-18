package com.jesusmuve.etsiitsmartcampus.feature.gestures

import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.heightIn
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.OutlinedButton
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.DisposableEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableIntStateOf
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.rememberUpdatedState
import androidx.compose.runtime.setValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.unit.dp
import androidx.lifecycle.Lifecycle
import androidx.lifecycle.LifecycleEventObserver
import androidx.lifecycle.compose.LocalLifecycleOwner
import com.jesusmuve.etsiitsmartcampus.R
import com.jesusmuve.etsiitsmartcampus.ui.components.Haptics

/**
 * Panel del gesto G1 con su botón equivalente.
 *
 * El sensor solo escucha mientras este panel está en pantalla y la Activity en primer plano
 *: se usa dentro de la fase de guiado de la ruta interior. Al reconocer el gesto o
 * pulsar el botón se llama a [onScanRequested]; quien lo usa abre el lector QR/NFC/manual.
 *
 * Tras reconocer el gesto el detector se pausa para no abrir el lector dos veces. Como el
 * estado usa `remember`, al volver del lector a la ruta el panel se recrea y el gesto vuelve a
 * estar activo sin pulsar nada.
 */
@Composable
fun ScanGesturePanel(
    onScanRequested: (ScanTrigger) -> Unit,
    modifier: Modifier = Modifier,
) {
    val context = LocalContext.current
    val lifecycleOwner = LocalLifecycleOwner.current
    val latestCallback by rememberUpdatedState(onScanRequested)
    var armed by remember { mutableStateOf(true) }
    var completedPhases by remember { mutableIntStateOf(0) }
    var pitchDegrees by remember { mutableStateOf<Int?>(null) }
    var differenceDegrees by remember { mutableStateOf<Int?>(null) }

    val sensorController = remember(context) {
        ScanGestureSensorController(
            context = context,
            onDetected = {
                armed = false
                // Confirmación táctil: el usuario nota que el gesto se ha reconocido.
                Haptics.shortPulse(context)
                latestCallback(ScanTrigger.GESTURE)
            },
            onProgress = { completedPhases = it },
            onAnglesChanged = { angle, difference ->
                pitchDegrees = angle
                differenceDegrees = difference
            },
        )
    }

    DisposableEffect(sensorController, lifecycleOwner, armed) {
        if (armed && sensorController.available) {
            val observer = LifecycleEventObserver { _, event ->
                when (event) {
                    Lifecycle.Event.ON_RESUME -> sensorController.start()
                    Lifecycle.Event.ON_PAUSE -> sensorController.stop()
                    else -> Unit
                }
            }
            lifecycleOwner.lifecycle.addObserver(observer)
            if (lifecycleOwner.lifecycle.currentState.isAtLeast(Lifecycle.State.RESUMED)) {
                sensorController.start()
            }
            onDispose {
                lifecycleOwner.lifecycle.removeObserver(observer)
                sensorController.stop()
            }
        } else {
            onDispose { sensorController.stop() }
        }
    }

    Column(modifier = modifier, verticalArrangement = Arrangement.spacedBy(8.dp)) {
        Text(
            text = when {
                !sensorController.available -> stringResource(R.string.gesture_missing_sensors)
                !armed -> stringResource(R.string.gesture_disarmed)
                else -> stringResource(R.string.gesture_instructions)
            },
            style = MaterialTheme.typography.bodySmall,
        )
        if (armed && sensorController.available) {
            Text(
                stringResource(R.string.gesture_nods_progress, completedPhases, G1_REQUIRED_PHASES),
                style = MaterialTheme.typography.bodySmall,
            )
            // Mientras toma la postura inicial se pide quietud; después, solo el contador.
            if (differenceDegrees == null) {
                Text(
                    text = stringResource(R.string.gesture_waiting_for_start),
                    style = MaterialTheme.typography.bodySmall,
                )
            }
        }
        OutlinedButton(
            onClick = { latestCallback(ScanTrigger.BUTTON) },
            modifier = Modifier
                .fillMaxWidth()
                .heightIn(min = 48.dp),
        ) {
            Text(stringResource(R.string.scanner_open_button))
        }
        if (!armed && sensorController.available) {
            OutlinedButton(
                onClick = {
                    completedPhases = 0
                    differenceDegrees = null
                    pitchDegrees = null
                    armed = true
                },
                modifier = Modifier.heightIn(min = 48.dp),
            ) {
                Text(stringResource(R.string.gesture_rearm))
            }
        }
    }
}
