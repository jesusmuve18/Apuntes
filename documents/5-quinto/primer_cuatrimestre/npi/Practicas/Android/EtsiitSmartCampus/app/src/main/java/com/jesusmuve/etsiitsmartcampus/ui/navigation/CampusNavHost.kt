package com.jesusmuve.etsiitsmartcampus.ui.navigation

import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.runtime.remember
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.lifecycle.viewmodel.compose.viewModel
import androidx.lifecycle.viewmodel.initializer
import androidx.lifecycle.viewmodel.viewModelFactory
import androidx.navigation.NavHostController
import androidx.navigation.NavType
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable
import androidx.navigation.compose.rememberNavController
import androidx.navigation.navArgument
import com.jesusmuve.etsiitsmartcampus.CampusApplication
import com.jesusmuve.etsiitsmartcampus.core.model.LanguageTag
import com.jesusmuve.etsiitsmartcampus.feature.audio.ListenPanel
import com.jesusmuve.etsiitsmartcampus.feature.outdoor.OutdoorScreen
import com.jesusmuve.etsiitsmartcampus.feature.outdoor.OutdoorViewModel
import com.jesusmuve.etsiitsmartcampus.feature.reservations.ReservationsScreen
import com.jesusmuve.etsiitsmartcampus.feature.reservations.ReservationsViewModel
import com.jesusmuve.etsiitsmartcampus.feature.route.IndoorRouteViewModel
import com.jesusmuve.etsiitsmartcampus.feature.schedule.ScheduleScreen
import com.jesusmuve.etsiitsmartcampus.feature.schedule.ScheduleViewModel
import com.jesusmuve.etsiitsmartcampus.feature.scanner.clearScannerResult
import com.jesusmuve.etsiitsmartcampus.feature.route.RouteScreen
import com.jesusmuve.etsiitsmartcampus.feature.scanner.ScannerScreen
import com.jesusmuve.etsiitsmartcampus.feature.scanner.putScannerResult
import com.jesusmuve.etsiitsmartcampus.feature.scanner.scannerResultFlow
import com.jesusmuve.etsiitsmartcampus.feature.search.SearchScreen
import com.jesusmuve.etsiitsmartcampus.feature.search.SearchViewModel
import com.jesusmuve.etsiitsmartcampus.feature.search.SpaceDetailViewModel
import com.jesusmuve.etsiitsmartcampus.feature.search.SpaceScreen
import com.jesusmuve.etsiitsmartcampus.ui.SettingsScreen
import com.jesusmuve.etsiitsmartcampus.ui.welcome.WelcomeScreen
import com.jesusmuve.etsiitsmartcampus.ui.welcome.WelcomeViewModel

/**
 * `NavHost` de la app: `search` como inicio, ficha `space/{spaceId}` (solo accesible por ID),
 * `route` (navegación interior), `scanner` (lector QR/NFC/manual), `settings` y
 * `outdoor` (navegación exterior).
 *
 * Cada pantalla recibe únicamente callbacks y estado, y no conoce el controlador de navegación
 *. «Volver» se enlaza a `popBackStack`.
 *
 * Los `ViewModel` de ruta y exterior se crean con fábricas que toman sus dependencias del
 * `AppContainer` (DI manual); quedan ligados a su entrada de la pila de navegación.
 *
 * Expone además un punto de composición para el idioma por app: [onApplyLanguage]
 * y [currentLanguageTag] permiten que la UI de Ajustes solicite el cambio sin invocar APIs
 * Android desde `ViewModel`. La aplicación real del locale la realiza la Activity.
 *
 * @param modifier Modifier del `NavHost`.
 * @param navController Controlador de navegación; se crea uno por defecto. Se expone para
 * que quien monta el `NavHost` pueda observar el destino actual (p. ej. para reflejarlo en
 * el estado de un `ViewModel`) sin que esta capa conozca lógica de negocio.
 * @param currentLanguageTag Tag efectivo del idioma actual (`es`/`en`) para la UI.
 * @param onApplyLanguage Callback que aplica un idioma seleccionado; lo ejecuta la Activity.
 */
@Composable
fun CampusNavHost(
    modifier: Modifier = Modifier,
    navController: NavHostController = rememberNavController(),
    currentLanguageTag: String = LanguageTag.SPANISH,
    onApplyLanguage: (String) -> Unit = {},
) {
    val application = LocalContext.current.applicationContext as CampusApplication
    val container = application.container
    // Preferencias del usuario (último destino), compartidas por búsqueda y ficha.
    val userPreferences = remember(application) {
        container.createUserPreferencesRepository(application.userPreferencesDataStore)
    }

    NavHost(
        navController = navController,
        startDestination = CampusRoutes.WELCOME,
        modifier = modifier,
    ) {
        composable(CampusRoutes.WELCOME) {
            val welcomeViewModel: WelcomeViewModel = viewModel(
                factory = viewModelFactory { initializer { WelcomeViewModel(userPreferences) } },
            )
            WelcomeScreen(
                viewModel = welcomeViewModel,
                onEnter = {
                    // La bienvenida no queda en la pila: «Volver» desde el inicio sale de la app.
                    navController.navigate(CampusRoutes.SEARCH) {
                        popUpTo(CampusRoutes.WELCOME) { inclusive = true }
                    }
                },
            )
        }
        composable(CampusRoutes.SEARCH) {
            val searchViewModel: SearchViewModel = viewModel(
                factory = viewModelFactory {
                    initializer { SearchViewModel(container.campusRepository, userPreferences) }
                },
            )
            SearchScreen(
                viewModel = searchViewModel,
                onNavigate = { route -> navController.navigate(route) },
            )
        }
        composable(
            route = CampusRoutes.SPACE_PATTERN,
            arguments = listOf(
                navArgument(CampusRoutes.ARG_SPACE_ID) { type = NavType.StringType },
            ),
        ) { backStackEntry ->
            // El identificador es obligatorio en el contrato; si falta, se pasa vacío
            // para que la ficha muestre un estado controlado y honesto, sin simular datos.
            val spaceId = backStackEntry.arguments
                ?.getString(CampusRoutes.ARG_SPACE_ID)
                .orEmpty()
            val spaceViewModel: SpaceDetailViewModel = viewModel(
                factory = viewModelFactory {
                    initializer { SpaceDetailViewModel(container.campusRepository, userPreferences, spaceId) }
                },
            )
            SpaceScreen(
                viewModel = spaceViewModel,
                onNavigate = { route -> navController.navigate(route) },
                onBack = { navController.popBackStack() },
                extraContent = { spokenText -> ListenPanel(text = spokenText) },
            )
        }
        composable(
            route = CampusRoutes.ROUTE_PATTERN,
            arguments = listOf(
                navArgument(CampusRoutes.ROUTE_ORIGIN_ARG) {
                    type = NavType.StringType
                    nullable = true
                    defaultValue = null
                },
                navArgument(CampusRoutes.ROUTE_DESTINATION_ARG) {
                    type = NavType.StringType
                    nullable = true
                    defaultValue = null
                },
            ),
        ) { backStackEntry ->
            val origin = backStackEntry.arguments?.getString(CampusRoutes.ROUTE_ORIGIN_ARG)
            val destination = backStackEntry.arguments?.getString(CampusRoutes.ROUTE_DESTINATION_ARG)
            val routeViewModel: IndoorRouteViewModel = viewModel(
                factory = viewModelFactory {
                    initializer {
                        IndoorRouteViewModel(
                            container.indoorMapSource,
                            initialOriginId = origin,
                            initialDestinationId = destination,
                        )
                    }
                },
            )
            // Punto devuelto por el lector: se entrega a la ruta, que decide si es el esperado
            //, y se marca como consumido para no procesarlo dos veces.
            val savedStateHandle = backStackEntry.savedStateHandle
            val scannedPoint by remember(savedStateHandle) { savedStateHandle.scannerResultFlow() }
                .collectAsState(initial = null)
            LaunchedEffect(scannedPoint) {
                scannedPoint?.let { point ->
                    routeViewModel.onPointIdentified(point)
                    savedStateHandle.clearScannerResult()
                }
            }
            RouteScreen(
                viewModel = routeViewModel,
                onBack = { navController.popBackStack() },
                onGoHome = { navController.popBackStack(CampusRoutes.SEARCH, inclusive = false) },
                onScan = { navController.navigate(CampusRoutes.SCANNER) },
            )
        }
        composable(CampusRoutes.SCANNER) {
            ScannerScreen(
                onIdentified = { point ->
                    navController.previousBackStackEntry?.savedStateHandle?.putScannerResult(point)
                    navController.popBackStack()
                },
                onCancel = { navController.popBackStack() },
            )
        }
        composable(CampusRoutes.RESERVATIONS) {
            val reservationsViewModel: ReservationsViewModel = viewModel(
                factory = viewModelFactory {
                    initializer {
                        ReservationsViewModel(
                            container.studyRoomsCatalog,
                            container.createReservationRepository(application.userPreferencesDataStore),
                        )
                    }
                },
            )
            ReservationsScreen(viewModel = reservationsViewModel, onBack = { navController.popBackStack() })
        }
        composable(CampusRoutes.SCHEDULE) {
            val scheduleViewModel: ScheduleViewModel = viewModel(
                factory = viewModelFactory { initializer { ScheduleViewModel(container.timetable) } },
            )
            ScheduleScreen(
                viewModel = scheduleViewModel,
                onBack = { navController.popBackStack() },
                onOpenRoute = { room -> navController.navigate(CampusRoutes.routeToRoom(room)) },
            )
        }
        composable(CampusRoutes.SETTINGS) {
            SettingsScreen(
                onBack = { navController.popBackStack() },
                currentLanguageTag = currentLanguageTag,
                onApplyLanguage = onApplyLanguage,
            )
        }
        composable(CampusRoutes.OUTDOOR) { backStackEntry ->
            val outdoorViewModel: OutdoorViewModel = viewModel(
                factory = viewModelFactory {
                    initializer { OutdoorViewModel(container.indoorMapSource, container.createLocationTracker()) }
                },
            )
            // Punto de la entrada escaneado para confirmar que ya está dentro: se entrega a la
            // pantalla exterior, que abre la ruta interior desde ese nodo.
            val savedStateHandle = backStackEntry.savedStateHandle
            val scannedPoint by remember(savedStateHandle) { savedStateHandle.scannerResultFlow() }
                .collectAsState(initial = null)
            LaunchedEffect(scannedPoint) {
                scannedPoint?.let { point ->
                    outdoorViewModel.onPointIdentified(point)
                    savedStateHandle.clearScannerResult()
                }
            }
            OutdoorScreen(
                viewModel = outdoorViewModel,
                onStartIndoorRoute = { originNodeId ->
                    navController.navigate(CampusRoutes.routeFrom(originNodeId))
                },
                onBack = { navController.popBackStack() },
                onScan = { navController.navigate(CampusRoutes.SCANNER) },
            )
        }
    }
}
