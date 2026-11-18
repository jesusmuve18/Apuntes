package com.jesusmuve.etsiitsmartcampus.feature.gestures

import android.content.Context
import android.hardware.Sensor
import android.hardware.SensorEvent
import android.hardware.SensorEventListener
import android.hardware.SensorManager

/**
 * Escucha el sensor de **proximidad** y la gravedad (o el acelerómetro filtrado) y avisa con
 * [onDetected] cuando [EarGestureRecognizer] reconoce que el móvil se ha llevado a la oreja.
 *
 * Solo funciona mientras la pantalla está en primer plano: la pantalla llama a [start] y
 * [stop] según su ciclo de vida. Sin sensor de proximidad, [available] es `false` y la
 * pantalla deja solo el botón «Escuchar».
 */
class EarGestureSensorController(
    context: Context,
    private val onDetected: () -> Unit,
) : SensorEventListener {

    private val manager = context.applicationContext.getSystemService(Context.SENSOR_SERVICE) as SensorManager
    private val proximity: Sensor? = manager.getDefaultSensor(Sensor.TYPE_PROXIMITY)
    private val gravitySensor: Sensor? = manager.getDefaultSensor(Sensor.TYPE_GRAVITY)
        ?: manager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER)
    private val recognizer = EarGestureRecognizer()
    private val gravity = FloatArray(3)
    private var gravityInitialized = false
    private var near = false

    /** `true` si el móvil tiene sensor de proximidad y de gravedad/acelerómetro. */
    val available: Boolean = proximity != null && gravitySensor != null

    fun start() {
        if (!available) return
        recognizer.reset()
        near = false
        gravityInitialized = false
        proximity?.let { manager.registerListener(this, it, SensorManager.SENSOR_DELAY_NORMAL) }
        gravitySensor?.let { manager.registerListener(this, it, SensorManager.SENSOR_DELAY_UI) }
    }

    fun stop() {
        manager.unregisterListener(this)
    }

    override fun onSensorChanged(event: SensorEvent) {
        when (event.sensor.type) {
            Sensor.TYPE_PROXIMITY -> {
                // Muchos sensores solo dan dos valores: 0 (cerca) o el alcance máximo (lejos).
                near = event.values[0] < event.sensor.maximumRange
            }
            Sensor.TYPE_GRAVITY -> {
                for (i in 0..2) gravity[i] = event.values[i]
                gravityInitialized = true
            }
            Sensor.TYPE_ACCELEROMETER -> {
                for (i in 0..2) {
                    gravity[i] = if (gravityInitialized) {
                        LOW_PASS_ALPHA * gravity[i] + (1f - LOW_PASS_ALPHA) * event.values[i]
                    } else {
                        event.values[i]
                    }
                }
                gravityInitialized = true
            }
            else -> return
        }
        if (!gravityInitialized) return
        val sample = EarSample(event.timestamp, near, gravity[1], gravity[2])
        if (recognizer.accept(sample)) onDetected()
    }

    override fun onAccuracyChanged(sensor: Sensor?, accuracy: Int) = Unit

    private companion object {
        const val LOW_PASS_ALPHA = 0.8f
    }
}
