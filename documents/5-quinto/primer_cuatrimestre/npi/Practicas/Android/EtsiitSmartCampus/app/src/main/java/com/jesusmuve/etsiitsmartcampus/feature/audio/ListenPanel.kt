package com.jesusmuve.etsiitsmartcampus.feature.audio

import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.heightIn
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.OutlinedButton
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.DisposableEffect
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.rememberUpdatedState
import androidx.compose.runtime.setValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalConfiguration
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.unit.dp
import androidx.lifecycle.Lifecycle
import androidx.lifecycle.LifecycleEventObserver
import androidx.lifecycle.compose.LocalLifecycleOwner
import com.jesusmuve.etsiitsmartcampus.R
import com.jesusmuve.etsiitsmartcampus.feature.gestures.EarGestureSensorController

/**
 * «Escuchar»: lee [text] en voz alta con el botón o al acercar el móvil a la oreja.
 *
 * El botón siempre está; el gesto solo si hay sensor de proximidad. Los sensores y la
 * voz solo funcionan con la pantalla en primer plano y se liberan al salir: al
 * cambiar de pantalla o pasar a segundo plano se deja de hablar.
 */
@Composable
fun ListenPanel(text: String, modifier: Modifier = Modifier) {
    val context = LocalContext.current
    val lifecycleOwner = LocalLifecycleOwner.current
    val language = LocalConfiguration.current.locales[0]?.language ?: "es"
    val currentText by rememberUpdatedState(text)
    val currentLanguage by rememberUpdatedState(language)
    var ttsReady by remember { mutableStateOf<Boolean?>(null) }
    var failed by remember { mutableStateOf(false) }
    var speechState by remember { mutableStateOf(SpeechState.IDLE) }

    val reader = remember(context) {
        SpeechReader(
            context,
            onReadyChanged = { ready -> ttsReady = ready },
            onStateChanged = { newState -> speechState = newState },
        )
    }
    val earGesture = remember(context) {
        EarGestureSensorController(context) {
            failed = !reader.speak(currentText, currentLanguage)
        }
    }
    // Si cambia el texto (otro paso de la ruta), lo que se estaba leyendo ya no vale.
    LaunchedEffect(text) { reader.stop() }

    DisposableEffect(lifecycleOwner, reader, earGesture) {
        val observer = LifecycleEventObserver { _, event ->
            when (event) {
                Lifecycle.Event.ON_RESUME -> earGesture.start()
                Lifecycle.Event.ON_PAUSE -> {
                    earGesture.stop()
                    reader.stop()
                }
                else -> Unit
            }
        }
        lifecycleOwner.lifecycle.addObserver(observer)
        if (lifecycleOwner.lifecycle.currentState.isAtLeast(Lifecycle.State.RESUMED)) earGesture.start()
        onDispose {
            lifecycleOwner.lifecycle.removeObserver(observer)
            earGesture.stop()
            reader.shutdown()
        }
    }

    Column(modifier = modifier, verticalArrangement = Arrangement.spacedBy(4.dp)) {
        Row(horizontalArrangement = Arrangement.spacedBy(8.dp), modifier = Modifier.fillMaxWidth()) {
            // Un solo botón cambia según el estado: Escuchar → Pausar → Reanudar.
            OutlinedButton(
                onClick = {
                    when (speechState) {
                        SpeechState.IDLE -> failed = !reader.speak(currentText, currentLanguage)
                        SpeechState.SPEAKING -> reader.pause()
                        SpeechState.PAUSED -> failed = !reader.resume()
                    }
                },
                enabled = ttsReady != false,
                modifier = Modifier
                    .weight(1f)
                    .heightIn(min = 48.dp),
            ) {
                Text(
                    stringResource(
                        when (speechState) {
                            SpeechState.IDLE -> R.string.listen_button
                            SpeechState.SPEAKING -> R.string.listen_pause
                            SpeechState.PAUSED -> R.string.listen_resume
                        },
                    ),
                )
            }
            if (speechState != SpeechState.IDLE) {
                OutlinedButton(
                    onClick = { reader.stop() },
                    modifier = Modifier.heightIn(min = 48.dp),
                ) { Text(stringResource(R.string.listen_stop)) }
            }
        }
        val hint = when {
            ttsReady == false || failed -> R.string.listen_unavailable
            earGesture.available -> R.string.listen_hint_ear
            else -> R.string.listen_hint_no_proximity
        }
        Text(stringResource(hint), style = MaterialTheme.typography.bodySmall)
    }
}
