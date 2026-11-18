package com.jesusmuve.etsiitsmartcampus.feature.search

import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.heightIn
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.verticalScroll
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.size
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Place
import androidx.compose.material3.Button
import androidx.compose.material3.Card
import androidx.compose.material3.CardDefaults
import androidx.compose.material3.Icon
import androidx.compose.ui.Alignment
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.OutlinedButton
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.material3.TextButton
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.semantics.heading
import androidx.compose.ui.semantics.semantics
import androidx.compose.ui.unit.dp
import androidx.lifecycle.compose.collectAsStateWithLifecycle
import com.jesusmuve.etsiitsmartcampus.R
import com.jesusmuve.etsiitsmartcampus.ui.components.CampusHeader
import com.jesusmuve.etsiitsmartcampus.ui.components.localized
import com.jesusmuve.etsiitsmartcampus.ui.navigation.CampusRoutes

/**
 * Ficha de un espacio: nombre, planta, tipo y descripción en el idioma activo.
 *
 * Ofrece «Ruta interior hasta aquí» solo si el espacio tiene ruta demostrable y
 * siempre «Cómo llegar a la ETSIIT» (navegación exterior). «Volver» siempre disponible
 *. Bajo la descripción se muestra [extraContent] (p. ej. el botón «Escuchar»).
 *
 * @param onNavigate Navegar a otra ruta de [CampusRoutes].
 * @param onBack Volver a la pantalla anterior.
 */
@Composable
fun SpaceScreen(
    viewModel: SpaceDetailViewModel,
    onNavigate: (String) -> Unit,
    onBack: () -> Unit,
    modifier: Modifier = Modifier,
    extraContent: @Composable (spokenText: String) -> Unit = {},
) {
    val state by viewModel.uiState.collectAsStateWithLifecycle()

    Scaffold(modifier = modifier) { innerPadding ->
        Column(
            modifier = Modifier
                .padding(innerPadding)
                .fillMaxSize()
                .verticalScroll(rememberScrollState())
                .padding(16.dp),
            verticalArrangement = Arrangement.spacedBy(12.dp),
        ) {
            CampusHeader(title = stringResource(R.string.space_title), onBack = onBack)
            when (val current = state) {
                SpaceDetailUiState.NotFound -> {
                    Text(stringResource(R.string.space_not_found))
                }

                is SpaceDetailUiState.Found -> {
                    val space = current.space
                    val name = localized(space.name.es, space.name.en)
                    val description = localized(space.description.es, space.description.en)
                    val floor = stringResource(R.string.search_floor_label, space.floor)
                    val type = stringResource(spaceTypeLabel(space.type))
                    val scheme = MaterialTheme.colorScheme
                    // Tarjeta destacada con el nombre, la planta y el tipo.
                    Card(
                        colors = CardDefaults.cardColors(containerColor = scheme.primaryContainer),
                        modifier = Modifier.fillMaxWidth(),
                    ) {
                        Row(modifier = Modifier.padding(20.dp), verticalAlignment = Alignment.CenterVertically) {
                            Icon(
                                imageVector = Icons.Filled.Place,
                                contentDescription = null,
                                tint = scheme.onPrimaryContainer,
                                modifier = Modifier.size(40.dp),
                            )
                            Spacer(Modifier.size(16.dp))
                            Column {
                                Text(
                                    text = name,
                                    style = MaterialTheme.typography.headlineSmall,
                                    fontWeight = FontWeight.Bold,
                                    color = scheme.onPrimaryContainer,
                                    modifier = Modifier.semantics { heading() },
                                )
                                Text(
                                    "$floor · $type",
                                    style = MaterialTheme.typography.titleSmall,
                                    color = scheme.onPrimaryContainer,
                                )
                            }
                        }
                    }
                    Text(description, style = MaterialTheme.typography.bodyLarge)

                    if (current.hasIndoorRoute) {
                        Button(
                            onClick = { onNavigate(CampusRoutes.routeTo(space.id)) },
                            modifier = Modifier
                                .fillMaxWidth()
                                .heightIn(min = 48.dp),
                        ) { Text(stringResource(R.string.space_route_here)) }
                    } else {
                        Text(stringResource(R.string.space_no_route), style = MaterialTheme.typography.bodySmall)
                    }
                    OutlinedButton(
                        onClick = { onNavigate(CampusRoutes.OUTDOOR) },
                        modifier = Modifier
                            .fillMaxWidth()
                            .heightIn(min = 48.dp),
                    ) { Text(stringResource(R.string.outdoor_title)) }

                    extraContent("$name. $floor") // Solo el título: la descripción sigue en pantalla.
                }
            }
        }
    }
}
