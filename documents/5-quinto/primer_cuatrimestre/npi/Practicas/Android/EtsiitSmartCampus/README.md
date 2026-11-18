# ETSIIT Smart Campus

Aplicación Android de orientación por la ETSIIT (Universidad de Granada) para la práctica de
**interacción por sensores** de *Nuevos Paradigmas de Interacción* (curso 2026-2027).

Ayuda a llegar a la escuela (Google Maps + aviso de llegada por GPS), a buscar un espacio y a
seguir una **ruta interior paso a paso** confirmando cada punto con **QR, NFC o código manual**.
El lector se abre con un botón o **asintiendo dos veces con el móvil** (gesto G1). Incluye
**«¿Voy bien?»** (brújula calibrada), **lectura en voz alta** (botón o acercar el móvil a la
oreja) y está en **español e inglés**. Funciona sin Internet salvo Google Maps.

Además tiene una **bienvenida** con nombre opcional (no es un inicio de sesión: no hay cuenta
ni contraseña), una **reserva de salas de estudio de la biblioteca de demostración**, con
salas de ejemplo y reservas guardadas solo en el móvil, y la consulta del **horario** de 1.º A,
desde el que se abre la ruta hasta el aula de cada clase.

## Lenguaje y versiones

| | Versión |
|---|---|
| Lenguaje | **Kotlin 2.2.10** (interfaz con Jetpack Compose) |
| Android | `minSdk 26` (Android 8.0) · `compileSdk 35` · `targetSdk 35` |
| Compilación | Android Gradle Plugin 9.5.1 · Gradle 9.6.0 · JDK 21 (incluido en Android Studio) |

## Librerías

No se incluyen en la entrega: **Gradle las descarga automáticamente** la primera vez (hace falta
Internet solo entonces) desde [Google Maven](https://maven.google.com) y
[Maven Central](https://central.sonatype.com). Las versiones están fijadas en
[`gradle/libs.versions.toml`](gradle/libs.versions.toml).

| Librería | Versión | Uso | Documentación |
|---|---|---|---|
| Jetpack Compose BOM (UI, Material 3) | 2024.09.00 | Interfaz | <https://developer.android.com/develop/ui/compose> |
| AndroidX Core KTX | 1.10.1 | Utilidades | <https://developer.android.com/kotlin/ktx> |
| AndroidX AppCompat | 1.8.0 | Idioma por aplicación | <https://developer.android.com/jetpack/androidx/releases/appcompat> |
| Activity Compose | 1.8.0 | Actividad y permisos | <https://developer.android.com/jetpack/androidx/releases/activity> |
| Lifecycle (runtime-ktx / runtime-compose / viewmodel-compose) | 2.6.1 / 2.8.4 / 2.8.4 | ViewModel y ciclo de vida | <https://developer.android.com/jetpack/androidx/releases/lifecycle> |
| Navigation Compose | 2.8.4 | Navegación | <https://developer.android.com/guide/navigation> |
| DataStore Preferences | 1.1.1 | Último destino | <https://developer.android.com/topic/libraries/architecture/datastore> |
| CameraX (core, camera2, lifecycle, view) | 1.4.1 | Cámara del lector QR | <https://developer.android.com/media/camera/camerax> |
| ML Kit Barcode Scanning (*bundled*) | 17.3.0 | QR sin conexión | <https://developers.google.com/ml-kit/vision/barcode-scanning/android> |
| kotlinx.serialization JSON | 1.9.0 | Lectura de los JSON | <https://kotlinlang.org/docs/serialization.html> |

NFC (`NfcAdapter`), sensores (`SensorManager`), ubicación (`LocationManager`) y voz
(`TextToSpeech`) son APIs del propio Android: no requieren dependencias. Licencias en
[`docs/THIRD_PARTY_LICENSES.md`](docs/THIRD_PARTY_LICENSES.md).

## Instalar y ejecutar

1. Instala [Android Studio](https://developer.android.com/studio) (incluye el JDK) y, desde el
   SDK Manager, **Android SDK 35**.
2. Abre en Android Studio **esta carpeta** (`EtsiitSmartCampus`, no la raíz del repositorio) y
   espera a que Gradle sincronice y descargue las librerías.
3. En el móvil activa *Opciones de desarrollador* → *Depuración USB*, conéctalo y pulsa **Run**.
   También puedes generar el APK e instalarlo:

   ```bash
./gradlew assembleDebug
   adb install app/build/outputs/apk/debug/app-debug.apk
   ```


Para probarlo todo hace falta un **móvil real** con cámara trasera, NFC, acelerómetro, sensor de
proximidad, brújula (vector de rotación) y GPS. El emulador sirve para la interfaz, pero no para
NFC, los gestos ni la lectura real de QR. Ningún sensor es obligatorio: sin él, la función se
sustituye por su botón.

### Puntos de control

Abre [`docs/qr/qr-puntos.html`](docs/qr/qr-puntos.html) en el navegador e imprímelo: contiene el
QR de cada punto de la ruta. Para NFC, graba en una etiqueta el
mismo texto que el QR como registro de **Texto** (por ejemplo con la app *NFC Tools*). El texto
bajo cada QR es también el **código manual**.

## Organización del código

`app/src/main/java/com/jesusmuve/etsiitsmartcampus/`

| Paquete | Contenido |
|---|---|
| (raíz) | `MainActivity` (UI e idioma), `CampusApplication`, `AppContainer` (dependencias, sin Hilt) |
| `core/model` | Modelos: `Space`, `IndoorMap` (nodos, tramos y rutas), `IdentifiedPoint`… |
| `core/data` | Lectura y validación de JSON, mapa interior y preferencias (DataStore) |
| `feature/search` | Búsqueda de espacios y ficha |
| `feature/outdoor` | Cómo llegar: Google Maps, GPS de llegada y confirmación al entrar |
| `feature/route` | Ruta interior de muestra (fija: ida y vuelta, escalera o ascensor), guiado, plano multitáctil y «¿Voy bien?» |
| `feature/scanner` | Lector QR (CameraX + ML Kit), NFC y código manual |
| `feature/gestures` | Gesto G1 (asentir dos veces) y gesto de acercar a la oreja |
| `feature/audio` | Lectura en voz alta (`TextToSpeech`) y panel «Escuchar» |
| `feature/reservations` | Reserva de salas de estudio (demostración, solo en el móvil) |
| `feature/schedule` | Consulta del horario y acceso a la ruta hasta el aula |
| `ui` | Navegación, bienvenida, tema, ajustes y componentes comunes |

Datos en `app/src/main/assets/`: `campus.json` (espacios),
`indoor/graph.json` (mapa interior y rutas de muestra), `library/study_rooms.json` (salas de
estudio de ejemplo), `schedule/timetable.json` (horario) y los planos y fotos. Textos de la interfaz en
`res/values/strings.xml` (español) y `res/values-en/strings.xml` (inglés).

Más documentación: guía del gesto en
[`GUIA_GESTO_G1.md`](GUIA_GESTO_G1.md) y ruta interior en
[`docs/ruta-interior.md`](docs/ruta-interior.md).
