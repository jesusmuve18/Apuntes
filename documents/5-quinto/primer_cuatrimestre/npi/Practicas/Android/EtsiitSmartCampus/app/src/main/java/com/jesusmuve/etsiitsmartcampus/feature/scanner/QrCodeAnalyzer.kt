package com.jesusmuve.etsiitsmartcampus.feature.scanner

import androidx.annotation.OptIn
import androidx.camera.core.ExperimentalGetImage
import androidx.camera.core.ImageAnalysis
import androidx.camera.core.ImageProxy
import com.google.mlkit.vision.barcode.BarcodeScanner
import com.google.mlkit.vision.barcode.BarcodeScannerOptions
import com.google.mlkit.vision.barcode.BarcodeScanning
import com.google.mlkit.vision.barcode.common.Barcode
import com.google.mlkit.vision.common.InputImage
import java.util.concurrent.atomic.AtomicBoolean

/**
 * Analizador de CameraX que detecta códigos QR con ML Kit Barcode Scanning (versión bundled,
 * funciona sin Internet ni Google Play Services).
 *
 * - Solo formato QR.
 * - Cierra siempre el [ImageProxy] al terminar ML Kit; si no, CameraX deja de entregar frames.
 * - Mientras ML Kit procesa un frame se descartan los siguientes (no se encolan tareas).
 * - No repite el mismo contenido de forma consecutiva, para no disparar eventos en bucle
 *   mientras el QR sigue delante de la cámara.
 * - Tras [stop] deja de analizar (los frames que lleguen se cierran sin procesar).
 *
 * @param onQrRead Se invoca en el hilo principal con el texto bruto del QR.
 */
class QrCodeAnalyzer(private val onQrRead: (String) -> Unit) : ImageAnalysis.Analyzer {

    private val scanner: BarcodeScanner = BarcodeScanning.getClient(
        BarcodeScannerOptions.Builder()
            .setBarcodeFormats(Barcode.FORMAT_QR_CODE)
            .build(),
    )

    private val stopped = AtomicBoolean(false)
    private val busy = AtomicBoolean(false)

    @Volatile
    private var lastValue: String? = null

    @OptIn(ExperimentalGetImage::class)
    override fun analyze(imageProxy: ImageProxy) {
        val mediaImage = imageProxy.image
        if (stopped.get() || mediaImage == null || !busy.compareAndSet(false, true)) {
            imageProxy.close()
            return
        }
        val image = InputImage.fromMediaImage(mediaImage, imageProxy.imageInfo.rotationDegrees)
        scanner.process(image)
            .addOnSuccessListener { barcodes ->
                val value = barcodes.firstNotNullOfOrNull { it.rawValue }
                if (value != null && value != lastValue && !stopped.get()) {
                    lastValue = value
                    onQrRead(value)
                }
            }
            .addOnCompleteListener {
                busy.set(false)
                imageProxy.close()
            }
    }

    /** Permite volver a notificar el último valor (p. ej. tras mostrar un error y reintentar). */
    fun resetLastValue() {
        lastValue = null
    }

    /** Deja de analizar y libera el detector de ML Kit. */
    fun stop() {
        if (stopped.compareAndSet(false, true)) {
            scanner.close()
        }
    }
}
