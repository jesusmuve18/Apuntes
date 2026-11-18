package com.jesusmuve.etsiitsmartcampus.feature.route

import android.hardware.SensorManager
import androidx.compose.foundation.Canvas
import androidx.compose.foundation.gestures.detectTapGestures
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.heightIn
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.layout.width
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Check
import androidx.compose.material3.Button
import androidx.compose.material3.Card
import androidx.compose.material3.CardDefaults
import androidx.compose.material3.Icon
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.OutlinedButton
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.DisposableEffect
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.geometry.Offset
import androidx.compose.ui.geometry.Size
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.Path
import androidx.compose.ui.graphics.StrokeCap
import androidx.compose.ui.graphics.drawscope.DrawScope
import androidx.compose.ui.graphics.drawscope.Stroke
import androidx.compose.ui.graphics.drawscope.rotate
import androidx.compose.ui.input.pointer.pointerInput
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.semantics.LiveRegionMode
import androidx.compose.ui.semantics.contentDescription
import androidx.compose.ui.semantics.heading
import androidx.compose.ui.semantics.liveRegion
import androidx.compose.ui.semantics.semantics
import androidx.compose.ui.text.TextStyle
import androidx.compose.ui.text.drawText
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.text.rememberTextMeasurer
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import androidx.lifecycle.Lifecycle
import androidx.lifecycle.LifecycleEventObserver
import androidx.lifecycle.compose.LocalLifecycleOwner
import com.jesusmuve.etsiitsmartcampus.R
import com.jesusmuve.etsiitsmartcampus.ui.components.Haptics
import kotlinx.coroutines.delay
import kotlinx.coroutines.isActive
import kotlin.math.roundToInt

/** Estado que se enseña al usuario (sin números). */
private enum class TrackState { ON_TRACK, TURN_LEFT, TURN_RIGHT, TURN_AROUND, UNAVAILABLE }

/**
 * «¿Voy bien?» en la fase de guiado: una brújula con el **rumbo real** del tramo.
 *
 * 1. El usuario sujeta el móvil delante (plano o en vertical) y pulsa «Calibrar».
 * 2. Aparecen dos flechas: la **fina** es hacia dónde apunta el móvil (siempre hacia arriba en la
 *    brújula) y la **gruesa** es hacia dónde está el siguiente punto, calculado con las posiciones
 *    de los dos puntos en el plano y la orientación real del edificio ([PlanBearing]). Si se
 *    solapan (±20°), las dos y el aro se ponen **verdes**, vibra una vez y dice «Vas bien»; si no,
 *    se ponen **rojas** y dice hacia dónde girar. Una «N» en el aro marca el norte.
 *
 * Las flechas se distinguen por forma y por texto, no solo por color.
 *
 * @param legKey Identifica el tramo: al cambiar, hay que volver a pulsar «Calibrar».
 * @param targetBearingDegrees Rumbo real del tramo; `null` si el tramo no tiene dirección útil.
 * @param latitude Latitud del edificio (declinación magnética).
 * @param longitude Longitud del edificio.
 */
@Composable
fun OrientationPanel(
    legKey: Any,
    targetBearingDegrees: Double?,
    latitude: Double,
    longitude: Double,
    modifier: Modifier = Modifier,
) {
    val context = LocalContext.current
    val lifecycleOwner = LocalLifecycleOwner.current
    val sensor = remember(context, latitude, longitude) { RotationVectorSensor(context, latitude, longitude) }
    val checker = remember { OrientationChecker() }
    // Modo de prueba (mantener pulsado el título): enseña los números y permite apuntar al norte.
    var diagnostics by remember { mutableStateOf(false) }
    var testNorth by remember(legKey) { mutableStateOf(false) }
    val target = if (testNorth) 0f else targetBearingDegrees?.toFloat()
    var active by remember(legKey) { mutableStateOf(false) }
    var calibrationFailed by remember(legKey) { mutableStateOf(false) }
    var heading by remember(legKey) { mutableStateOf<Float?>(null) }
    var trackState by remember(legKey) { mutableStateOf(TrackState.UNAVAILABLE) }

    // El sensor solo escucha con la pantalla en primer plano.
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

    // Tras calibrar (o con el modo de prueba abierto), se lee la brújula unas 10 veces por segundo.
    LaunchedEffect(legKey, active, target, diagnostics) {
        if (!active && !diagnostics) return@LaunchedEffect
        while (isActive) {
            val current = sensor.headingDegrees
            val newState = when (checker.check(target, current, sensor.reliable)) {
                OrientationResult.UNAVAILABLE -> TrackState.UNAVAILABLE
                OrientationResult.ALIGNED -> TrackState.ON_TRACK
                OrientationResult.NOT_ALIGNED -> when (checker.turnHint(target, current)) {
                    TurnHint.LEFT -> TrackState.TURN_LEFT
                    TurnHint.RIGHT -> TrackState.TURN_RIGHT
                    TurnHint.TURN_AROUND -> TrackState.TURN_AROUND
                    null -> TrackState.ON_TRACK
                }
            }
            if (active && newState == TrackState.ON_TRACK && trackState != TrackState.ON_TRACK) {
                Haptics.shortPulse(context)
            }
            trackState = newState
            heading = current
            delay(SENSOR_POLL_MS)
        }
    }

    fun calibrate() {
        sensor.resetSmoothing()
        val ok = sensor.reliable
        active = ok
        calibrationFailed = !ok
    }

    Card(modifier = modifier.fillMaxWidth()) {
        Column(
            modifier = Modifier.padding(16.dp),
            verticalArrangement = Arrangement.spacedBy(10.dp),
            horizontalAlignment = Alignment.CenterHorizontally,
        ) {
            Text(
                text = stringResource(R.string.orientation_title),
                style = MaterialTheme.typography.titleMedium,
                modifier = Modifier
                    .fillMaxWidth()
                    .semantics { heading() }
                    .pointerInput(Unit) {
                        detectTapGestures(onLongPress = { diagnostics = !diagnostics })
                    },
            )
            if (diagnostics && sensor.available) {
                DiagnosticsBox(
                    headingDegrees = heading,
                    targetDegrees = target,
                    upright = sensor.upright,
                    accuracyLevel = sensor.accuracyLevel,
                    declination = sensor.declination,
                    testNorth = testNorth,
                    onToggleNorth = {
                        testNorth = !testNorth
                        active = true
                    },
                )
            }
            if (!sensor.available) {
                Text(stringResource(R.string.orientation_no_sensor), modifier = Modifier.fillMaxWidth())
                return@Column
            }
            if (target == null) {
                Text(stringResource(R.string.orientation_no_direction), modifier = Modifier.fillMaxWidth())
                return@Column
            }

            if (!active) {
                Text(
                    stringResource(R.string.orientation_hint),
                    style = MaterialTheme.typography.bodyMedium,
                    modifier = Modifier.fillMaxWidth(),
                )
                if (calibrationFailed) {
                    Text(
                        stringResource(R.string.orientation_calibration_failed),
                        style = MaterialTheme.typography.bodySmall,
                        color = MaterialTheme.colorScheme.error,
                        modifier = Modifier.fillMaxWidth(),
                    )
                }
                Button(
                    onClick = { calibrate() },
                    modifier = Modifier
                        .fillMaxWidth()
                        .heightIn(min = 48.dp),
                ) { Text(stringResource(R.string.orientation_calibrate)) }
                return@Column
            }

            val statusText = stringResource(
                when (trackState) {
                    TrackState.ON_TRACK -> R.string.orientation_on_track
                    TrackState.TURN_LEFT -> R.string.orientation_turn_left
                    TrackState.TURN_RIGHT -> R.string.orientation_turn_right
                    TrackState.TURN_AROUND -> R.string.orientation_turn_around
                    TrackState.UNAVAILABLE -> R.string.orientation_unavailable
                },
            )
            CompassDial(
                targetDegrees = target,
                headingDegrees = heading,
                state = trackState,
                toleranceDegrees = checker.toleranceDegrees,
                modifier = Modifier
                    .size(DIAL_SIZE)
                    .semantics { contentDescription = statusText },
            )
            StatusLine(trackState, statusText)
            Legend()
            OutlinedButton(
                onClick = { calibrate() },
                modifier = Modifier
                    .fillMaxWidth()
                    .heightIn(min = 48.dp),
            ) { Text(stringResource(R.string.orientation_recalibrate)) }
        }
    }
}

/**
 * Modo de prueba de la brújula: muestra los números que la interfaz normal oculta, para comprobar
 * en cualquier sitio (comparando con Google Maps u otra brújula) si el móvil mide bien. Con
 * «Probar apuntando al norte», el destino pasa a ser el norte: al ponerse verde, el borde de
 * arriba del móvil (o la cámara, en vertical) debe mirar al norte de verdad.
 */
@Composable
private fun DiagnosticsBox(
    headingDegrees: Float?,
    targetDegrees: Float?,
    upright: Boolean,
    accuracyLevel: Int,
    declination: Float,
    testNorth: Boolean,
    onToggleNorth: () -> Unit,
) {
    val unknown = stringResource(R.string.orientation_diag_unknown)
    val cardinals = stringResource(R.string.orientation_diag_cardinals).split(",")
    val headingText = headingDegrees?.let {
        stringResource(R.string.orientation_diag_degrees, it.roundToInt(), cardinal(it, cardinals))
    } ?: unknown
    val targetText = targetDegrees?.let {
        stringResource(R.string.orientation_diag_degrees, it.roundToInt(), cardinal(it, cardinals))
    } ?: unknown
    val posture = stringResource(if (upright) R.string.orientation_diag_upright else R.string.orientation_diag_flat)
    val accuracy = stringResource(
        when (accuracyLevel) {
            SensorManager.SENSOR_STATUS_ACCURACY_HIGH -> R.string.orientation_diag_accuracy_high
            SensorManager.SENSOR_STATUS_ACCURACY_MEDIUM -> R.string.orientation_diag_accuracy_medium
            SensorManager.SENSOR_STATUS_ACCURACY_LOW -> R.string.orientation_diag_accuracy_low
            else -> R.string.orientation_diag_accuracy_unreliable
        },
    )
    Card(
        colors = CardDefaults.cardColors(containerColor = MaterialTheme.colorScheme.surfaceVariant),
        modifier = Modifier.fillMaxWidth(),
    ) {
        Column(modifier = Modifier.padding(12.dp), verticalArrangement = Arrangement.spacedBy(4.dp)) {
            Text(stringResource(R.string.orientation_diag_title), style = MaterialTheme.typography.labelLarge)
            Text(stringResource(R.string.orientation_diag_heading, headingText), style = MaterialTheme.typography.bodySmall)
            Text(stringResource(R.string.orientation_diag_target, targetText), style = MaterialTheme.typography.bodySmall)
            Text(stringResource(R.string.orientation_diag_posture, posture), style = MaterialTheme.typography.bodySmall)
            Text(stringResource(R.string.orientation_diag_accuracy, accuracy), style = MaterialTheme.typography.bodySmall)
            Text(
                stringResource(R.string.orientation_diag_declination, declination),
                style = MaterialTheme.typography.bodySmall,
            )
            OutlinedButton(onClick = onToggleNorth, modifier = Modifier.heightIn(min = 48.dp)) {
                Text(
                    stringResource(
                        if (testNorth) R.string.orientation_diag_back_to_leg else R.string.orientation_diag_test_north,
                    ),
                )
            }
        }
    }
}

/** Punto cardinal aproximado de un rumbo; [names] son los 8 puntos empezando por el norte. */
private fun cardinal(degrees: Float, names: List<String>): String {
    if (names.size != 8) return ""
    return names[((OrientationChecker.normalize(degrees) + 22.5f) / 45f).toInt() % names.size]
}

/** Texto grande del estado, con icono de «correcto» cuando se va bien (no solo color). */
@Composable
private fun StatusLine(state: TrackState, text: String) {
    val color = when (state) {
        TrackState.ON_TRACK -> OnTrackGreen
        TrackState.UNAVAILABLE -> MaterialTheme.colorScheme.onSurfaceVariant
        else -> OffTrackRed
    }
    Row(
        verticalAlignment = Alignment.CenterVertically,
        modifier = Modifier.semantics { liveRegion = LiveRegionMode.Polite },
    ) {
        if (state == TrackState.ON_TRACK) {
            Icon(Icons.Filled.Check, contentDescription = null, tint = color)
            Spacer(Modifier.width(6.dp))
        }
        Text(text, style = MaterialTheme.typography.titleLarge, fontWeight = FontWeight.Bold, color = color)
    }
}

/** Leyenda de las dos flechas, para no depender del color. */
@Composable
private fun Legend() {
    Column(verticalArrangement = Arrangement.spacedBy(2.dp), modifier = Modifier.fillMaxWidth()) {
        Text(stringResource(R.string.orientation_legend_target), style = MaterialTheme.typography.bodySmall)
        Text(stringResource(R.string.orientation_legend_phone), style = MaterialTheme.typography.bodySmall)
        Text(stringResource(R.string.orientation_legend_colors), style = MaterialTheme.typography.bodySmall)
    }
}

/**
 * Brújula que gira con el usuario: la flecha fina (móvil) apunta siempre arriba y la gruesa
 * (siguiente punto) gira según `rumbo del tramo − rumbo del móvil`. La «N» del aro marca el norte.
 * Todo verde si están alineadas; todo rojo si no.
 */
@Composable
private fun CompassDial(
    targetDegrees: Float,
    headingDegrees: Float?,
    state: TrackState,
    toleranceDegrees: Float,
    modifier: Modifier = Modifier,
) {
    // Ángulo de la flecha del destino respecto a la parte de arriba (hacia donde apunta el móvil).
    val relative = headingDegrees?.let { targetDegrees - it } ?: 0f
    // Sin animación: el rumbo ya llega suavizado (HeadingSmoother) y animar el ángulo haría dar
    // una vuelta entera al cruzar de 179° a −179°.
    val targetRotation = shortestTarget(relative)
    val northRotation = shortestTarget(-(headingDegrees ?: 0f))
    val scheme = MaterialTheme.colorScheme
    val accent = when (state) {
        TrackState.ON_TRACK -> OnTrackGreen
        TrackState.UNAVAILABLE -> scheme.outline
        else -> OffTrackRed
    }
    val face = accent.copy(alpha = 0.10f)
    val textMeasurer = rememberTextMeasurer()
    val northLabel = stringResource(R.string.orientation_north_short)
    Box(modifier = modifier, contentAlignment = Alignment.Center) {
        Canvas(modifier = Modifier.size(DIAL_SIZE)) {
            val r = size.minDimension / 2f
            drawCircle(color = face, radius = r)
            drawCircle(color = accent, radius = r - 3.dp.toPx(), style = Stroke(width = 4.dp.toPx()))
            // Marca del norte en el aro.
            rotate(northRotation) {
                val label = textMeasurer.measure(
                    northLabel,
                    style = TextStyle(fontSize = 14.sp, fontWeight = FontWeight.Bold, color = scheme.onSurfaceVariant),
                )
                drawText(
                    label,
                    topLeft = Offset(center.x - label.size.width / 2f, center.y - r + 8.dp.toPx()),
                )
            }
            if (headingDegrees != null) {
                // Margen de «vas bien» alrededor de la dirección del siguiente punto.
                rotate(targetRotation) {
                    drawArc(
                        color = accent.copy(alpha = 0.18f),
                        startAngle = -90f - toleranceDegrees,
                        sweepAngle = 2 * toleranceDegrees,
                        useCenter = true,
                        topLeft = Offset(center.x - r * 0.88f, center.y - r * 0.88f),
                        size = Size(r * 1.76f, r * 1.76f),
                    )
                    drawBoldArrow(accent, r * 0.72f)
                }
            }
            // Flecha del móvil: fina, siempre hacia arriba.
            drawThinArrow(accent, r * 0.8f)
            drawCircle(color = scheme.surface, radius = 9.dp.toPx())
            drawCircle(color = accent, radius = 9.dp.toPx(), style = Stroke(width = 2.dp.toPx()))
        }
    }
}

/** Lleva un ángulo a (-180, 180] para que la animación gire por el camino corto. */
private fun shortestTarget(degrees: Float): Float {
    val n = OrientationChecker.normalize(degrees)
    return if (n > 180f) n - 360f else n
}

private fun DrawScope.drawThinArrow(color: Color, length: Float) {
    val tip = Offset(center.x, center.y - length)
    drawLine(color, center, tip, strokeWidth = 4.dp.toPx(), cap = StrokeCap.Round)
    val head = 14.dp.toPx()
    drawLine(color, tip, Offset(tip.x - head * 0.6f, tip.y + head), strokeWidth = 4.dp.toPx(), cap = StrokeCap.Round)
    drawLine(color, tip, Offset(tip.x + head * 0.6f, tip.y + head), strokeWidth = 4.dp.toPx(), cap = StrokeCap.Round)
}

private fun DrawScope.drawBoldArrow(color: Color, length: Float) {
    val w = 14.dp.toPx()
    val headLength = 30.dp.toPx()
    val path = Path().apply {
        moveTo(center.x, center.y - length) // punta
        lineTo(center.x + w * 1.6f, center.y - length + headLength)
        lineTo(center.x + w * 0.5f, center.y - length + headLength)
        lineTo(center.x + w * 0.5f, center.y + length * 0.25f)
        lineTo(center.x - w * 0.5f, center.y + length * 0.25f)
        lineTo(center.x - w * 0.5f, center.y - length + headLength)
        lineTo(center.x - w * 1.6f, center.y - length + headLength)
        close()
    }
    drawPath(path, color.copy(alpha = 0.85f))
}

/** Verde de «vas bien», con contraste suficiente sobre fondo claro. */
private val OnTrackGreen = Color(0xFF1F7A4D)

/** Rojo de «no vas bien», con contraste suficiente sobre fondo claro. */
private val OffTrackRed = Color(0xFFC62828)

private val DIAL_SIZE = 220.dp

private const val SENSOR_POLL_MS = 100L
