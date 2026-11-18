package com.jesusmuve.etsiitsmartcampus.feature.search

import androidx.annotation.StringRes
import androidx.compose.foundation.Image
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.PaddingValues
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.heightIn
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.foundation.shape.CircleShape
import androidx.compose.foundation.text.KeyboardOptions
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.automirrored.filled.KeyboardArrowRight
import androidx.compose.material.icons.automirrored.filled.List
import androidx.compose.material.icons.filled.DateRange
import androidx.compose.material.icons.filled.LocationOn
import androidx.compose.material.icons.filled.Place
import androidx.compose.material.icons.filled.PlayArrow
import androidx.compose.material.icons.filled.Refresh
import androidx.compose.material.icons.filled.Search
import androidx.compose.material.icons.filled.Settings
import androidx.compose.material3.Card
import androidx.compose.material3.CardDefaults
import androidx.compose.material3.Icon
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.OutlinedTextField
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.clip
import androidx.compose.ui.graphics.Brush
import androidx.compose.ui.graphics.vector.ImageVector
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.semantics.contentDescription
import androidx.compose.ui.semantics.heading
import androidx.compose.ui.semantics.semantics
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.text.input.ImeAction
import androidx.compose.ui.unit.dp
import androidx.lifecycle.compose.collectAsStateWithLifecycle
import com.jesusmuve.etsiitsmartcampus.R
import com.jesusmuve.etsiitsmartcampus.core.model.Space
import com.jesusmuve.etsiitsmartcampus.core.model.SpaceTypePresentation
import com.jesusmuve.etsiitsmartcampus.core.model.spaceTypePresentationOf
import com.jesusmuve.etsiitsmartcampus.ui.components.localized
import com.jesusmuve.etsiitsmartcampus.ui.navigation.CampusRoutes

/**
 * Pantalla de inicio.
 *
 * Cabecera con el logo y el saludo, accesos directos al resto de funciones, buscador, último
 * destino consultado y lista de espacios (todo el catálogo si la consulta está vacía). Pinta el
 * estado de [SearchViewModel] y le pasa las acciones.
 *
 * @param onNavigate Navegar a otra ruta de [CampusRoutes] (ficha, exterior, ruta, ajustes…).
 */
@Composable
fun SearchScreen(
    viewModel: SearchViewModel,
    onNavigate: (String) -> Unit,
    modifier: Modifier = Modifier,
) {
    val state by viewModel.uiState.collectAsStateWithLifecycle()

    Scaffold(modifier = modifier) { innerPadding ->
        LazyColumn(
            modifier = Modifier
                .padding(innerPadding)
                .fillMaxSize(),
            contentPadding = PaddingValues(16.dp),
            verticalArrangement = Arrangement.spacedBy(12.dp),
        ) {
            item { HomeHeader(userName = state.userName) }
            item { QuickAccess(onNavigate = onNavigate) }
            item {
                Text(
                    text = stringResource(R.string.search_title),
                    style = MaterialTheme.typography.titleLarge,
                    fontWeight = FontWeight.SemiBold,
                    modifier = Modifier
                        .padding(top = 8.dp)
                        .semantics { heading() },
                )
            }
            item {
                val fieldDescription = stringResource(R.string.search_query_content_description)
                OutlinedTextField(
                    value = state.query,
                    onValueChange = viewModel::onQueryChange,
                    label = { Text(stringResource(R.string.search_query_label)) },
                    placeholder = { Text(stringResource(R.string.search_query_placeholder)) },
                    leadingIcon = { Icon(Icons.Filled.Search, contentDescription = null) },
                    singleLine = true,
                    shape = MaterialTheme.shapes.medium,
                    keyboardOptions = KeyboardOptions(imeAction = ImeAction.Search),
                    modifier = Modifier
                        .fillMaxWidth()
                        .semantics { contentDescription = fieldDescription },
                )
            }
            state.lastDestination?.let { last ->
                item {
                    LastDestinationCard(
                        space = last,
                        onClick = { viewModel.onLastDestinationSelected(onNavigate) },
                    )
                }
            }
            when (val results = state.results) {
                is SearchResults.Spaces -> items(results.items, key = { it.id }) { space ->
                    SpaceRow(space = space, onClick = { viewModel.onSpaceSelected(space, onNavigate) })
                }
                SearchResults.QueryEmpty -> item {
                    Text(stringResource(R.string.search_no_results_text), color = MaterialTheme.colorScheme.onSurfaceVariant)
                }
                is SearchResults.CatalogError -> item {
                    Text(stringResource(R.string.search_catalog_error_text), color = MaterialTheme.colorScheme.error)
                }
            }
        }
    }
}

/** Cabecera granate con el logo, el saludo (con el nombre de la bienvenida) y el lema. */
@Composable
private fun HomeHeader(userName: String?) {
    val scheme = MaterialTheme.colorScheme
    Card(
        colors = CardDefaults.cardColors(containerColor = scheme.primary),
        shape = MaterialTheme.shapes.large,
        modifier = Modifier.fillMaxWidth(),
    ) {
        Row(
            verticalAlignment = Alignment.CenterVertically,
            modifier = Modifier
                .background(Brush.horizontalGradient(listOf(scheme.primary, scheme.primary.copy(alpha = 0.85f))))
                .padding(20.dp),
        ) {
            Image(
                painter = painterResource(R.drawable.app_logo),
                contentDescription = null,
                modifier = Modifier
                    .size(56.dp)
                    .clip(CircleShape)
                    .background(scheme.onPrimary),
            )
            Spacer(Modifier.size(16.dp))
            Column {
                Text(
                    text = if (userName != null) {
                        stringResource(R.string.search_greeting, userName)
                    } else {
                        stringResource(R.string.search_greeting_anonymous)
                    },
                    style = MaterialTheme.typography.headlineSmall,
                    fontWeight = FontWeight.Bold,
                    color = scheme.onPrimary,
                    modifier = Modifier.semantics { heading() },
                )
                Text(
                    text = stringResource(R.string.welcome_tagline),
                    style = MaterialTheme.typography.bodyMedium,
                    color = scheme.onPrimary.copy(alpha = 0.9f),
                )
            }
        }
    }
}

/** Un acceso directo del inicio: icono, título y destino. */
private data class QuickAction(val icon: ImageVector, @StringRes val title: Int, val route: String)

private val quickActions = listOf(
    QuickAction(Icons.Filled.LocationOn, R.string.outdoor_title, CampusRoutes.OUTDOOR),
    QuickAction(Icons.Filled.PlayArrow, R.string.route_title, CampusRoutes.ROUTE),
    QuickAction(Icons.Filled.DateRange, R.string.reservations_title, CampusRoutes.RESERVATIONS),
    QuickAction(Icons.AutoMirrored.Filled.List, R.string.schedule_title, CampusRoutes.SCHEDULE),
    QuickAction(Icons.Filled.Settings, R.string.settings_title, CampusRoutes.SETTINGS),
)

/** Accesos directos en cuadrícula de dos columnas, con icono y texto. */
@Composable
private fun QuickAccess(onNavigate: (String) -> Unit) {
    Column(verticalArrangement = Arrangement.spacedBy(12.dp)) {
        quickActions.chunked(2).forEach { row ->
            Row(horizontalArrangement = Arrangement.spacedBy(12.dp)) {
                row.forEach { action ->
                    QuickActionTile(action, onClick = { onNavigate(action.route) }, modifier = Modifier.weight(1f))
                }
                if (row.size == 1) Spacer(Modifier.weight(1f))
            }
        }
    }
}

@Composable
private fun QuickActionTile(action: QuickAction, onClick: () -> Unit, modifier: Modifier = Modifier) {
    val scheme = MaterialTheme.colorScheme
    Card(
        onClick = onClick,
        colors = CardDefaults.cardColors(containerColor = scheme.secondaryContainer),
        modifier = modifier.heightIn(min = 96.dp),
    ) {
        Column(
            modifier = Modifier.padding(16.dp),
            verticalArrangement = Arrangement.spacedBy(8.dp),
        ) {
            Icon(action.icon, contentDescription = null, tint = scheme.onSecondaryContainer, modifier = Modifier.size(28.dp))
            Text(
                text = stringResource(action.title),
                style = MaterialTheme.typography.titleSmall,
                fontWeight = FontWeight.SemiBold,
                color = scheme.onSecondaryContainer,
            )
        }
    }
}

@Composable
private fun LastDestinationCard(space: Space, onClick: () -> Unit) {
    val description = stringResource(R.string.search_last_destination_content_description)
    val scheme = MaterialTheme.colorScheme
    Card(
        onClick = onClick,
        colors = CardDefaults.cardColors(containerColor = scheme.tertiaryContainer),
        modifier = Modifier
            .fillMaxWidth()
            .semantics { contentDescription = description },
    ) {
        Row(modifier = Modifier.padding(16.dp), verticalAlignment = Alignment.CenterVertically) {
            Icon(Icons.Filled.Refresh, contentDescription = null, tint = scheme.onTertiaryContainer)
            Spacer(Modifier.size(12.dp))
            Column(modifier = Modifier.weight(1f)) {
                Text(
                    stringResource(R.string.search_last_destination_title),
                    style = MaterialTheme.typography.labelLarge,
                    color = scheme.onTertiaryContainer,
                )
                Text(
                    localized(space.name.es, space.name.en),
                    style = MaterialTheme.typography.titleMedium,
                    color = scheme.onTertiaryContainer,
                )
            }
            Icon(Icons.AutoMirrored.Filled.KeyboardArrowRight, contentDescription = null, tint = scheme.onTertiaryContainer)
        }
    }
}

@Composable
private fun SpaceRow(space: Space, onClick: () -> Unit) {
    val name = localized(space.name.es, space.name.en)
    val description = stringResource(R.string.search_space_item_content_description, name)
    val scheme = MaterialTheme.colorScheme
    Card(
        onClick = onClick,
        colors = CardDefaults.cardColors(containerColor = scheme.surfaceVariant.copy(alpha = 0.55f)),
        modifier = Modifier
            .fillMaxWidth()
            .heightIn(min = 64.dp)
            .semantics(mergeDescendants = true) { contentDescription = description },
    ) {
        Row(modifier = Modifier.padding(14.dp), verticalAlignment = Alignment.CenterVertically) {
            Icon(
                imageVector = if (space.hasRoute) Icons.Filled.Place else Icons.Filled.LocationOn,
                contentDescription = null,
                tint = scheme.primary,
                modifier = Modifier.size(28.dp),
            )
            Spacer(Modifier.size(12.dp))
            Column(modifier = Modifier.weight(1f)) {
                Text(name, style = MaterialTheme.typography.titleMedium)
                Text(
                    text = stringResource(R.string.search_floor_label, space.floor) + " · " +
                        stringResource(spaceTypeLabel(space.type)),
                    style = MaterialTheme.typography.bodyMedium,
                    color = scheme.onSurfaceVariant,
                )
            }
            Icon(Icons.AutoMirrored.Filled.KeyboardArrowRight, contentDescription = null, tint = scheme.onSurfaceVariant)
        }
    }
}

/** Etiqueta legible del tipo de espacio; nunca se muestra la clave técnica. */
@StringRes
internal fun spaceTypeLabel(rawType: String): Int = when (spaceTypePresentationOf(rawType)) {
    SpaceTypePresentation.Classroom -> R.string.space_type_classroom
    SpaceTypePresentation.Laboratory -> R.string.space_type_laboratory
    SpaceTypePresentation.Office -> R.string.space_type_office
    SpaceTypePresentation.Library -> R.string.space_type_library
    SpaceTypePresentation.Cafeteria -> R.string.space_type_cafeteria
    SpaceTypePresentation.Entrance -> R.string.space_type_entrance
    SpaceTypePresentation.Stairs -> R.string.space_type_stairs
    SpaceTypePresentation.Hall -> R.string.space_type_hall
    SpaceTypePresentation.Common -> R.string.space_type_common
    SpaceTypePresentation.Unknown -> R.string.space_type_unknown
}
