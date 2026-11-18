package com.jesusmuve.etsiitsmartcampus.feature.scanner

import android.Manifest
import android.app.Activity
import android.content.Context
import android.content.ContextWrapper
import android.content.Intent
import android.content.pm.PackageManager
import android.net.Uri
import android.provider.Settings
import androidx.activity.compose.rememberLauncherForActivityResult
import androidx.activity.result.contract.ActivityResultContracts
import androidx.compose.foundation.background
import androidx.compose.foundation.border
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.aspectRatio
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.heightIn
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.layout.width
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.shape.CircleShape
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.foundation.text.KeyboardActions
import androidx.compose.foundation.text.KeyboardOptions
import androidx.compose.foundation.verticalScroll
import androidx.compose.material3.Button
import androidx.compose.material3.Card
import androidx.compose.material3.CardDefaults
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.OutlinedButton
import androidx.compose.material3.OutlinedTextField
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.material3.TextButton
import androidx.compose.runtime.Composable
import androidx.compose.runtime.DisposableEffect
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.rememberUpdatedState
import androidx.compose.runtime.saveable.rememberSaveable
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.clip
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.semantics.LiveRegionMode
import androidx.compose.ui.semantics.liveRegion
import androidx.compose.ui.semantics.semantics
import androidx.compose.ui.text.input.ImeAction
import androidx.compose.ui.text.input.KeyboardCapitalization
import androidx.compose.ui.unit.dp
import androidx.core.content.ContextCompat
import androidx.lifecycle.Lifecycle
import androidx.lifecycle.LifecycleEventObserver
import androidx.lifecycle.compose.LocalLifecycleOwner
import androidx.lifecycle.viewmodel.compose.viewModel
import com.jesusmuve.etsiitsmartcampus.CampusApplication
import com.jesusmuve.etsiitsmartcampus.R
import com.jesusmuve.etsiitsmartcampus.core.model.IdentifiedPoint
import com.jesusmuve.etsiitsmartcampus.core.model.ScanSource

/** Estado del permiso de cámara tal y como lo necesita la UI. */
private enum class CameraPermission { REQUESTING, GRANTED, DENIED, PERMANENTLY_DENIED }

/**
 * Pantalla del lector de puntos de control: cámara (QR), NFC y código manual.
 *
 * Es la **única** interfaz pública del lector para el resto de la app: quien la abre solo
 * recibe un [IdentifiedPoint] por [onIdentified] o una cancelación por [onCancel]. Ninguna otra
 * pantalla depende de CameraX ni de NFC.
 *
 * - El permiso de cámara se pide al abrir esta pantalla, no al arrancar la app. Si se deniega,
 *   se explica y quedan NFC y el código manual: la ruta no se bloquea.
 * - Solo se entrega la primera lectura válida; a partir de ahí la cámara y NFC se liberan.
 * - No decide si el punto es el esperado: eso lo hace la ruta con el punto recibido.
 *
 * @param onIdentified Punto identificado (ID existente en el catálogo).
 * @param onCancel El usuario cierra el lector sin identificar nada.
 */
@Composable
fun ScannerScreen(
    onIdentified: (IdentifiedPoint) -> Unit,
    onCancel: () -> Unit,
    modifier: Modifier = Modifier,
) {
    val context = LocalContext.current
    val factory = remember {
        val container = (context.applicationContext as CampusApplication).container
        ScannerViewModelFactory(container.createTagReader(container.campusRepository))
    }
    val viewModel: ScannerViewModel = viewModel(factory = factory)
    val uiState by viewModel.uiState.collectAsState()

    val currentOnIdentified by rememberUpdatedState(onIdentified)
    LaunchedEffect(uiState.identified) {
        uiState.identified?.let { currentOnIdentified(it) }
    }

    ScannerContent(
        uiState = uiState,
        onPayloadRead = viewModel::onPayloadRead,
        onManualCodeChange = viewModel::onManualCodeChange,
        onManualCodeSubmit = viewModel::onManualCodeSubmit,
        onRetry = viewModel::onRejectionDismissed,
        onCancel = onCancel,
        modifier = modifier,
    )
}

@Composable
private fun ScannerContent(
    uiState: ScannerUiState,
    onPayloadRead: (String, ScanSource) -> Unit,
    onManualCodeChange: (String) -> Unit,
    onManualCodeSubmit: () -> Unit,
    onRetry: () -> Unit,
    onCancel: () -> Unit,
    modifier: Modifier = Modifier,
) {
    val context = LocalContext.current
    val activity = remember(context) { context.findActivity() }
    val finished = uiState.identified != null

    // --- Permiso de cámara -------------------------------------------------------------------
    var cameraPermission by rememberSaveable {
        mutableStateOf(
            if (context.hasCameraPermission()) CameraPermission.GRANTED else CameraPermission.REQUESTING,
        )
    }
    val permissionLauncher = rememberLauncherForActivityResult(
        ActivityResultContracts.RequestPermission(),
    ) { granted ->
        cameraPermission = when {
            granted -> CameraPermission.GRANTED
            activity != null && activity.shouldShowRequestPermissionRationale(Manifest.permission.CAMERA) ->
                CameraPermission.DENIED
            else -> CameraPermission.PERMANENTLY_DENIED
        }
    }
    LaunchedEffect(Unit) {
        if (cameraPermission == CameraPermission.REQUESTING) {
            permissionLauncher.launch(Manifest.permission.CAMERA)
        }
    }
    var cameraUnavailable by remember { mutableStateOf(false) }

    // --- NFC (solo con la pantalla en primer plano) -------------------------------------------
    var nfcAvailability by remember { mutableStateOf(NfcTagReader.availability(context)) }
    val currentOnPayloadRead by rememberUpdatedState(onPayloadRead)
    val nfcReader = remember { NfcTagReader { text -> currentOnPayloadRead(text, ScanSource.NFC) } }
    val lifecycleOwner = LocalLifecycleOwner.current
    DisposableEffect(lifecycleOwner, activity, finished) {
        val observer = LifecycleEventObserver { _, event ->
            when (event) {
                Lifecycle.Event.ON_RESUME -> {
                    // Al volver de Ajustes puede haber cambiado el permiso o el estado de NFC.
                    if (context.hasCameraPermission()) cameraPermission = CameraPermission.GRANTED
                    nfcAvailability = NfcTagReader.availability(context)
                    if (activity != null && !finished) nfcReader.enable(activity)
                }
                Lifecycle.Event.ON_PAUSE -> activity?.let(nfcReader::disable)
                else -> Unit
            }
        }
        lifecycleOwner.lifecycle.addObserver(observer)
        onDispose {
            lifecycleOwner.lifecycle.removeObserver(observer)
            activity?.let(nfcReader::disable)
        }
    }

    Scaffold(modifier = modifier) { innerPadding ->
        Column(
            modifier = Modifier
                .padding(innerPadding)
                .fillMaxSize()
                .verticalScroll(rememberScrollState())
                .padding(16.dp),
            verticalArrangement = Arrangement.spacedBy(12.dp),
        ) {
            Text(
                text = stringResource(R.string.scanner_title),
                style = MaterialTheme.typography.headlineSmall,
            )
            Text(
                text = stringResource(R.string.scanner_instructions),
                style = MaterialTheme.typography.bodyLarge,
            )

            CameraArea(
                permission = cameraPermission,
                cameraUnavailable = cameraUnavailable,
                finished = finished,
                rejectionShown = uiState.rejection != null,
                onQrRead = { onPayloadRead(it, ScanSource.QR) },
                onCameraUnavailable = { cameraUnavailable = true },
                onRequestPermission = { permissionLauncher.launch(Manifest.permission.CAMERA) },
                onOpenAppSettings = { context.openAppSettings() },
            )

            uiState.rejection?.let { rejection ->
                RejectionCard(rejection = rejection, source = uiState.rejectedSource, onRetry = onRetry)
            }

            NfcStatus(availability = nfcAvailability, onOpenNfcSettings = { context.openNfcSettings() })

            ManualCodeSection(
                code = uiState.manualCode,
                onCodeChange = onManualCodeChange,
                onSubmit = onManualCodeSubmit,
            )

            OutlinedButton(
                onClick = onCancel,
                modifier = Modifier
                    .fillMaxWidth()
                    .heightIn(min = 48.dp),
            ) {
                Text(stringResource(R.string.scanner_cancel))
            }
        }
    }
}

@Composable
private fun CameraArea(
    permission: CameraPermission,
    cameraUnavailable: Boolean,
    finished: Boolean,
    rejectionShown: Boolean,
    onQrRead: (String) -> Unit,
    onCameraUnavailable: () -> Unit,
    onRequestPermission: () -> Unit,
    onOpenAppSettings: () -> Unit,
) {
    val boxModifier = Modifier
        .fillMaxWidth()
        .aspectRatio(1f)
        .clip(RoundedCornerShape(12.dp))
        .background(MaterialTheme.colorScheme.surfaceVariant)

    when {
        permission == CameraPermission.GRANTED && !cameraUnavailable && !finished -> {
            Box(modifier = boxModifier) {
                QrCameraPreview(
                    onQrRead = onQrRead,
                    onCameraUnavailable = onCameraUnavailable,
                    rejectionShown = rejectionShown,
                    modifier = Modifier.fillMaxSize(),
                )
                // Marco de encuadre para el QR.
                Box(
                    modifier = Modifier
                        .align(Alignment.Center)
                        .fillMaxWidth(0.65f)
                        .aspectRatio(1f)
                        .border(3.dp, Color.White, RoundedCornerShape(16.dp)),
                )
                CameraActiveBadge(modifier = Modifier.align(Alignment.TopStart).padding(8.dp))
            }
        }
        else -> {
            val (message, action) = when {
                finished -> stringResource(R.string.scanner_camera_released) to null
                cameraUnavailable -> stringResource(R.string.scanner_camera_unavailable) to null
                permission == CameraPermission.REQUESTING ->
                    stringResource(R.string.scanner_camera_permission_requesting) to null
                permission == CameraPermission.DENIED ->
                    stringResource(R.string.scanner_camera_permission_denied) to
                        (stringResource(R.string.scanner_camera_permission_retry) to onRequestPermission)
                else ->
                    stringResource(R.string.scanner_camera_permission_permanently_denied) to
                        (stringResource(R.string.scanner_open_settings) to onOpenAppSettings)
            }
            Card(modifier = Modifier.fillMaxWidth()) {
                Column(
                    modifier = Modifier.padding(16.dp),
                    verticalArrangement = Arrangement.spacedBy(8.dp),
                ) {
                    Text(text = message, style = MaterialTheme.typography.bodyLarge)
                    action?.let { (label, onClick) ->
                        Button(onClick = onClick, modifier = Modifier.heightIn(min = 48.dp)) {
                            Text(label)
                        }
                    }
                }
            }
        }
    }
}

/** Indicador visible (texto + punto rojo, no solo color) de que la cámara está en uso. */
@Composable
private fun CameraActiveBadge(modifier: Modifier = Modifier) {
    Row(
        modifier = modifier
            .clip(RoundedCornerShape(50))
            .background(Color.Black.copy(alpha = 0.6f))
            .padding(horizontal = 10.dp, vertical = 6.dp),
        verticalAlignment = Alignment.CenterVertically,
    ) {
        Box(
            modifier = Modifier
                .size(10.dp)
                .clip(CircleShape)
                .background(Color.Red),
        )
        Spacer(modifier = Modifier.width(6.dp))
        Text(
            text = stringResource(R.string.scanner_camera_active),
            color = Color.White,
            style = MaterialTheme.typography.labelLarge,
        )
    }
}

@Composable
private fun RejectionCard(rejection: ScanRejection, source: ScanSource?, onRetry: () -> Unit) {
    val message = when (rejection) {
        ScanRejection.MALFORMED -> stringResource(R.string.scanner_rejected_malformed)
        ScanRejection.UNKNOWN -> when (source) {
            ScanSource.NFC -> stringResource(R.string.scanner_rejected_unknown_nfc)
            ScanSource.MANUAL -> stringResource(R.string.scanner_rejected_unknown_manual)
            else -> stringResource(R.string.scanner_rejected_unknown_qr)
        }
    }
    Card(
        modifier = Modifier
            .fillMaxWidth()
            .semantics { liveRegion = LiveRegionMode.Polite },
        colors = CardDefaults.cardColors(
            containerColor = MaterialTheme.colorScheme.errorContainer,
            contentColor = MaterialTheme.colorScheme.onErrorContainer,
        ),
    ) {
        Column(modifier = Modifier.padding(16.dp), verticalArrangement = Arrangement.spacedBy(4.dp)) {
            Text(text = message, style = MaterialTheme.typography.bodyLarge)
            TextButton(onClick = onRetry, modifier = Modifier.heightIn(min = 48.dp)) {
                Text(stringResource(R.string.scanner_retry))
            }
        }
    }
}

@Composable
private fun NfcStatus(availability: NfcAvailability, onOpenNfcSettings: () -> Unit) {
    val text = when (availability) {
        NfcAvailability.READY -> stringResource(R.string.scanner_nfc_ready)
        NfcAvailability.DISABLED -> stringResource(R.string.scanner_nfc_disabled)
        NfcAvailability.UNSUPPORTED -> stringResource(R.string.scanner_nfc_unsupported)
    }
    Column(verticalArrangement = Arrangement.spacedBy(4.dp)) {
        Text(text = text, style = MaterialTheme.typography.bodyMedium)
        if (availability == NfcAvailability.DISABLED) {
            TextButton(onClick = onOpenNfcSettings, modifier = Modifier.heightIn(min = 48.dp)) {
                Text(stringResource(R.string.scanner_nfc_open_settings))
            }
        }
    }
}

@Composable
private fun ManualCodeSection(code: String, onCodeChange: (String) -> Unit, onSubmit: () -> Unit) {
    Column(verticalArrangement = Arrangement.spacedBy(8.dp)) {
        OutlinedTextField(
            value = code,
            onValueChange = onCodeChange,
            label = { Text(stringResource(R.string.scanner_manual_label)) },
            placeholder = { Text(stringResource(R.string.scanner_manual_placeholder)) },
            singleLine = true,
            keyboardOptions = KeyboardOptions(
                capitalization = KeyboardCapitalization.None,
                autoCorrectEnabled = false,
                imeAction = ImeAction.Done,
            ),
            keyboardActions = KeyboardActions(onDone = { onSubmit() }),
            modifier = Modifier.fillMaxWidth(),
        )
        Button(
            onClick = onSubmit,
            enabled = code.isNotBlank(),
            modifier = Modifier
                .fillMaxWidth()
                .heightIn(min = 48.dp),
        ) {
            Text(stringResource(R.string.scanner_manual_submit))
        }
    }
}

private fun Context.hasCameraPermission(): Boolean =
    ContextCompat.checkSelfPermission(this, Manifest.permission.CAMERA) == PackageManager.PERMISSION_GRANTED

private tailrec fun Context.findActivity(): Activity? = when (this) {
    is Activity -> this
    is ContextWrapper -> baseContext.findActivity()
    else -> null
}

private fun Context.openAppSettings() {
    startActivity(
        Intent(Settings.ACTION_APPLICATION_DETAILS_SETTINGS, Uri.fromParts("package", packageName, null))
            .addFlags(Intent.FLAG_ACTIVITY_NEW_TASK),
    )
}

private fun Context.openNfcSettings() {
    startActivity(Intent(Settings.ACTION_NFC_SETTINGS).addFlags(Intent.FLAG_ACTIVITY_NEW_TASK))
}
