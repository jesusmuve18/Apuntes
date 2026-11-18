package com.jesusmuve.etsiitsmartcampus.feature.scanner

import android.util.Log
import android.view.ViewGroup
import androidx.camera.core.CameraSelector
import androidx.camera.core.ImageAnalysis
import androidx.camera.core.Preview
import androidx.camera.lifecycle.ProcessCameraProvider
import androidx.camera.view.PreviewView
import androidx.compose.runtime.Composable
import androidx.compose.runtime.DisposableEffect
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.remember
import androidx.compose.runtime.rememberUpdatedState
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.viewinterop.AndroidView
import androidx.core.content.ContextCompat
import androidx.lifecycle.compose.LocalLifecycleOwner
import java.util.concurrent.Executors

private const val TAG = "QrCameraPreview"

/**
 * Vista previa de la cámara trasera con análisis QR, vinculada al ciclo de vida de la pantalla.
 *
 * La cámara solo existe mientras este composable está en composición: al salir (lectura válida,
 * cancelar o volver) se llama a `unbindAll()` y se libera ML Kit. Al pasar a segundo plano,
 * CameraX la detiene automáticamente por estar ligada al `LifecycleOwner`.
 *
 * @param onQrRead Texto bruto de cada QR nuevo detectado (hilo principal).
 * @param onCameraUnavailable Se invoca si no se puede abrir la cámara (sin cámara, en uso…).
 * @param rejectionShown Mientras se muestra un error no se repite el mismo QR; al ocultarlo se
 * permite volver a leerlo (reintento).
 */
@Composable
internal fun QrCameraPreview(
    onQrRead: (String) -> Unit,
    onCameraUnavailable: () -> Unit,
    rejectionShown: Boolean,
    modifier: Modifier = Modifier,
) {
    val context = LocalContext.current
    val lifecycleOwner = LocalLifecycleOwner.current
    val currentOnQrRead by rememberUpdatedState(onQrRead)
    val currentOnCameraUnavailable by rememberUpdatedState(onCameraUnavailable)

    val previewView = remember {
        PreviewView(context).apply {
            layoutParams = ViewGroup.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT,
            )
            scaleType = PreviewView.ScaleType.FILL_CENTER
            implementationMode = PreviewView.ImplementationMode.COMPATIBLE
        }
    }
    val analyzer = remember { QrCodeAnalyzer { value -> currentOnQrRead(value) } }

    LaunchedEffect(rejectionShown) {
        if (!rejectionShown) analyzer.resetLastValue()
    }

    DisposableEffect(lifecycleOwner) {
        val analysisExecutor = Executors.newSingleThreadExecutor()
        val providerFuture = ProcessCameraProvider.getInstance(context)
        var cameraProvider: ProcessCameraProvider? = null
        var disposed = false

        providerFuture.addListener({
            if (disposed) return@addListener
            try {
                val provider = providerFuture.get()
                cameraProvider = provider
                val preview = Preview.Builder().build().also {
                    it.setSurfaceProvider(previewView.surfaceProvider)
                }
                val analysis = ImageAnalysis.Builder()
                    .setBackpressureStrategy(ImageAnalysis.STRATEGY_KEEP_ONLY_LATEST)
                    .build()
                    .also { it.setAnalyzer(analysisExecutor, analyzer) }
                provider.unbindAll()
                provider.bindToLifecycle(
                    lifecycleOwner,
                    CameraSelector.DEFAULT_BACK_CAMERA,
                    preview,
                    analysis,
                )
            } catch (e: Exception) {
                // Sin cámara trasera, cámara ocupada por otra app, etc. No se bloquea el lector:
                // quedan NFC y código manual.
                Log.w(TAG, "No se pudo abrir la cámara", e)
                currentOnCameraUnavailable()
            }
        }, ContextCompat.getMainExecutor(context))

        onDispose {
            disposed = true
            analyzer.stop()
            cameraProvider?.unbindAll()
            analysisExecutor.shutdown()
        }
    }

    AndroidView(factory = { previewView }, modifier = modifier)
}
