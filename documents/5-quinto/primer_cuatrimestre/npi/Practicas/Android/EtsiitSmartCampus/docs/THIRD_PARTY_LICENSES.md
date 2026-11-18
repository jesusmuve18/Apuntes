# Licencias de terceros

ETSIIT Smart Campus usa las siguientes librerías y recursos de terceros. Las librerías no se
distribuyen con el código fuente: Gradle las descarga en la versión indicada en
`gradle/libs.versions.toml`.

## Librerías

| Componente | Autor | Licencia |
|---|---|---|
| AndroidX (Core, AppCompat, Activity, Lifecycle, Navigation, DataStore) | Google / The Android Open Source Project | Apache License 2.0 |
| Jetpack Compose (UI, Material 3) | Google / The Android Open Source Project | Apache License 2.0 |
| CameraX | Google / The Android Open Source Project | Apache License 2.0 |
| ML Kit Barcode Scanning | Google | Términos de ML Kit (<https://developers.google.com/ml-kit/terms>) |
| kotlinx.serialization | JetBrains | Apache License 2.0 |
| Kotlin estándar | JetBrains | Apache License 2.0 |

Texto de la licencia Apache 2.0: <https://www.apache.org/licenses/LICENSE-2.0>.

## Recursos

- **Planos de planta e imagen del edificio:** renders del modelo 3D de la ETSIIT
  (*Modelo 3D ETSIIT*, material de la asignatura en PRADO). Uso docente.
- **Coordenadas de la entrada:** página «Localización y contacto» de la ETSIIT (etsiit.ugr.es).
- **Gestión multitáctil del plano:** inspirada en el ejemplo `MultiTouchView` de la asignatura
  (M. Cabrera, 2017), reescrita para Jetpack Compose.
- **Google Maps:** se abre como aplicación externa mediante su URL pública; no se integra su SDK
  ni se usa ninguna clave de API.
