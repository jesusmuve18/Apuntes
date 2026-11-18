package com.jesusmuve.etsiitsmartcampus.feature.route

import androidx.compose.foundation.Canvas
import androidx.compose.foundation.Image
import androidx.compose.foundation.gestures.awaitEachGesture
import androidx.compose.foundation.gestures.awaitFirstDown
import androidx.compose.foundation.gestures.calculateCentroid
import androidx.compose.foundation.gestures.calculatePan
import androidx.compose.foundation.gestures.calculateZoom
import androidx.compose.foundation.gestures.detectTapGestures
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.FlowRow
import androidx.compose.foundation.layout.ExperimentalLayoutApi
import androidx.compose.foundation.layout.aspectRatio
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.heightIn
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.OutlinedButton
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableFloatStateOf
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.rememberUpdatedState
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.clipToBounds
import androidx.compose.ui.geometry.Offset
import androidx.compose.ui.geometry.isUnspecified
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.drawscope.Stroke
import androidx.compose.ui.graphics.graphicsLayer
import androidx.compose.ui.input.pointer.pointerInput
import androidx.compose.ui.input.pointer.positionChanged
import androidx.compose.ui.layout.ContentScale
import androidx.compose.ui.layout.onSizeChanged
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.semantics.contentDescription
import androidx.compose.ui.semantics.semantics
import androidx.compose.ui.unit.IntSize
import androidx.compose.ui.unit.dp
import com.jesusmuve.etsiitsmartcampus.R
import com.jesusmuve.etsiitsmartcampus.ui.components.AssetImageState
import com.jesusmuve.etsiitsmartcampus.ui.components.rememberAssetImage
import kotlin.math.abs
import kotlin.math.max

/** Tipo de marca sobre el plano. La forma es distinta en cada tipo: no se distinguen solo por color. */
enum class PlanMarkerStyle {
    /** Punto en el que está el usuario: círculo relleno. */
    CURRENT,

    /** Siguiente punto que debe buscar: anillo con punto central. */
    TARGET,
}

/** Marca sobre el plano, en coordenadas normalizadas 0..1 de la imagen. */
data class PlanMarker(val x: Float, val y: Float, val style: PlanMarkerStyle)

/** Parámetros de la interacción multitáctil, agrupados para ajustarlos en un solo sitio. */
private object PlanGestureConfig {
    const val MIN_SCALE = 1f
    const val MAX_SCALE = 5f

    /** Zoom al pulsar «Acercar»/«Alejar» o al hacer doble toque. */
    const val STEP_SCALE = 1.6f
    const val DOUBLE_TAP_SCALE = 2.5f

    /** Escala con la que se centra el plano en el siguiente punto. */
    const val FOCUS_SCALE = 2.5f

    /** Un toque con dos dedos debe ser breve: más tiempo se considera otro gesto. */
    const val TWO_FINGER_TAP_MAX_MS = 300L
}

/**
 * Plano de una planta con marcas y **interacción multitáctil** (sensor «interacción
 * multitáctil» del guion, ).
 *
 * Gestos reconocidos, con gestión propia de los punteros (`awaitEachGesture`), igual que en el
 * ejemplo de la asignatura con `MotionEvent`/`ACTION_POINTER_DOWN`:
 * - **Pellizcar con dos dedos:** ampliar o reducir alrededor del punto medio entre los dedos.
 * - **Arrastrar con dos dedos** (o con uno si ya está ampliado): desplazar el plano.
 * - **Toque breve con dos dedos a la vez:** centrar el plano en el siguiente punto de la ruta.
 * - **Doble toque:** ampliar en ese punto o volver a la vista completa.
 *
 * Con el plano sin ampliar, un dedo no se captura, para que la pantalla pueda desplazarse.
 * Todos los gestos tienen botón equivalente: «Acercar», «Alejar»,
 * «Ver siguiente punto» y «Ver plano completo».
 *
 * @param planAsset Imagen del plano dentro de `assets/`.
 * @param markers Marcas que se dibujan sobre el plano.
 * @param focus Punto (0..1) en el que se centra el plano con el toque de dos dedos o el botón;
 * normalmente el siguiente punto de la ruta.
 * @param description Descripción para TalkBack de lo que muestra el plano.
 */
@OptIn(ExperimentalLayoutApi::class)
@Composable
fun ZoomablePlan(
    planAsset: String,
    markers: List<PlanMarker>,
    focus: Offset?,
    description: String,
    modifier: Modifier = Modifier,
) {
    val image by rememberAssetImage(planAsset)

    // Estado de la transformación. Se reinicia al cambiar de plano (otra planta).
    var scale by remember(planAsset) { mutableFloatStateOf(1f) }
    var offset by remember(planAsset) { mutableStateOf(Offset.Zero) }
    var viewport by remember { mutableStateOf(IntSize.Zero) }
    val currentFocus by rememberUpdatedState(focus)

    /** Limita el desplazamiento para que el plano nunca deje huecos vacíos en el visor. */
    fun clamp(candidate: Offset, newScale: Float): Offset {
        val maxX = (newScale - 1f) * viewport.width / 2f
        val maxY = (newScale - 1f) * viewport.height / 2f
        return Offset(candidate.x.coerceIn(-maxX, maxX), candidate.y.coerceIn(-maxY, maxY))
    }

    /**
     * Aplica un zoom manteniendo fijo el punto [anchor] (relativo al centro del visor): el
     * contenido que estaba bajo los dedos sigue bajo los dedos.
     */
    fun zoomAround(anchor: Offset, zoomFactor: Float, pan: Offset = Offset.Zero) {
        val newScale = (scale * zoomFactor).coerceIn(PlanGestureConfig.MIN_SCALE, PlanGestureConfig.MAX_SCALE)
        val ratio = newScale / scale
        val newOffset = anchor - (anchor - offset) * ratio + pan
        scale = newScale
        offset = clamp(newOffset, newScale)
    }

    /** Centra el plano en un punto normalizado del plano. */
    fun centerOn(point: Offset, targetScale: Float) {
        val relative = Offset(
            (point.x - 0.5f) * viewport.width,
            (point.y - 0.5f) * viewport.height,
        )
        scale = targetScale
        offset = clamp(-relative * targetScale, targetScale)
    }

    fun reset() {
        scale = 1f
        offset = Offset.Zero
    }

    Column(modifier = modifier.fillMaxWidth()) {
        val aspect = (image as? AssetImageState.Ready)?.bitmap?.let { it.width.toFloat() / it.height } ?: (4f / 3f)
        Box(
            modifier = Modifier
                .fillMaxWidth()
                .aspectRatio(aspect)
                .clipToBounds()
                .onSizeChanged { viewport = it }
                .semantics { contentDescription = description }
                .pointerInput(planAsset) {
                    awaitEachGesture {
                        val firstDown = awaitFirstDown(requireUnconsumed = false)
                        val startTime = firstDown.uptimeMillis
                        var endTime = startTime
                        var maxPointers = 1
                        var movement = 0f
                        do {
                            val event = awaitPointerEvent()
                            val pressedCount = event.changes.count { it.pressed }
                            maxPointers = max(maxPointers, pressedCount)
                            endTime = event.changes.maxOfOrNull { it.uptimeMillis } ?: endTime

                            val zoomChange = event.calculateZoom()
                            val panChange = event.calculatePan()
                            movement += panChange.getDistance() + abs(1f - zoomChange) * size.width

                            // Dos o más dedos siempre se capturan; uno solo, únicamente si el
                            // plano está ampliado (si no, se deja pasar al desplazamiento de la página).
                            val handle = pressedCount >= 2 || (pressedCount == 1 && scale > 1f)
                            if (handle) {
                                val centroid = event.calculateCentroid(useCurrent = true)
                                val anchor = if (centroid.isUnspecified) {
                                    Offset.Zero
                                } else {
                                    centroid - Offset(size.width / 2f, size.height / 2f)
                                }
                                zoomAround(anchor, zoomChange, panChange)
                                event.changes.forEach { change ->
                                    if (change.positionChanged()) change.consume()
                                }
                            }
                        } while (event.changes.any { it.pressed })

                        val wasTwoFingerTap = maxPointers == 2 &&
                            movement < viewConfiguration.touchSlop &&
                            endTime - startTime <= PlanGestureConfig.TWO_FINGER_TAP_MAX_MS
                        if (wasTwoFingerTap) {
                            currentFocus?.let { centerOn(it, PlanGestureConfig.FOCUS_SCALE) }
                        }
                    }
                }
                .pointerInput(planAsset) {
                    detectTapGestures(
                        onDoubleTap = { tap ->
                            if (scale > 1f) {
                                reset()
                            } else {
                                val anchor = tap - Offset(size.width / 2f, size.height / 2f)
                                zoomAround(anchor, PlanGestureConfig.DOUBLE_TAP_SCALE)
                            }
                        },
                    )
                },
            contentAlignment = Alignment.Center,
        ) {
            when (val state = image) {
                is AssetImageState.Ready -> {
                    val markerColor = MaterialTheme.colorScheme.primary
                    val targetColor = MaterialTheme.colorScheme.tertiary
                    Box(
                        modifier = Modifier
                            .fillMaxSize()
                            .graphicsLayer {
                                scaleX = scale
                                scaleY = scale
                                translationX = offset.x
                                translationY = offset.y
                            },
                    ) {
                        Image(
                            bitmap = state.bitmap,
                            // La descripción ya está en el contenedor (semantics).
                            contentDescription = null,
                            contentScale = ContentScale.FillBounds,
                            modifier = Modifier.fillMaxSize(),
                        )
                        Canvas(modifier = Modifier.fillMaxSize()) {
                            // Las marcas mantienen su tamaño en pantalla aunque se amplíe el plano.
                            val radius = 9.dp.toPx() / scale
                            val stroke = 3.dp.toPx() / scale
                            markers.forEach { marker ->
                                val center = Offset(marker.x * size.width, marker.y * size.height)
                                when (marker.style) {
                                    PlanMarkerStyle.CURRENT -> {
                                        drawCircle(Color.White, radius + stroke, center)
                                        drawCircle(markerColor, radius, center)
                                    }

                                    PlanMarkerStyle.TARGET -> {
                                        drawCircle(Color.White, radius * 1.6f + stroke, center)
                                        drawCircle(targetColor, radius * 1.6f, center, style = Stroke(stroke * 1.5f))
                                        drawCircle(targetColor, radius * 0.45f, center)
                                    }
                                }
                            }
                        }
                    }
                }

                AssetImageState.Missing -> Text(
                    text = stringResource(R.string.route_plan_missing),
                    style = MaterialTheme.typography.bodyMedium,
                )

                AssetImageState.Loading -> Text(
                    text = stringResource(R.string.loading),
                    style = MaterialTheme.typography.bodyMedium,
                )
            }
        }

        Text(
            text = stringResource(R.string.route_plan_legend),
            style = MaterialTheme.typography.bodySmall,
        )
        Text(
            text = stringResource(R.string.route_plan_gestures_hint),
            style = MaterialTheme.typography.bodySmall,
        )
        FlowRow(
            horizontalArrangement = Arrangement.spacedBy(8.dp),
            modifier = Modifier.fillMaxWidth(),
        ) {
            OutlinedButton(
                onClick = { zoomAround(Offset.Zero, PlanGestureConfig.STEP_SCALE) },
                modifier = Modifier.heightIn(min = 48.dp),
            ) { Text(stringResource(R.string.route_plan_zoom_in)) }
            OutlinedButton(
                onClick = { zoomAround(Offset.Zero, 1f / PlanGestureConfig.STEP_SCALE) },
                enabled = scale > 1f,
                modifier = Modifier.heightIn(min = 48.dp),
            ) { Text(stringResource(R.string.route_plan_zoom_out)) }
            if (focus != null) {
                OutlinedButton(
                    onClick = { centerOn(focus, PlanGestureConfig.FOCUS_SCALE) },
                    modifier = Modifier.heightIn(min = 48.dp),
                ) { Text(stringResource(R.string.route_plan_focus)) }
            }
            OutlinedButton(
                onClick = { reset() },
                enabled = scale > 1f,
                modifier = Modifier.heightIn(min = 48.dp),
            ) { Text(stringResource(R.string.route_plan_reset)) }
        }
    }
}
