package com.jesusmuve.etsiitsmartcampus.feature.outdoor

import android.Manifest
import androidx.activity.compose.rememberLauncherForActivityResult
import androidx.activity.result.contract.ActivityResultContracts
import androidx.compose.foundation.Image
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.aspectRatio
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.heightIn
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.verticalScroll
import androidx.compose.material3.Button
import androidx.compose.material3.Card
import androidx.compose.material3.CardDefaults
import androidx.compose.material3.CircularProgressIndicator
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.OutlinedButton
import androidx.compose.material3.Scaffold
import androidx.compose.material3.SnackbarHost
import androidx.compose.material3.SnackbarHostState
import androidx.compose.material3.Text
import androidx.compose.material3.TextButton
import androidx.compose.runtime.Composable
import androidx.compose.runtime.DisposableEffect
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.remember
import androidx.compose.runtime.rememberCoroutineScope
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.layout.ContentScale
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.semantics.LiveRegionMode
import androidx.compose.ui.semantics.heading
import androidx.compose.ui.semantics.liveRegion
import androidx.compose.ui.semantics.semantics
import androidx.compose.ui.unit.dp
import androidx.lifecycle.Lifecycle
import androidx.lifecycle.LifecycleEventObserver
import androidx.lifecycle.compose.LocalLifecycleOwner
import androidx.lifecycle.compose.collectAsStateWithLifecycle
import com.jesusmuve.etsiitsmartcampus.R
import com.jesusmuve.etsiitsmartcampus.ui.components.CampusHeader
import com.jesusmuve.etsiitsmartcampus.core.model.Building
import com.jesusmuve.etsiitsmartcampus.ui.components.AssetImageState
import com.jesusmuve.etsiitsmartcampus.ui.components.localized
import com.jesusmuve.etsiitsmartcampus.ui.components.rememberAssetImage
import kotlinx.coroutines.launch
import kotlin.math.roundToInt

/**
 * Pantalla de **navegación exterior**: cómo llegar a la ETSIIT.
 *
 * - **Google Maps:** abre la ruta a pie hasta la entrada (la app de Maps hace la navegación).
 * - **GPS de la app** (sensor de posición): mientras esta pantalla está visible, comprueba si
 *   el usuario ha llegado a la entrada y, en ese caso, le ofrece empezar la ruta interior.
 * - Alternativa sin sensores: «Ya estoy en la ETSIIT» empieza la ruta a mano.
 * - **Confirmar que ya está dentro:** el GPS no distingue con fiabilidad dentro de fuera; si la
 *   señal empeora tras llegar, solo se *sugiere* ([ArrivalStatus.ProbablyInside]). La
 *   confirmación es escanear el punto de control de la entrada ([onScan]); el punto leído llega
 *   a [OutdoorViewModel.onPointIdentified] y la ruta interior empieza en él.
 *
 * El permiso de ubicación se pide solo cuando el usuario pulsa el botón, tras explicar para qué
 * sirve; si lo deniega, todo lo demás sigue funcionando.
 *
 * @param onStartIndoorRoute Abre la ruta interior empezando en el nodo indicado (la entrada).
 * @param onBack Volver a la pantalla anterior.
 * @param onScan Abre el lector QR/NFC/manual para confirmar el punto de la entrada.
 */
@Composable
fun OutdoorScreen(
    viewModel: OutdoorViewModel,
    onStartIndoorRoute: (originNodeId: String) -> Unit,
    onBack: () -> Unit,
    modifier: Modifier = Modifier,
    onScan: () -> Unit = {},
) {
    val state by viewModel.uiState.collectAsStateWithLifecycle()
    val context = LocalContext.current
    val snackbarHostState = remember { SnackbarHostState() }
    val scope = rememberCoroutineScope()
    val noMapsAppText = stringResource(R.string.outdoor_no_maps_app)
    val noSettingsText = stringResource(R.string.outdoor_no_settings)

    // GPS atado al ciclo de vida: se enciende en ON_START y se apaga en ON_STOP o al
    // salir de la pantalla. Así nunca se usa la ubicación en segundo plano.
    val lifecycleOwner = LocalLifecycleOwner.current
    DisposableEffect(lifecycleOwner, viewModel) {
        val observer = LifecycleEventObserver { _, event ->
            when (event) {
                Lifecycle.Event.ON_START -> viewModel.onScreenVisible()
                Lifecycle.Event.ON_STOP -> viewModel.onScreenHidden()
                else -> Unit
            }
        }
        lifecycleOwner.lifecycle.addObserver(observer)
        onDispose {
            lifecycleOwner.lifecycle.removeObserver(observer)
            viewModel.onScreenHidden()
        }
    }

    val permissionLauncher = rememberLauncherForActivityResult(
        ActivityResultContracts.RequestMultiplePermissions(),
    ) { results ->
        // Basta con cualquiera de los dos: con la aproximada se avisa de que falta precisión.
        viewModel.onPermissionResult(results.values.any { it })
    }

    fun showMessage(text: String) {
        scope.launch { snackbarHostState.showSnackbar(text) }
    }

    // Punto de control escaneado: si pertenece a la ruta interior, la ruta empieza en él.
    LaunchedEffect(state.confirmedOriginNodeId) {
        state.confirmedOriginNodeId?.let { nodeId ->
            viewModel.onConfirmedOriginHandled()
            onStartIndoorRoute(nodeId)
        }
    }
    val notInRouteText = stringResource(R.string.outdoor_scan_not_in_route)
    LaunchedEffect(state.scanNotInRoute) {
        if (state.scanNotInRoute) {
            viewModel.onScanMessageShown()
            snackbarHostState.showSnackbar(notInRouteText)
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
            CampusHeader(title = stringResource(R.string.outdoor_title), onBack = onBack)

            BuildingImage()

            val building = state.building
            when {
                state.loadFailed -> Text(stringResource(R.string.route_load_error))
                building == null -> Row(
                    horizontalArrangement = Arrangement.spacedBy(12.dp),
                    verticalAlignment = Alignment.CenterVertically,
                ) {
                    CircularProgressIndicator()
                    Text(stringResource(R.string.loading))
                }

                else -> {
                    Text(localized(building.nameEs, building.nameEn), style = MaterialTheme.typography.titleMedium)
                    Text(localized(building.addressEs, building.addressEn), style = MaterialTheme.typography.bodyMedium)

                    Button(
                        onClick = {
                            val opened = MapsLauncher.openWalkingDirections(
                                context,
                                building.entranceLatitude,
                                building.entranceLongitude,
                                building.mapsQuery,
                                building.mapsPlaceId,
                            )
                            if (!opened) showMessage(noMapsAppText)
                        },
                        modifier = Modifier
                            .fillMaxWidth()
                            .heightIn(min = 48.dp),
                    ) { Text(stringResource(R.string.outdoor_open_maps)) }
                    Text(stringResource(R.string.outdoor_maps_needs_internet), style = MaterialTheme.typography.bodySmall)

                    ArrivalCard(
                        state = state,
                        building = building,
                        onRequestPermission = {
                            permissionLauncher.launch(
                                arrayOf(
                                    Manifest.permission.ACCESS_FINE_LOCATION,
                                    Manifest.permission.ACCESS_COARSE_LOCATION,
                                ),
                            )
                        },
                        onOpenAppSettings = {
                            if (!MapsLauncher.openAppSettings(context)) showMessage(noSettingsText)
                        },
                        onOpenLocationSettings = {
                            if (!MapsLauncher.openLocationSettings(context)) showMessage(noSettingsText)
                        },
                        onStartIndoorRoute = { onStartIndoorRoute(building.entranceNodeId) },
                        onScan = onScan,
                    )

                    // Confirmación fiable de que ya está dentro, con o sin GPS.
                    Text(stringResource(R.string.outdoor_scan_entrance_hint), style = MaterialTheme.typography.bodySmall)
                    OutlinedButton(
                        onClick = onScan,
                        modifier = Modifier
                            .fillMaxWidth()
                            .heightIn(min = 48.dp),
                    ) { Text(stringResource(R.string.outdoor_scan_entrance)) }

                    // Alternativa manual: nunca hace falta el GPS para empezar la ruta interior.
                    OutlinedButton(
                        onClick = { onStartIndoorRoute(building.entranceNodeId) },
                        modifier = Modifier
                            .fillMaxWidth()
                            .heightIn(min = 48.dp),
                    ) { Text(stringResource(R.string.outdoor_already_here)) }
                }
            }
        }
    }
}

@Composable
private fun BuildingImage() {
    val image by rememberAssetImage(BUILDING_IMAGE_ASSET, maxDimension = BUILDING_IMAGE_MAX_DIMENSION)
    val ready = image as? AssetImageState.Ready ?: return
    Image(
        bitmap = ready.bitmap,
        contentDescription = stringResource(R.string.outdoor_image_description),
        contentScale = ContentScale.Crop,
        modifier = Modifier
            .fillMaxWidth()
            .aspectRatio(BUILDING_IMAGE_ASPECT_RATIO),
    )
}

/** Tarjeta de llegada por GPS: cambia según permiso, disponibilidad y distancia. */
@Composable
private fun ArrivalCard(
    state: OutdoorUiState,
    building: Building,
    onRequestPermission: () -> Unit,
    onOpenAppSettings: () -> Unit,
    onOpenLocationSettings: () -> Unit,
    onStartIndoorRoute: () -> Unit,
    onScan: () -> Unit,
) {
    val arrived = state.permission == LocationPermissionState.GRANTED &&
        state.locationAvailable &&
        state.arrival.hasArrived
    // El color acompaña al texto, nunca lo sustituye.
    val colors = if (arrived) {
        CardDefaults.cardColors(containerColor = MaterialTheme.colorScheme.primaryContainer)
    } else {
        CardDefaults.cardColors()
    }

    Card(colors = colors, modifier = Modifier.fillMaxWidth()) {
        Column(
            modifier = Modifier.padding(16.dp),
            verticalArrangement = Arrangement.spacedBy(8.dp),
        ) {
            Text(
                text = stringResource(R.string.outdoor_gps_title),
                style = MaterialTheme.typography.titleMedium,
                modifier = Modifier.semantics { heading() },
            )
            when {
                state.permission == LocationPermissionState.NOT_REQUESTED -> {
                    Text(stringResource(R.string.outdoor_gps_explain))
                    Button(onClick = onRequestPermission, modifier = Modifier.heightIn(min = 48.dp)) {
                        Text(stringResource(R.string.outdoor_gps_allow))
                    }
                }

                state.permission == LocationPermissionState.DENIED -> {
                    Text(stringResource(R.string.outdoor_gps_denied))
                    Row(horizontalArrangement = Arrangement.spacedBy(8.dp)) {
                        OutlinedButton(onClick = onRequestPermission, modifier = Modifier.heightIn(min = 48.dp)) {
                            Text(stringResource(R.string.outdoor_gps_ask_again))
                        }
                        OutlinedButton(onClick = onOpenAppSettings, modifier = Modifier.heightIn(min = 48.dp)) {
                            Text(stringResource(R.string.outdoor_open_app_settings))
                        }
                    }
                }

                !state.locationAvailable -> {
                    Text(stringResource(R.string.outdoor_gps_disabled))
                    Button(onClick = onOpenLocationSettings, modifier = Modifier.heightIn(min = 48.dp)) {
                        Text(stringResource(R.string.outdoor_enable_location))
                    }
                }

                else -> ArrivalStatusContent(
                    status = state.arrival,
                    radiusMeters = building.arrivalRadiusMeters,
                    onStartIndoorRoute = onStartIndoorRoute,
                    onScan = onScan,
                )
            }
        }
    }
}

@Composable
private fun ArrivalStatusContent(
    status: ArrivalStatus,
    radiusMeters: Double,
    onStartIndoorRoute: () -> Unit,
    onScan: () -> Unit,
) {
    // Región en vivo: TalkBack anuncia la distancia y, sobre todo, la llegada.
    val liveModifier = Modifier.semantics { liveRegion = LiveRegionMode.Polite }
    when (status) {
        ArrivalStatus.NoFix -> Text(stringResource(R.string.outdoor_gps_searching), modifier = liveModifier)

        is ArrivalStatus.LowAccuracy -> Text(
            text = stringResource(R.string.outdoor_gps_low_accuracy),
            modifier = liveModifier,
        )

        is ArrivalStatus.Away -> {
            Text(
                text = stringResource(
                    R.string.outdoor_gps_away,
                    formatDistance(status.distanceMeters),
                ),
                modifier = liveModifier,
            )
            Text(
                text = stringResource(R.string.outdoor_gps_away_hint),
                style = MaterialTheme.typography.bodySmall,
            )
        }

        is ArrivalStatus.AtEntrance -> {
            Text(
                text = stringResource(R.string.outdoor_gps_arrived),
                style = MaterialTheme.typography.titleMedium,
                modifier = liveModifier,
            )
            Button(
                onClick = onStartIndoorRoute,
                modifier = Modifier
                    .fillMaxWidth()
                    .heightIn(min = 48.dp),
            ) { Text(stringResource(R.string.outdoor_start_indoor)) }
        }

        // Solo una suposición: se dice así y se invita a confirmarla con la etiqueta.
        is ArrivalStatus.ProbablyInside -> {
            Text(
                text = stringResource(R.string.outdoor_gps_probably_inside),
                style = MaterialTheme.typography.titleMedium,
                modifier = liveModifier,
            )
            Button(
                onClick = onScan,
                modifier = Modifier
                    .fillMaxWidth()
                    .heightIn(min = 48.dp),
            ) { Text(stringResource(R.string.outdoor_scan_entrance)) }
            OutlinedButton(
                onClick = onStartIndoorRoute,
                modifier = Modifier
                    .fillMaxWidth()
                    .heightIn(min = 48.dp),
            ) { Text(stringResource(R.string.outdoor_start_indoor)) }
        }
    }
}

/** Distancia legible: metros por debajo de 1 km y kilómetros con un decimal por encima. */
@Composable
private fun formatDistance(meters: Double): String =
    if (meters < METERS_PER_KM) {
        stringResource(R.string.distance_meters, meters.roundToInt())
    } else {
        stringResource(R.string.distance_kilometers, meters / METERS_PER_KM)
    }

private const val METERS_PER_KM = 1000.0
private const val BUILDING_IMAGE_ASSET = "outdoor/etsiit.jpg"
private const val BUILDING_IMAGE_MAX_DIMENSION = 1280
private const val BUILDING_IMAGE_ASPECT_RATIO = 4f / 3f
