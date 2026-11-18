# Ruta interior y navegación exterior

## 1. Cómo está montado

```
app/src/main/assets/
├── indoor/
│   ├── graph.json          ← nodos, tramos, plantas y datos del edificio
│   ├── plans/              ← plano de cada planta (renders del modelo 3D de la ETSIIT)
│   │   ├── planta_baja.jpg
│   │   └── planta_segunda.jpg
│   └── photos/             ← fotos de la ruta (<nodo>_ida.jpg / <nodo>_vuelta.jpg)
└── outdoor/etsiit.jpg      ← imagen del edificio en la pantalla exterior
```

**Las posiciones de los nodos en los planos son aproximadas** (`planX`/`planY` en `graph.json`).

## 2. Ruta de muestra

```
entrada → paseo → entrada_b (Edificio B) → pasillo_p0 ─┬─ escalera_p0 ═(escalera)═ escalera_p2 ─┬─ pasillo_p2 → aula_2_2
                                                         └─ ascensor_p0 ═(ascensor)═ ascensor_p2 ─┘
```

- Recorrido real (según las fotos): cancela de entrada → paseo cubierto junto al muro azul del
  **edificio A** (el de la izquierda en el plano) → puertas del **Edificio B** («Aulas y
  Laboratorios», el del fondo) → pasillo → escalera de caracol o ascensor → segunda planta → aula
  2.2 («Aula Profesor Pedro Cano»).
- **Solo hay rutas fijas** (no se calcula ningún camino). Están escritas a mano en la lista
  `routes` de `graph.json`:

  | `id` | Recorrido | Se usa cuando… |
  |---|---|---|
  | `ida` | entrada → … → escalera → … → aula 2.2 | Ida |
  | `ida_ascensor` | entrada → … → ascensor → … → aula 2.2 | Ida con «Evitar escaleras» |
  | `vuelta` | aula 2.2 → … → escalera → … → entrada | Vuelta |
  | `vuelta_ascensor` | aula 2.2 → … → ascensor → … → entrada | Vuelta con «Evitar escaleras» |

- En la pantalla se elige **punto de partida** y **destino** entre todos los nodos (y, si se
  quiere, «Evitar escaleras»). Se puede elegir cualquiera, pero **solo funcionan entrada → aula 2.2
  y aula 2.2 → entrada**; con el resto la app avisa de que esa ruta no está en la demostración.
- Desde **«Consultar horarios»**, al tocar una clase se abre la ruta hacia su aula. Fundamentos de
  Programación (1.er cuatrimestre) se da en el **aula 2.2**, así que esa abre la ruta de verdad; las
  demás aulas salen como destino («Aula 0.7»…) y avisan al empezar.

## 3. Fotos

Las fotos están en `assets/indoor/photos/` (reducidas a 1280 px) y se llaman `<nodo>_ida.jpg`
(yendo al aula 2.2) o `<nodo>_vuelta.jpg` (volviendo a la entrada); `exterior.jpg` es la fachada.
Cada parada de una ruta (`stops` en `graph.json`) lleva la foto que se enseña mientras el usuario
está en ese paso; la de la última parada sale en la pantalla de llegada. **El orden lo ha fijado el
equipo**; para cambiar una foto basta con cambiar el `photoAsset` de esa parada.

| Paso | Ida (escalera) | Foto | Vuelta (escalera) | Foto |
|---|---|---|---|---|
| 1 | entrada → paseo | `exterior.jpg` | aula 2.2 → pasillo P2 | `aula_2_2_vuelta.jpg` |
| 2 | paseo → Edificio B | `entrada_ida.jpg` | pasillo P2 → escalera P2 | `escalera_p2_vuelta.jpg` |
| 3 | Edificio B → pasillo P0 | `entrada_b_ida.jpg` | bajar la escalera | (sin foto) |
| 4 | pasillo P0 → escalera P0 | `pasillo_p0_ida.jpg` | escalera P0 → pasillo P0 | `escalera_p0_vuelta.jpg` |
| 5 | subir la escalera | `escalera_p0_ida.jpg` | pasillo P0 → Edificio B | `pasillo_p0_vuelta.jpg` |
| 6 | escalera P2 → pasillo P2 | (sin foto) | Edificio B → paseo | `entrada_b_vuelta.jpg` |
| 7 | pasillo P2 → aula 2.2 | `escalera_p2_ida.jpg` | paseo → entrada | `entrada_vuelta.jpg` |
| Llegada | aula 2.2 | `aula_2_2_ida.jpg` | entrada | `exterior.jpg` |

**Con «Evitar escaleras»** las rutas son iguales cambiando la escalera por el ascensor: en el paso
que en la ida lleva `escalera_p0_ida.jpg` sale `ascensor_p0_ida.jpg`, y en la vuelta, en lugar de
`escalera_p0_vuelta.jpg`, también `ascensor_p0_ida.jpg` (solo hay foto del ascensor en la planta
baja). El paso dentro del ascensor no lleva foto.

## 4. Editar `graph.json`

- **Nodo:** `id` (también será el contenido del QR/NFC de ese punto), `floorId`, nombres y
  descripciones ES/EN, `photoAsset` y `planX`/`planY` (posición en el plano, de 0 a 1: 0,0 es la
  esquina superior izquierda de la imagen).
- **Tramo:** `from`, `to`, `kind` (`WALK`, `STAIRS`, `ELEVATOR`), instrucción ES/EN y, si se
  puede recorrer al revés, `reverseInstructionEs`/`reverseInstructionEn` (la vuelta los necesita).
- **Ruta de muestra** (`routes`): `id`, `accessible` (`true` = por ascensor) y `stops`, la lista
  de paradas en orden, cada una con `nodeId` y `photoAsset`. Entre cada dos paradas seguidas tiene
  que existir un tramo del grafo en ese sentido; si no, la app no carga la ruta.
- **Edificio:** coordenadas de la entrada (`entranceLatitude`, `entranceLongitude`), radio de
  llegada `arrivalRadiusMeters` (20 m) y `provisional`, que si vale `true` muestra un aviso de
  «datos de prueba» en la ruta. Con un radio tan pequeño, las coordenadas deben ser las de la
  propia puerta; se pueden obtener en Google Maps manteniendo pulsado sobre ese punto.
- **Orientación de los planos** (brújula): `planUpBearingDegrees`, `planDepthFactor`,
  `planAspectRatio`. Ver el apartado 7.
- **Plantas:** cada una lleva `level` (0, 2…); el barómetro lo usa para saber cuántas plantas hay
  que subir. `floorHeightMeters` (4 m) es la altura aproximada entre plantas.
- **Rumbo fijo de un tramo (opcional):** si la dirección calculada con el plano no sirve en algún
  tramo, se puede poner `bearingDegrees` (grados desde el norte) en ese tramo y manda sobre el
  cálculo. Al recorrerlo al revés se usa el opuesto.
- Para añadir otra planta: imagen en `plans/` (por ejemplo `Transparencias/PrimeraSola.jpg` del
  modelo 3D) y una entrada en `floors`.

## 5. Lector QR/NFC y gesto

El lector (`feature/scanner`) y el gesto G1 (`feature/gestures`) están integrados en la ruta: el gesto o el
botón abren el lector y `CampusNavHost` entrega el punto leído a la ruta, que llama a:

```kotlin
routeViewModel.onPointIdentified(IdentifiedPoint(id = textoLeido, source = ScanSource.QR))
```

Si el ID es el siguiente punto, la ruta avanza y avisa «… confirmado con el código QR»; si no, avisa
y no avanza. Los QR deben contener **exactamente** el `id` del nodo.

Desde la pantalla exterior, el lector sirve para confirmar que el usuario ya está dentro: el punto
va a `OutdoorViewModel.onPointIdentified`, que abre la ruta interior desde ese nodo.

## 6. Licencias y créditos

- Planos e imagen del edificio: renders del modelo 3D de la ETSIIT (`Modelo_3D_ETSIIT`) facilitado
  en la asignatura.
- Coordenadas de la entrada: página «Localización y contacto» de la ETSIIT (etsiit.ugr.es).
- La gestión multitáctil se inspira en el ejemplo `MultiTouchView` de la asignatura
  (M. Cabrera, 2017), reescrito para Jetpack Compose.

## 7. Brújula «¿Voy bien?» con el rumbo real

**Qué hace:** en cada tramo a pie, la app calcula hacia dónde está de verdad el siguiente punto
(grados desde el norte) con las posiciones de los dos nodos en el plano. La brújula del móvil dice
hacia dónde apunta el teléfono. Se dibujan dos flechas: **gruesa** (siguiente punto) y **fina**
(móvil). Si se solapan (±20°), todo se pone **verde** y dice «Vas bien»; si no, **rojo** y dice
«Gira a la izquierda / derecha» o «Date la vuelta». «Calibrar» empieza la medición (y la reinicia).
Funciona con el móvil plano o en vertical.

**Cómo pasa del plano a la realidad** (`PlanBearing.kt`):
- `planUpBearingDegrees = 290`: la parte de **arriba** de los planos (el pico del edificio) mira a
  290° desde el norte, en sentido horario (es decir, 70° hacia el oeste). **Medido en la entrada el
  13/11/2026**: con 60° la flecha del primer tramo salía unos 130° desviada respecto a la dirección
  real. Si todas las flechas salieran desviadas el mismo ángulo, bastaría con corregir este
  número (por ejemplo, sumar 15 si la dirección buena queda siempre 15° a la derecha).
- `planDepthFactor = 0,67`: los planos son vistas 3D inclinadas y la profundidad sale acortada. Se
  ha medido con el bloque cuadrado azul de la planta baja: con 0,67 sus cuatro lados miden lo mismo.
- `planAspectRatio = 1,3333`: las imágenes son de 1024 × 768.

**Límites:** cerca de pilares, ordenadores o estructuras metálicas la brújula se desvía. Si el
sistema marca la lectura como poco fiable, la app pide mover el móvil haciendo un ocho. Escaleras
y ascensores no llevan brújula (el camino lo marca la escalera); llevan el barómetro.

## 8. Cambio de planta con el barómetro

En los tramos de escalera o ascensor, la app mide la presión del aire al empezar el tramo y va
calculando cuántos metros se suben o se bajan (al subir, la presión baja ~0,12 hPa por metro).
Cuando se alcanza el 60 % de la altura esperada (`plantas × floorHeightMeters`), vibra y dice
«Parece que ya estás en la segunda planta». **No avanza la ruta sola**: hay que
confirmar con «Ya estoy aquí» o con el QR/NFC. También avisa si se sube cuando hay que bajar.

Muchos móviles de gama media no tienen barómetro: entonces la app lo dice y todo lo demás funciona.

