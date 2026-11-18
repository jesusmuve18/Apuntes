# Gesto G1: asentir dos veces para abrir el lector

El gesto se reconoce comparando **el ángulo inicial de inclinación** del móvil con **el ángulo
actual**, medidos con el sensor de gravedad (o el acelerómetro).

## Qué significa «1 de 2» y «2 de 2»

El contador cuenta **asentimientos completos** (`G1_REQUIRED_PHASES = 2`):

- **Calibración:** al sostener el móvil con la pantalla hacia arriba unos 70 ms se guarda el ángulo inicial.
- **Un asentimiento:** levantar el borde superior **+33°** o más respecto a esa postura (objetivo natural:
  unos 45°) y volver a **±20°** de la postura inicial, habiendo bajado al menos el 40 % del ascenso máximo.
- **1 de 2** aparece al terminar el primer asentimiento; al terminar el segundo (**2 de 2**) se abre el lector.

## Cómo usarlo

1. Busca la ruta interior y pulsa «Iniciar ruta». El gesto solo está activo durante el guiado.
2. Sostén el teléfono con la pantalla hacia arriba (no vertical) y espera un instante.
3. Asiente dos veces: sube el borde superior unos 45° respecto al inicio y vuelve, dos veces.
4. Se abre el **lector QR/NFC/manual**. Al leer un punto, la ruta decide si es el siguiente.
5. Al volver a la ruta el gesto se reactiva solo. Si no se usa el gesto, el botón
   «Escanear punto de control» hace lo mismo.

En el panel se muestra cuántos asentimientos lleva (0, 1 o 2).

## Parámetros configurables

Abre `app/src/main/java/com/jesusmuve/etsiitsmartcampus/feature/gestures/Gestures.kt`.
En `GestureConfig` puedes modificar:

- `riseDegrees = 33f`: grados que hay que subir en cada asentimiento.
  Para hacerlo aún más sensible, probar `28f` (aumenta falsos positivos).
- `returnToleranceDegrees = 20f`: margen en grados para reconocer el regreso
  y pasar a **2 de 2**.
- `maxInitialPitchDegrees = 55f`: máximo ángulo inicial en valor absoluto.
- `armDurationNs = 70_000_000L`: tiempo de medición de la referencia inicial.
- `maxGestureDurationNs = 9_000_000_000L`: duración máxima desde el pico hasta
  la vuelta. No impone velocidad mínima.
- `minDescentFraction = 0.40f`: porcentaje mínimo de bajada real desde el pico.

## Implementación

- `Gestures.kt`: reconocedor, sin dependencias de Android, con estados `ARMING`, `WAITING_FOR_RISE`,
  `WAITING_FOR_RETURN`, `FIRED`; compara ángulos en grados directamente.
- `ScanGestureSensorController.kt`: utiliza `Sensor.TYPE_GRAVITY` si el dispositivo
  dispone de él. Como alternativa, usa acelerómetro con filtro ligero.
- `ScanGesturePanel.kt`: contador de asentimientos y botón equivalente; se muestra en la fase
  de guiado de `RouteScreen`.
- `strings.xml` y `values-en/strings.xml`: instrucciones en español e inglés.

No se usa el giroscopio: basta con la inclinación, así el gesto vale igual hecho rápido o
despacio.

El gesto y el botón abren el lector real (`CampusRoutes.SCANNER`).
La detección solo funciona en la fase de guiado de la ruta y con la app en primer plano.

