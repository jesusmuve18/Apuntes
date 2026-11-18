package com.jesusmuve.etsiitsmartcampus.feature.route

import android.content.Context
import android.hardware.Sensor
import android.hardware.SensorEvent
import android.hardware.SensorEventListener
import android.hardware.SensorManager

/**
 * Presión atmosférica del barómetro (`TYPE_PRESSURE`), para detectar cambios de planta.
 *
 * Solo guarda la última lectura en hPa; la interpretación la hace [FloorChangeDetector]. Se
 * arranca y se para con el ciclo de vida de la pantalla. Muchos móviles de gama media
 * y baja no tienen barómetro: [available] lo indica y la pantalla lo dice.
 */
class BarometerSensor(context: Context) : SensorEventListener {

    private val manager = context.applicationContext.getSystemService(Context.SENSOR_SERVICE) as SensorManager
    private val sensor: Sensor? = manager.getDefaultSensor(Sensor.TYPE_PRESSURE)

    /** `false` si el móvil no tiene barómetro. */
    val available: Boolean = sensor != null

    /** Última presión en hPa, o `null` si aún no ha llegado ninguna lectura. */
    @Volatile
    var pressureHpa: Float? = null
        private set

    /** Número de lecturas recibidas; sirve para no procesar dos veces la misma. */
    @Volatile
    var sampleCount: Long = 0
        private set

    fun start() {
        val s = sensor ?: return
        manager.registerListener(this, s, SensorManager.SENSOR_DELAY_NORMAL)
    }

    fun stop() {
        manager.unregisterListener(this)
        pressureHpa = null
    }

    override fun onSensorChanged(event: SensorEvent) {
        if (event.sensor.type != Sensor.TYPE_PRESSURE) return
        pressureHpa = event.values.firstOrNull()
        sampleCount++
    }

    override fun onAccuracyChanged(sensor: Sensor?, accuracy: Int) = Unit
}
