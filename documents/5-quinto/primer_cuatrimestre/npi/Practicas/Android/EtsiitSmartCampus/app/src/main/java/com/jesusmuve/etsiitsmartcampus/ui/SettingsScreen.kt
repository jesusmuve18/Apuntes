package com.jesusmuve.etsiitsmartcampus.ui

import androidx.annotation.StringRes
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.heightIn
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.selection.selectable
import androidx.compose.foundation.selection.selectableGroup
import androidx.compose.foundation.verticalScroll
import androidx.compose.material3.Card
import androidx.compose.material3.HorizontalDivider
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.RadioButton
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.material3.TextButton
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.semantics.Role
import androidx.compose.ui.semantics.heading
import androidx.compose.ui.semantics.semantics
import androidx.compose.ui.unit.dp
import com.jesusmuve.etsiitsmartcampus.R
import com.jesusmuve.etsiitsmartcampus.ui.components.CampusHeader
import com.jesusmuve.etsiitsmartcampus.core.model.LanguageTag

/**
 * Ajustes y ayuda.
 *
 * - **Idioma**: selector ES/EN. La pantalla no cambia el idioma por sí misma: llama a
 *   [onApplyLanguage] y la `Activity` lo aplica con AppCompat, que además lo recuerda.
 * - **Ayuda**: cómo usar cada función, incluidos los gestos y sus botones equivalentes.
 *
 * @param currentLanguageTag Idioma efectivo (`es`/`en`).
 * @param onApplyLanguage Aplica el idioma elegido (lo ejecuta la Activity).
 * @param onBack Volver a la pantalla anterior.
 */
@Composable
fun SettingsScreen(
    onBack: () -> Unit,
    currentLanguageTag: String = LanguageTag.SPANISH,
    onApplyLanguage: (String) -> Unit = {},
    modifier: Modifier = Modifier,
) {
    Scaffold(modifier = modifier) { innerPadding ->
        Column(
            modifier = Modifier
                .padding(innerPadding)
                .fillMaxSize()
                .verticalScroll(rememberScrollState())
                .padding(16.dp),
            verticalArrangement = Arrangement.spacedBy(12.dp),
        ) {
            CampusHeader(title = stringResource(R.string.settings_title), onBack = onBack)

            LanguageSelector(currentLanguageTag = currentLanguageTag, onApplyLanguage = onApplyLanguage)

            HorizontalDivider()
            Text(
                text = stringResource(R.string.settings_help_title),
                style = MaterialTheme.typography.titleLarge,
                modifier = Modifier.semantics { heading() },
            )
            helpSections.forEach { (title, body) -> HelpCard(title, body) }
        }
    }
}

@Composable
private fun LanguageSelector(currentLanguageTag: String, onApplyLanguage: (String) -> Unit) {
    Text(
        text = stringResource(R.string.settings_language_title),
        style = MaterialTheme.typography.titleLarge,
        modifier = Modifier.semantics { heading() },
    )
    Column(modifier = Modifier.selectableGroup()) {
        listOf(
            LanguageTag.SPANISH to R.string.settings_language_es,
            LanguageTag.ENGLISH to R.string.settings_language_en,
        ).forEach { (tag, label) ->
            val selected = LanguageTag.normalize(currentLanguageTag) == tag
            Row(
                verticalAlignment = Alignment.CenterVertically,
                modifier = Modifier
                    .fillMaxWidth()
                    .heightIn(min = 48.dp)
                    .selectable(
                        selected = selected,
                        role = Role.RadioButton,
                        onClick = { if (!selected) onApplyLanguage(tag) },
                    ),
            ) {
                // El texto acompaña al círculo marcado; el estado no depende solo del color.
                RadioButton(selected = selected, onClick = null)
                Text(stringResource(label), modifier = Modifier.padding(start = 12.dp))
            }
        }
    }
    Text(stringResource(R.string.settings_language_hint), style = MaterialTheme.typography.bodySmall)
}

@Composable
private fun HelpCard(@StringRes title: Int, @StringRes body: Int) {
    Card(modifier = Modifier.fillMaxWidth()) {
        Column(modifier = Modifier.padding(16.dp), verticalArrangement = Arrangement.spacedBy(4.dp)) {
            Text(
                text = stringResource(title),
                style = MaterialTheme.typography.titleMedium,
                modifier = Modifier.semantics { heading() },
            )
            Text(stringResource(body), style = MaterialTheme.typography.bodyMedium)
        }
    }
}

/** Secciones de ayuda (título, texto), en el orden en que se usa la aplicación. */
private val helpSections = listOf(
    R.string.help_outdoor_title to R.string.help_outdoor_text,
    R.string.help_search_title to R.string.help_search_text,
    R.string.help_route_title to R.string.help_route_text,
    R.string.help_g1_title to R.string.help_g1_text,
    R.string.help_orientation_title to R.string.help_orientation_text,
    R.string.help_listen_title to R.string.help_listen_text,
    R.string.help_reservations_title to R.string.help_reservations_text,
    R.string.help_data_title to R.string.help_data_text,
)
