package com.jesusmuve.etsiitsmartcampus.ui.theme

import androidx.compose.foundation.isSystemInDarkTheme
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Shapes
import androidx.compose.material3.darkColorScheme
import androidx.compose.material3.lightColorScheme
import androidx.compose.runtime.Composable
import androidx.compose.ui.unit.dp
import androidx.compose.foundation.shape.RoundedCornerShape

private val LightColorScheme = lightColorScheme(
    primary = Granate,
    onPrimary = OnGranate,
    primaryContainer = GranateContainer,
    onPrimaryContainer = OnGranateContainer,
    secondary = Pizarra,
    onSecondary = OnPizarra,
    secondaryContainer = PizarraContainer,
    onSecondaryContainer = OnPizarraContainer,
    tertiary = Verde,
    onTertiary = OnVerde,
    tertiaryContainer = VerdeContainer,
    onTertiaryContainer = OnVerdeContainer,
    background = FondoClaro,
    onBackground = OnFondoClaro,
    surface = FondoClaro,
    onSurface = OnFondoClaro,
    surfaceVariant = SuperficieVariante,
    onSurfaceVariant = OnSuperficieVariante,
    outline = Contorno,
)

private val DarkColorScheme = darkColorScheme(
    primary = GranateOscuro,
    onPrimary = OnGranateOscuro,
    primaryContainer = GranateContainerOscuro,
    onPrimaryContainer = OnGranateContainerOscuro,
    secondary = PizarraOscuro,
    onSecondary = OnPizarraOscuro,
    secondaryContainer = PizarraContainerOscuro,
    onSecondaryContainer = OnPizarraContainerOscuro,
    tertiary = VerdeOscuro,
    onTertiary = OnVerdeOscuro,
    tertiaryContainer = VerdeContainerOscuro,
    onTertiaryContainer = OnVerdeContainerOscuro,
    background = FondoOscuro,
    onBackground = OnFondoOscuro,
    surface = FondoOscuro,
    onSurface = OnFondoOscuro,
    surfaceVariant = SuperficieVarianteOscuro,
    onSurfaceVariant = OnSuperficieVarianteOscuro,
    outline = ContornoOscuro,
)

/** Esquinas redondeadas algo más marcadas que las de Material por defecto. */
private val CampusShapes = Shapes(
    small = RoundedCornerShape(10.dp),
    medium = RoundedCornerShape(16.dp),
    large = RoundedCornerShape(24.dp),
)

/**
 * Tema de ETSIIT Smart Campus (Material 3).
 *
 * Aplica una **paleta fija** propia (granate, azul pizarra y verde) en claro y oscuro: no se usa
 * el color dinámico del sistema (Material You), decisión, para que la identidad visual sea
 * la misma en todos los dispositivos. El color nunca transmite información por sí solo.
 *
 * @param darkTheme `true` si el sistema está en modo oscuro.
 * @param content Contenido al que se aplica el tema.
 */
@Composable
fun ETSIITSmartCampusTheme(
    darkTheme: Boolean = isSystemInDarkTheme(),
    content: @Composable () -> Unit,
) {
    MaterialTheme(
        colorScheme = if (darkTheme) DarkColorScheme else LightColorScheme,
        typography = Typography,
        shapes = CampusShapes,
        content = content,
    )
}
