package com.jesusmuve.etsiitsmartcampus.feature.gestures

import android.content.Context
import android.hardware.Sensor
import android.hardware.SensorEvent
import android.hardware.SensorEventListener
import android.hardware.SensorManager

/**
 * Envía grados de inclinación al reconocedor. Se prefiere TYPE_GRAVITY, que ya
 * elimina la aceleración lineal; si falta, usa el acelerómetro con filtro suave.
 * No se consulta ni integra el giroscopio: la detección depende de los ÁNGULOS.
 */
class ScanGestureSensorController(
    context: Context,
    private val onDetected: () -> Unit,
    private val onProgress: (Int) -> Unit = {},
    private val onAnglesChanged: (Int, Int?) -> Unit = { _, _ -> },
) : SensorEventListener {
    private val manager = context.applicationContext
        .getSystemService(Context.SENSOR_SERVICE) as SensorManager
    private val gravitySensor = manager.getDefaultSensor(Sensor.TYPE_GRAVITY)
    private val accelerometer = manager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER)
    private val sensor = gravitySensor ?: accelerometer

    val available: Boolean = sensor != null

    private val recognizer = ScanGestureRecognizer()
    private val gravity = FloatArray(3)
    private var gravityInitialized = false
    private var listening = false
    private var previousProgress = 0
    private var lastReportNs = 0L

    fun start(): Boolean {
        if (listening) return true
        val availableSensor = sensor ?: return false
        recognizer.reset()
        gravityInitialized = false
        previousProgress = 0
        lastReportNs = 0L
        onProgress(0)
        // Activarlo antes de registrar evita perder las primeras muestras.
        listening = true
        val registered = manager.registerListener(
            this, availableSensor, SensorManager.SENSOR_DELAY_GAME,
        )
        if (!registered) {
            listening = false
            manager.unregisterListener(this)
            return false
        }
        return true
    }

    fun stop() {
        listening = false
        manager.unregisterListener(this)
    }

    override fun onAccuracyChanged(sensor: Sensor?, accuracy: Int) = Unit

    override fun onSensorChanged(event: SensorEvent) {
        if (!listening) return
        if (event.sensor.type != sensor?.type) return
        if (event.sensor.type == Sensor.TYPE_GRAVITY) {
            for (i in 0..2) gravity[i] = event.values[i]
        } else {
            // Alternativa para teléfonos sin TYPE_GRAVITY: filtro del acelerómetro.
            val alpha = 0.60f
            for (i in 0..2) {
                gravity[i] = if (!gravityInitialized) event.values[i]
                    else alpha * gravity[i] + (1f - alpha) * event.values[i]
            }
        }
        gravityInitialized = true

        val sample = MotionSample(
            timestampNs = event.timestamp,
            gravityX = gravity[0],
            gravityY = gravity[1],
            gravityZ = gravity[2],
        )
        val detected = recognizer.accept(sample)
        val phase = recognizer.completedPhases
        if (phase != previousProgress) {
            previousProgress = phase
            onProgress(phase)
        }
        if (lastReportNs == 0L || event.timestamp - lastReportNs >= 100_000_000L) {
            lastReportNs = event.timestamp
            onAnglesChanged(recognizer.pitchDegrees(sample), recognizer.differenceDegrees(sample))
        }
        if (detected) {
            stop() // evita múltiples aperturas hasta pulsar "Reactivar gesto"
            onDetected()
        }
    }
}
