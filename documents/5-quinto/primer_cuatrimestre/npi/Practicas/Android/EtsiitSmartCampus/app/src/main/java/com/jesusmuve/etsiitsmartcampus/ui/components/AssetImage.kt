package com.jesusmuve.etsiitsmartcampus.ui.components

import android.content.Context
import android.graphics.BitmapFactory
import androidx.compose.runtime.Composable
import androidx.compose.runtime.State
import androidx.compose.runtime.produceState
import androidx.compose.ui.graphics.ImageBitmap
import androidx.compose.ui.graphics.asImageBitmap
import androidx.compose.ui.platform.LocalContext
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext
import java.io.IOException

/** Estado de una imagen cargada desde `assets/`. */
sealed interface AssetImageState {
    /** Se está leyendo. */
    data object Loading : AssetImageState

    /** No existe o no se puede decodificar (p. ej. foto aún no añadida). */
    data object Missing : AssetImageState

    /** Imagen lista para dibujar. */
    data class Ready(val bitmap: ImageBitmap) : AssetImageState
}

/**
 * Carga una imagen de `assets/` fuera del hilo principal y la reduce si es muy grande.
 *
 * Se hace a mano con `BitmapFactory` en lugar de añadir una librería de imágenes.
 * Las fotos de móvil pesan mucho: se submuestrean para que el lado mayor no pase de
 * [maxDimension] píxeles y así no agotar la memoria.
 *
 * @param assetPath Ruta dentro de `assets/`; con `null` el resultado es [AssetImageState.Missing].
 */
@Composable
fun rememberAssetImage(assetPath: String?, maxDimension: Int = 2048): State<AssetImageState> {
    val appContext = LocalContext.current.applicationContext
    return produceState<AssetImageState>(initialValue = AssetImageState.Loading, assetPath, maxDimension) {
        value = if (assetPath == null) {
            AssetImageState.Missing
        } else {
            withContext(Dispatchers.IO) { decodeAsset(appContext, assetPath, maxDimension) }
        }
    }
}

private fun decodeAsset(context: Context, path: String, maxDimension: Int): AssetImageState = try {
    val bounds = BitmapFactory.Options().apply { inJustDecodeBounds = true }
    context.assets.open(path).use { BitmapFactory.decodeStream(it, null, bounds) }

    var sample = 1
    while (bounds.outWidth / sample > maxDimension || bounds.outHeight / sample > maxDimension) {
        sample *= 2
    }
    val options = BitmapFactory.Options().apply { inSampleSize = sample }
    val bitmap = context.assets.open(path).use { BitmapFactory.decodeStream(it, null, options) }
    if (bitmap == null) AssetImageState.Missing else AssetImageState.Ready(bitmap.asImageBitmap())
} catch (e: IOException) {
    // Si un punto no tiene foto, se muestra un aviso, no un error.
    AssetImageState.Missing
}
