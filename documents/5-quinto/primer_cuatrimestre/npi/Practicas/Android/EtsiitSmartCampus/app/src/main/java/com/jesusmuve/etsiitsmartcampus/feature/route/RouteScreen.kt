package com.jesusmuve.etsiitsmartcampus.feature.route

import android.util.Log
import androidx.compose.foundation.Image
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.heightIn
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.aspectRatio
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.selection.selectable
import androidx.compose.foundation.selection.toggleable
import androidx.compose.foundation.verticalScroll
import androidx.compose.material3.Button
import androidx.compose.material3.AlertDialog
import androidx.compose.material3.Card
import androidx.compose.material3.CardDefaults
import androidx.compose.material3.CircularProgressIndicator
import androidx.compose.material3.HorizontalDivider
import androidx.compose.material3.LinearProgressIndicator
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.OutlinedButton
import androidx.compose.material3.RadioButton
import androidx.compose.material3.Scaffold
import androidx.compose.material3.SnackbarHost
import androidx.compose.material3.SnackbarHostState
import androidx.compose.material3.Switch
import androidx.compose.material3.Text
import androidx.compose.material3.TextButton
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.saveable.rememberSaveable
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.ui.draw.clip
import androidx.compose.ui.geometry.Offset
import androidx.compose.ui.layout.ContentScale
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.semantics.LiveRegionMode
import androidx.compose.ui.semantics.Role
import androidx.compose.ui.semantics.heading
import androidx.compose.ui.semantics.liveRegion
import androidx.compose.ui.semantics.semantics
import androidx.compose.ui.unit.dp
import androidx.lifecycle.compose.collectAsStateWithLifecycle
import com.jesusmuve.etsiitsmartcampus.R
import com.jesusmuve.etsiitsmartcampus.ui.components.CampusHeader
import com.jesusmuve.etsiitsmartcampus.core.model.EdgeKind
import com.jesusmuve.etsiitsmartcampus.core.model.IndoorMap
import com.jesusmuve.etsiitsmartcampus.core.model.IndoorNode
import com.jesusmuve.etsiitsmartcampus.core.model.ScanSource
import com.jesusmuve.etsiitsmartcampus.feature.audio.ListenPanel
import com.jesusmuve.etsiitsmartcampus.feature.gestures.ScanGesturePanel
import com.jesusmuve.etsiitsmartcampus.ui.components.AssetImageState
import com.jesusmuve.etsiitsmartcampus.ui.components.localized
import com.jesusmuve.etsiitsmartcampus.ui.components.rememberAssetImage

/**
 * Pantalla de **navegación interior por nodos**.
 *
 * Fases ([RoutePhase]): elegir punto de partida y destino → guiado paso a paso con la foto del
 * punto donde se está y el plano multitáctil → llegada. La pantalla no contiene lógica de ruta: muestra el estado del
 * [IndoorRouteViewModel] y le pasa las acciones del usuario.
 *
 * @param viewModel Lógica de la ruta (creado con la fábrica del `AppContainer`).
 * @param onBack Volver a la pantalla anterior.
 * @param onGoHome Volver al inicio tras llegar.
 */
@Composable
fun RouteScreen(
    viewModel: IndoorRouteViewModel,
    onBack: () -> Unit,
    onGoHome: () -> Unit,
    modifier: Modifier = Modifier,
    onScan: () -> Unit = {},
) {
    val state by viewModel.uiState.collectAsStateWithLifecycle()
    val snackbarHostState = remember { SnackbarHostState() }

    // Avisos puntuales (punto confirmado, punto equivocado…) como snackbar: TalkBack los anuncia.
    val messageText = state.message?.let { routeMessageText(it, state.map) }
    LaunchedEffect(state.message) {
        if (messageText != null) {
            snackbarHostState.showSnackbar(messageText)
            viewModel.messageShown()
        }
    }

    Scaffold(
        modifier = modifier,
        snackbarHost = { SnackbarHost(snackbarHostState) },
    ) { innerPadding ->
        Column(
            modifier = Modifier
                .padding(innerPadding)
                .fillMaxSize()
                .verticalScroll(rememberScrollState())
                .padding(16.dp),
            verticalArrangement = Arrangement.spacedBy(12.dp),
        ) {
            ScreenHeader(onBack = onBack)

            val map = state.map
            when (state.phase) {
                RoutePhase.LOADING -> LoadingContent()
                RoutePhase.LOAD_ERROR -> LoadErrorContent(state.loadProblems, onRetry = viewModel::loadMap)
                RoutePhase.CHOOSING -> if (map != null) {
                    ChoosingContent(state = state, map = map, viewModel = viewModel)
                }

                RoutePhase.GUIDING -> {
                    val leg = state.currentLeg
                    if (map != null && leg != null) {
                        GuidingContent(state = state, map = map, leg = leg, viewModel = viewModel, onScan = onScan)
                    }
                }

                RoutePhase.ARRIVED -> {
                    val destination = state.legs.lastOrNull()?.to
                    if (destination != null) {
                        ArrivedContent(
                            destination = destination,
                            photoAsset = state.arrivalPhotoAsset,
                            onNewRoute = viewModel::planNewRouteFromHere,
                            onGoHome = onGoHome,
                        )
                    }
                }
            }
        }
    }
}

@Composable
private fun ScreenHeader(onBack: () -> Unit) {
    CampusHeader(title = stringResource(R.string.route_title), onBack = onBack)
}

@Composable
private fun LoadingContent() {
    Row(horizontalArrangement = Arrangement.spacedBy(12.dp), verticalAlignment = Alignment.CenterVertically) {
        CircularProgressIndicator()
        Text(stringResource(R.string.loading))
    }
}

@Composable
private fun LoadErrorContent(problems: List<String>, onRetry: () -> Unit) {
    Text(stringResource(R.string.route_load_error), style = MaterialTheme.typography.bodyLarge)
    // El detalle técnico de los problemas no se enseña al usuario: va al registro para depurar.
    LaunchedEffect(problems) { problems.forEach { Log.w(LOG_TAG, it) } }
    Button(onClick = onRetry, modifier = Modifier.heightIn(min = 48.dp)) {
        Text(stringResource(R.string.retry))
    }
}

/** Fase de elección: punto de partida, destino, variante por ascensor e inicio. */
@Composable
private fun ChoosingContent(state: IndoorRouteUiState, map: IndoorMap, viewModel: IndoorRouteViewModel) {
    var picking by rememberSaveable { mutableStateOf<PickerTarget?>(null) }

    if (map.building.provisional) ProvisionalDataNotice()

    Text(stringResource(R.string.route_choose_intro), style = MaterialTheme.typography.bodyLarge)

    PlaceCard(
        label = stringResource(R.string.route_origin),
        placeId = state.originId,
        map = map,
        onChange = { picking = PickerTarget.ORIGIN },
    )
    OutlinedButton(
        onClick = viewModel::swapOriginAndDestination,
        enabled = state.destinationId?.startsWith(ROOM_PREFIX) != true,
        modifier = Modifier.heightIn(min = 48.dp),
    ) { Text(stringResource(R.string.route_swap)) }
    PlaceCard(
        label = stringResource(R.string.route_destination),
        placeId = state.destinationId,
        map = map,
        onChange = { picking = PickerTarget.DESTINATION },
    )

    // Fila completa conmutable: TalkBack la anuncia como un interruptor con su texto.
    Row(
        verticalAlignment = Alignment.CenterVertically,
        horizontalArrangement = Arrangement.spacedBy(12.dp),
        modifier = Modifier
            .fillMaxWidth()
            .heightIn(min = 48.dp)
            .toggleable(
                value = state.avoidStairs,
                role = Role.Switch,
                onValueChange = viewModel::setAvoidStairs,
            ),
    ) {
        Column(modifier = Modifier.weight(1f)) {
            Text(stringResource(R.string.route_avoid_stairs), style = MaterialTheme.typography.bodyLarge)
            Text(stringResource(R.string.route_avoid_stairs_hint), style = MaterialTheme.typography.bodySmall)
        }
        Switch(checked = state.avoidStairs, onCheckedChange = null)
    }

    Button(
        onClick = viewModel::startRoute,
        modifier = Modifier
            .fillMaxWidth()
            .heightIn(min = 48.dp),
    ) { Text(stringResource(R.string.route_start)) }

    when (picking) {
        PickerTarget.ORIGIN -> PlacePickerDialog(
            title = stringResource(R.string.route_pick_origin),
            map = map,
            extraRoom = null,
            selectedId = state.originId,
            onSelect = { viewModel.selectOrigin(it); picking = null },
            onDismiss = { picking = null },
        )

        PickerTarget.DESTINATION -> PlacePickerDialog(
            title = stringResource(R.string.route_pick_destination),
            map = map,
            extraRoom = state.extraRoom,
            selectedId = state.destinationId,
            onSelect = { viewModel.selectDestination(it); picking = null },
            onDismiss = { picking = null },
        )

        null -> Unit
    }
}

private enum class PickerTarget { ORIGIN, DESTINATION }

@Composable
private fun ProvisionalDataNotice() {
    Card(colors = CardDefaults.cardColors(containerColor = MaterialTheme.colorScheme.secondaryContainer)) {
        Text(
            text = stringResource(R.string.route_provisional_notice),
            style = MaterialTheme.typography.bodyMedium,
            modifier = Modifier.padding(12.dp),
        )
    }
}

/** Nombre visible de un lugar: un nodo del grafo o un aula del horario («Aula 0.7»). */
@Composable
private fun placeName(placeId: String, map: IndoorMap): String {
    map.node(placeId)?.let { return localized(it.nameEs, it.nameEn) }
    return stringResource(R.string.route_room_name, placeId.removePrefix(ROOM_PREFIX))
}

@Composable
private fun PlaceCard(label: String, placeId: String?, map: IndoorMap, onChange: () -> Unit) {
    Card(modifier = Modifier.fillMaxWidth()) {
        Row(
            verticalAlignment = Alignment.CenterVertically,
            modifier = Modifier.padding(12.dp),
        ) {
            Column(modifier = Modifier.weight(1f)) {
                Text(label, style = MaterialTheme.typography.labelLarge)
                if (placeId != null) {
                    Text(placeName(placeId, map), style = MaterialTheme.typography.titleMedium)
                    map.node(placeId)?.let { node -> map.floor(node.floorId) }?.let { floor ->
                        Text(localized(floor.nameEs, floor.nameEn), style = MaterialTheme.typography.bodySmall)
                    }
                }
            }
            TextButton(onClick = onChange, modifier = Modifier.heightIn(min = 48.dp)) {
                Text(stringResource(R.string.route_change))
            }
        }
    }
}

/**
 * Lista de lugares agrupados por planta, con botones de opción accesibles. Si la pantalla se abrió
 * desde el horario con un aula que no está en el grafo, aparece primero.
 */
@Composable
private fun PlacePickerDialog(
    title: String,
    map: IndoorMap,
    extraRoom: String?,
    selectedId: String?,
    onSelect: (String) -> Unit,
    onDismiss: () -> Unit,
) {
    AlertDialog(
        onDismissRequest = onDismiss,
        title = { Text(title) },
        text = {
            Column(modifier = Modifier.verticalScroll(rememberScrollState())) {
                if (extraRoom != null) {
                    PlaceOption(
                        name = stringResource(R.string.route_room_name, extraRoom),
                        selected = selectedId == ROOM_PREFIX + extraRoom,
                        onClick = { onSelect(ROOM_PREFIX + extraRoom) },
                    )
                }
                map.floors.forEach { floor ->
                    Text(
                        text = localized(floor.nameEs, floor.nameEn),
                        style = MaterialTheme.typography.titleSmall,
                        modifier = Modifier
                            .padding(top = 8.dp)
                            .semantics { heading() },
                    )
                    map.nodes.filter { it.floorId == floor.id }.forEach { node ->
                        PlaceOption(
                            name = localized(node.nameEs, node.nameEn),
                            selected = node.id == selectedId,
                            onClick = { onSelect(node.id) },
                        )
                    }
                }
            }
        },
        confirmButton = {
            TextButton(onClick = onDismiss) { Text(stringResource(R.string.close)) }
        },
    )
}

@Composable
private fun PlaceOption(name: String, selected: Boolean, onClick: () -> Unit) {
    Row(
        verticalAlignment = Alignment.CenterVertically,
        modifier = Modifier
            .fillMaxWidth()
            .heightIn(min = 48.dp)
            .selectable(selected = selected, role = Role.RadioButton, onClick = onClick),
    ) {
        RadioButton(selected = selected, onClick = null)
        Text(text = name, modifier = Modifier.padding(start = 8.dp))
    }
}

/** Fase de guiado: un tramo cada vez. */
@Composable
private fun GuidingContent(
    state: IndoorRouteUiState,
    map: IndoorMap,
    leg: RouteLeg,
    viewModel: IndoorRouteViewModel,
    onScan: () -> Unit,
) {
    val stepNumber = state.currentLegIndex + 1
    val totalSteps = state.legs.size
    val target = leg.to
    val targetName = localized(target.nameEs, target.nameEn)
    val fromFloor = map.floor(leg.from.floorId)
    val toFloor = map.floor(target.floorId)

    // «Paso X de N» se anuncia al cambiar de paso (región en vivo).
    Text(
        text = stringResource(R.string.route_step_of, stepNumber, totalSteps),
        style = MaterialTheme.typography.titleMedium,
        modifier = Modifier.semantics { liveRegion = LiveRegionMode.Polite },
    )
    LinearProgressIndicator(
        progress = { state.currentLegIndex.toFloat() / totalSteps },
        modifier = Modifier.fillMaxWidth(),
    )

    if (fromFloor != null && toFloor != null) {
        val floorText = if (fromFloor.id != toFloor.id) {
            stringResource(
                R.string.route_floor_change,
                localized(fromFloor.nameEs, fromFloor.nameEn),
                localized(toFloor.nameEs, toFloor.nameEn),
            )
        } else {
            stringResource(R.string.route_floor_current, localized(toFloor.nameEs, toFloor.nameEn))
        }
        Text(floorText, style = MaterialTheme.typography.labelLarge)
    }

    when (leg.kind) {
        EdgeKind.STAIRS -> Text(stringResource(R.string.route_kind_stairs), style = MaterialTheme.typography.labelLarge)
        EdgeKind.ELEVATOR -> Text(stringResource(R.string.route_kind_elevator), style = MaterialTheme.typography.labelLarge)
        EdgeKind.WALK -> Unit
    }

    val instruction = localized(leg.instructionEs, leg.instructionEn)
    val lookFor = stringResource(R.string.route_look_for, targetName)
    val targetDescription = localized(target.descriptionEs, target.descriptionEn)
    Text(text = instruction, style = MaterialTheme.typography.headlineSmall)
    Text(text = lookFor, style = MaterialTheme.typography.titleMedium)
    Text(targetDescription, style = MaterialTheme.typography.bodyMedium)

    // «Escuchar»: botón o acercar el móvil a la oreja. Solo el paso y la instrucción, para que sea
    // corto; el resto sigue en pantalla.
    val stepTitle = stringResource(R.string.route_step_of, stepNumber, totalSteps)
    ListenPanel(text = "$stepTitle. $instruction")

    // Foto del punto donde está el usuario (el rojo del plano), vista en el sentido de la marcha.
    NodePhoto(node = leg.from, asset = leg.photoAsset)

    if (toFloor != null) {
        val markers = buildList {
            // El punto de partida solo se marca si está en la misma planta que el plano.
            if (leg.from.floorId == target.floorId) {
                add(PlanMarker(leg.from.planX, leg.from.planY, PlanMarkerStyle.CURRENT))
            }
            add(PlanMarker(target.planX, target.planY, PlanMarkerStyle.TARGET))
        }
        ZoomablePlan(
            planAsset = toFloor.planAsset,
            markers = markers,
            focus = Offset(target.planX, target.planY),
            description = stringResource(
                R.string.route_plan_description,
                localized(toFloor.nameEs, toFloor.nameEn),
                targetName,
            ),
        )
    }

    if (leg.levelChange != 0 && toFloor != null) {
        // Escalera o ascensor: el barómetro avisa al llegar a la planta (no avanza solo).
        FloorChangePanel(
            legKey = state.currentLegIndex,
            levelChange = leg.levelChange,
            floorHeightMeters = map.building.floorHeightMeters,
            // Va dentro de una frase («Sube hasta la segunda planta»): primera letra en minúscula.
            targetFloorName = localized(toFloor.nameEs, toFloor.nameEn).replaceFirstChar { it.lowercase() },
        )
    } else {
        // «¿Voy bien?»: comparar hacia dónde apunta el móvil con el rumbo real del tramo.
        OrientationPanel(
            legKey = state.currentLegIndex,
            targetBearingDegrees = leg.bearingDegrees,
            latitude = map.building.entranceLatitude,
            longitude = map.building.entranceLongitude,
        )
    }

    HorizontalDivider()
    Text(stringResource(R.string.route_advance_hint), style = MaterialTheme.typography.bodySmall)
    Button(
        onClick = viewModel::confirmManually,
        modifier = Modifier
            .fillMaxWidth()
            .heightIn(min = 48.dp),
    ) { Text(stringResource(R.string.route_im_here)) }
    // Gesto G1 y botón equivalente: los dos abren el lector QR/NFC/manual, que devuelve el
    // punto leído; el ViewModel decide si es el esperado. El sensor solo escucha durante el guiado.
    ScanGesturePanel(onScanRequested = { onScan() })
    Row(horizontalArrangement = Arrangement.spacedBy(8.dp), modifier = Modifier.fillMaxWidth()) {
        OutlinedButton(
            onClick = viewModel::previousLeg,
            enabled = state.currentLegIndex > 0,
            modifier = Modifier
                .weight(1f)
                .heightIn(min = 48.dp),
        ) { Text(stringResource(R.string.route_previous_step)) }
        OutlinedButton(
            onClick = viewModel::cancelRoute,
            modifier = Modifier
                .weight(1f)
                .heightIn(min = 48.dp),
        ) { Text(stringResource(R.string.route_cancel)) }
    }
}

/**
 * Foto de un punto de la ruta, o un aviso si aún no se ha añadido.
 *
 * @param asset Foto de la parada en la ruta de muestra: depende del sentido de la marcha (las
 * fotos de «ida» y «vuelta» del mismo punto se ven distintas).
 */
@Composable
private fun NodePhoto(node: IndoorNode, asset: String?) {
    val photo by rememberAssetImage(asset, maxDimension = PHOTO_MAX_DIMENSION)
    when (val image = photo) {
        is AssetImageState.Ready -> Image(
            bitmap = image.bitmap,
            contentDescription = stringResource(R.string.route_photo_description, localized(node.nameEs, node.nameEn)),
            contentScale = ContentScale.Crop,
            modifier = Modifier
                .fillMaxWidth()
                // Las fotos están hechas en vertical: se respeta su forma, sin pasar de 4:5 de alto.
                .aspectRatio((image.bitmap.width.toFloat() / image.bitmap.height).coerceAtLeast(MIN_PHOTO_ASPECT_RATIO))
                .clip(RoundedCornerShape(12.dp)),
        )

        AssetImageState.Missing -> Card(modifier = Modifier.fillMaxWidth()) {
            Box(contentAlignment = Alignment.Center, modifier = Modifier.padding(16.dp)) {
                Text(stringResource(R.string.route_photo_missing), style = MaterialTheme.typography.bodyMedium)
            }
        }

        AssetImageState.Loading -> Unit
    }
}

/** Fase de llegada. */
@Composable
private fun ArrivedContent(
    destination: IndoorNode,
    photoAsset: String?,
    onNewRoute: () -> Unit,
    onGoHome: () -> Unit,
) {
    Text(
        text = stringResource(R.string.route_arrived_title),
        style = MaterialTheme.typography.headlineMedium,
        modifier = Modifier.semantics {
            heading()
            liveRegion = LiveRegionMode.Polite
        },
    )
    Text(localized(destination.nameEs, destination.nameEn), style = MaterialTheme.typography.titleLarge)
    Text(localized(destination.descriptionEs, destination.descriptionEn), style = MaterialTheme.typography.bodyLarge)
    NodePhoto(node = destination, asset = photoAsset)

    Button(
        onClick = onNewRoute,
        modifier = Modifier
            .fillMaxWidth()
            .heightIn(min = 48.dp),
    ) { Text(stringResource(R.string.route_new_from_here)) }
    OutlinedButton(
        onClick = onGoHome,
        modifier = Modifier
            .fillMaxWidth()
            .heightIn(min = 48.dp),
    ) { Text(stringResource(R.string.route_go_home)) }
}

/** Texto de cada aviso en el idioma activo. */
@Composable
private fun routeMessageText(message: RouteMessage, map: IndoorMap?): String = when (message) {
    RouteMessage.SameOriginAndDestination -> stringResource(R.string.route_msg_same)
    RouteMessage.NotInDemo -> stringResource(R.string.route_msg_not_in_demo)
    is RouteMessage.UnexpectedPoint -> {
        val node = map?.node(message.readId)
        if (node != null) {
            stringResource(R.string.route_msg_unexpected_known, localized(node.nameEs, node.nameEn))
        } else {
            stringResource(R.string.route_msg_unexpected_unknown)
        }
    }

    is RouteMessage.Confirmed -> {
        val node = map?.node(message.nodeId)
        val name = node?.let { localized(it.nameEs, it.nameEn) } ?: message.nodeId
        when (message.source) {
            ScanSource.MANUAL -> stringResource(R.string.route_msg_confirmed_manual, name)
            ScanSource.QR -> stringResource(R.string.route_msg_confirmed_qr, name)
            ScanSource.NFC -> stringResource(R.string.route_msg_confirmed_nfc, name)
        }
    }
}

private const val LOG_TAG = "RouteScreen"
private const val PHOTO_MAX_DIMENSION = 1600
private const val MIN_PHOTO_ASPECT_RATIO = 4f / 5f
