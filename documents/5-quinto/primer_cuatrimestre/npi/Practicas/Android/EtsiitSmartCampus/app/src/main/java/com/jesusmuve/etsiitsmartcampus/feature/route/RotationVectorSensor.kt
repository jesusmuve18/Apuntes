package com.jesusmuve.etsiitsmartcampus.feature.route

import android.content.Context
import android.hardware.GeomagneticField
import android.hardware.Sensor
import android.hardware.SensorEvent
import android.hardware.SensorEventListener
import android.hardware.SensorManager
import kotlin.math.abs
import kotlin.math.atan2

/**
 * Rumbo del móvil (hacia dónde apunta) a partir de `TYPE_ROTATION_VECTOR`, para
 * «¿Voy bien?».
 *
 * - **Móvil plano o en vertical:** con el móvil plano, «hacia dónde apunta» es el borde de arriba;
 *   sostenido en vertical delante de la cara, es la dirección de la cámara trasera. Se elige el
 *   eje que esté más horizontal, así funciona de las dos formas sin que el usuario tenga que pensar.
 * - **Norte geográfico:** la brújula da el norte magnético; se le suma la declinación del lugar
 *   (`GeomagneticField`) para comparar con los rumbos del plano, que son geográficos.
 * - **Suavizado:** [HeadingSmoother] evita que la flecha tiemble.
 *
 * Solo guarda la última lectura; la comparación la hace [OrientationChecker]. Se arranca y se
 * para con el ciclo de vida de la pantalla.
 *
 * @param latitude Latitud del edificio, para calcular la declinación magnética.
 * @param longitude Longitud del edificio.
 */
class RotationVectorSensor(
    context: Context,
    latitude: Double,
    longitude: Double,
) : SensorEventListener {

    private val manager = context.applicationContext.getSystemService(Context.SENSOR_SERVICE) as SensorManager
    private val sensor: Sensor? = manager.getDefaultSensor(Sensor.TYPE_ROTATION_VECTOR)
    private val rotationMatrix = FloatArray(9)
    private val smoother = HeadingSmoother()
    private var accuracy = SensorManager.SENSOR_STATUS_ACCURACY_MEDIUM

    /** Declinación magnética del lugar en grados (positiva hacia el este). */
    private val declinationDegrees: Float = GeomagneticField(
        latitude.toFloat(),
        longitude.toFloat(),
        ALTITUDE_METERS,
        System.currentTimeMillis(),
    ).declination

    /** `false` si el móvil no tiene vector de rotación: la pantalla lo dice. */
    val available: Boolean = sensor != null

    /**
     * Último rumbo hacia el que apunta el móvil, en grados [0, 360) desde el norte geográfico, o
     * `null` si aún no ha llegado ninguna lectura.
     */
    @Volatile
    var headingDegrees: Float? = null
        private set

    /**
     * `true` si el móvil está en vertical y se usa la dirección de la cámara trasera; `false` si
     * está más bien plano y se usa el borde de arriba. Solo para el modo de prueba.
     */
    @Volatile
    var upright: Boolean = false
        private set

    /** Declinación magnética aplicada (grados). Solo para el modo de prueba. */
    val declination: Float get() = declinationDegrees

    /** Precisión que informa el sistema (`SensorManager.SENSOR_STATUS_*`). Solo para el modo de prueba. */
    val accuracyLevel: Int get() = accuracy

    /** `false` si el sistema marca la lectura como poco fiable (brújula sin calibrar, imanes…). */
    val reliable: Boolean
        get() = accuracy != SensorManager.SENSOR_STATUS_UNRELIABLE &&
            accuracy != SensorManager.SENSOR_STATUS_NO_CONTACT

    fun start() {
        val s = sensor ?: return
        manager.registerListener(this, s, SensorManager.SENSOR_DELAY_UI)
    }

    fun stop() {
        manager.unregisterListener(this)
        headingDegrees = null
    }

    /** Vuelve a empezar el suavizado (al pulsar «Calibrar»). */
    fun resetSmoothing() {
        smoother.reset()
    }

    override fun onSensorChanged(event: SensorEvent) {
        if (event.sensor.type != Sensor.TYPE_ROTATION_VECTOR) return
        // Algunos dispositivos envían 5 valores y getRotationMatrixFromVector solo admite 3 o 4.
        val values = if (event.values.size > 4) event.values.copyOf(4) else event.values
        SensorManager.getRotationMatrixFromVector(rotationMatrix, values)
        upright = abs(rotationMatrix[7]) >= UPRIGHT_THRESHOLD
        val magnetic = pointingAzimuth(rotationMatrix) ?: return
        headingDegrees = smoother.add(OrientationChecker.normalize(magnetic + declinationDegrees))
    }

    override fun onAccuracyChanged(sensor: Sensor?, accuracy: Int) {
        this.accuracy = accuracy
    }

    private companion object {
        /** Altitud aproximada de Granada; apenas influye en la declinación. */
        const val ALTITUDE_METERS = 700f

        /**
         * Por encima de esta componente vertical, el borde superior del móvil apunta demasiado
         * hacia arriba o hacia abajo para usarlo y se toma la dirección de la cámara trasera.
         */
        const val UPRIGHT_THRESHOLD = 0.7f

        /** Si el eje elegido es casi vertical, no hay dirección horizontal fiable. */
        const val MIN_HORIZONTAL = 0.2f

        /**
         * Rumbo magnético (grados) hacia el que apunta el móvil, o `null` si no se puede saber.
         *
         * La matriz [r] pasa de ejes del móvil a ejes del mundo (este, norte, arriba). Su columna 1
         * es el eje Y del móvil (borde de arriba) y la columna 2, el eje Z (sale de la pantalla);
         * la cámara trasera mira hacia −Z.
         */
        fun pointingAzimuth(r: FloatArray): Float? {
            val topUp = r[7]
            val (east, north) = if (abs(topUp) < UPRIGHT_THRESHOLD) {
                r[1] to r[4] // móvil más bien plano: borde de arriba
            } else {
                -r[2] to -r[5] // móvil en vertical: cámara trasera
            }
            if (abs(east) < MIN_HORIZONTAL && abs(north) < MIN_HORIZONTAL) return null
            return OrientationChecker.normalize(Math.toDegrees(atan2(east.toDouble(), north.toDouble())).toFloat())
        }
    }
}
