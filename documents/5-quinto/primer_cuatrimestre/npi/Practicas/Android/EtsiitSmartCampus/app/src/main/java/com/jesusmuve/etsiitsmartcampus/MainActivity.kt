package com.jesusmuve.etsiitsmartcampus

import android.os.Bundle
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.appcompat.app.AppCompatDelegate
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Surface
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.ui.Modifier
import androidx.core.os.LocaleListCompat
import androidx.lifecycle.viewmodel.compose.viewModel
import androidx.navigation.compose.currentBackStackEntryAsState
import androidx.navigation.compose.rememberNavController
import com.jesusmuve.etsiitsmartcampus.core.model.LanguageTag
import com.jesusmuve.etsiitsmartcampus.ui.MainViewModel
import com.jesusmuve.etsiitsmartcampus.ui.navigation.CampusNavHost
import com.jesusmuve.etsiitsmartcampus.ui.theme.ETSIITSmartCampusTheme

/** Actividad principal y punto de entrada de la interfaz de ETSIIT Smart Campus. */
class MainActivity : AppCompatActivity() {

    /** Contenedor de dependencias manual de la app, obtenido desde [CampusApplication]. */
    private val appContainer: AppContainer
        get() = (application as CampusApplication).container

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContent {
            ETSIITSmartCampusTheme {
                // El ViewModel no necesita dependencias, así que se crea con la fábrica por defecto.
                val mainViewModel: MainViewModel = viewModel()

                val navController = rememberNavController()

                // Cada cambio de pantalla se comunica al ViewModel.
                val backStackEntry by navController.currentBackStackEntryAsState()
                LaunchedEffect(backStackEntry) {
                    backStackEntry?.destination?.route?.let { route ->
                        mainViewModel.onDestinationChanged(route)
                    }
                }

                // Idioma actual de la interfaz. Al cambiarlo, AppCompat recrea la actividad.
                val currentLanguageTag = resolveCurrentLanguageTag()

                // Cada pantalla tiene su propio Scaffold, que ya aplica los márgenes de la barra de
                // estado: aquí solo se pone el fondo, para no duplicar ese margen.
                Surface(modifier = Modifier.fillMaxSize(), color = MaterialTheme.colorScheme.background) {
                    CampusNavHost(
                        navController = navController,
                        currentLanguageTag = currentLanguageTag,
                        onApplyLanguage = ::applyLanguageTag,
                    )
                }
            }
        }
    }

    /**
     * Aplica el idioma seleccionado desde la UI de Ajustes.
     *
     * @param languageTag Idioma elegido: `es` o `en`.
     */
    private fun applyLanguageTag(languageTag: String) {
        val normalizedTag = LanguageTag.normalize(languageTag)
        AppCompatDelegate.setApplicationLocales(
            LocaleListCompat.forLanguageTags(normalizedTag),
        )
    }

    /** Idioma actual de la app (`es` o `en`). */
    private fun resolveCurrentLanguageTag(): String {
        val applicationLocales = AppCompatDelegate.getApplicationLocales()
        val appTag = if (!applicationLocales.isEmpty) applicationLocales[0]?.toLanguageTag() else null
        val effectiveTag = appTag ?: resources.configuration.locales[0]?.toLanguageTag()
        return LanguageTag.normalize(effectiveTag)
    }
}
