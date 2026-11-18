package com.jesusmuve.etsiitsmartcampus.ui.components

import android.content.Context
import android.os.Build
import android.os.VibrationEffect
import android.os.Vibrator
import android.os.VibratorManager

/**
 * Vibración breve como confirmación táctil (p. ej. al reconocer el gesto G1).
 *
 * Solo complementa lo que ya se ve en pantalla; si el móvil no vibra o el usuario lo tiene
 * desactivado no pasa nada. Requiere el permiso normal `VIBRATE` (sin diálogo).
 */
object Haptics {

    private const val SHORT_PULSE_MS = 80L

    fun shortPulse(context: Context) {
        val vibrator: Vibrator? = if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
            (context.getSystemService(Context.VIBRATOR_MANAGER_SERVICE) as? VibratorManager)?.defaultVibrator
        } else {
            @Suppress("DEPRECATION")
            context.getSystemService(Context.VIBRATOR_SERVICE) as? Vibrator
        }
        if (vibrator?.hasVibrator() != true) return
        vibrator.vibrate(VibrationEffect.createOneShot(SHORT_PULSE_MS, VibrationEffect.DEFAULT_AMPLITUDE))
    }
}
