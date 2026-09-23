(define (domain tierra-media)

  ; Requisitos
  ; :typing          -> permite usar tipos en parámetros y predicados
  ; :action-costs -> permite definir funciones numéricas y modificarlas en efectos
  ;                     (necesario para costes de acción no unitarios en el Ejercicio 5)
  (:requirements :typing :action-costs)

  ; Tipos (sin cambios respecto al Ejercicio 1)
  ; Jerarquía:
  ;   object
  ;     - Localizacion
  ;     - Recurso
  ;           - TipoRecurso
  ;           - NodoRecurso
  ;     - Personaje
  ;           - TipoPersonaje
  ;
  (:types
    Localizacion  - object  ; lugares del mapa (Hobbiton, Bree, Rivendell, etc.)
    Recurso       - object
    TipoRecurso   - Recurso ; tipo de recurso extraíble (Mineral, Mithril, Madera, etc.)
    NodoRecurso   - Recurso ; nodo/yacimiento de un recurso en una localización concreta
    Personaje     - object  ; instancias concretas de personajes (Enano1, Hobbit1, etc.)
    TipoPersonaje - object  ; categoría de personaje (Enano, Hobbit, etc.)
  )

  ; Constantes
  (:constants
    Enano  Hobbit  - TipoPersonaje                         ; razas de personaje
    Mineral Mithril Madera Especia Alimento - TipoRecurso  ; recursos posibles
  )

  ; Predicados

  ; (estaEn ?p ?l)
  ;   Cierto si el personaje ?p se encuentra en la localización ?l.
  (:predicates
    (estaEn ?p - Personaje ?l - Localizacion)

    ; (nodoEn ?n ?l)
    ;   Cierto si el nodo de recurso ?n está en la localización ?l.
    (nodoEn ?n - NodoRecurso ?l - Localizacion)

    ; (tipoNodo ?n ?r)
    ;   Relaciona un nodo con el tipo de recurso que produce.
    (tipoNodo ?n - NodoRecurso ?r - TipoRecurso)

    ; (camino ?l1 ?l2)
    ;   Cierto si existe un camino transitable entre ?l1 y ?l2.
    (camino ?l1 - Localizacion ?l2 - Localizacion)

    ; (caminoDestruible ?l1 ?l2)
    ;   Cierto si el camino entre ?l1 y ?l2 se destruye al transitarlo.
    (caminoDestruible ?l1 - Localizacion ?l2 - Localizacion)

    ; (trabajando ?p ?l ?r)
    ;   Cierto si ?p está extrayendo el recurso ?r en la localización ?l.
    (trabajando ?p - Personaje ?l - Localizacion ?r - TipoRecurso)

    ; (disponible ?p)
    ;   Cierto si el personaje no está asignado a ninguna tarea.
    (disponible ?p - Personaje)

    ; (puedeExtraer ?tp ?tr)
    ;   Cierto si los personajes de tipo ?tp pueden extraer el recurso ?tr.
    (puedeExtraer ?tp - TipoPersonaje ?tr - TipoRecurso)

    ; (esDeTipo ?p ?tp)
    ;   Relaciona una instancia de personaje con su tipo/raza.
    (esDeTipo ?p - Personaje ?tp - TipoPersonaje)
  )

  ; Funciones Numéricas
  ;
  ; (costeCamino ?l1 ?l2)
  ;   Almacena el coste de desplazarse de ?l1 a ?l2.
  ;   Se inicializa en el fichero de problema para cada par de localizaciones.
  ;   Los desplazamientos no indicados explícitamente tienen coste 1 (unitario).
  ;
  ; (total-cost)
  ;   Acumulador estándar de PDDL para el coste total del plan.
  ;   El planificador minimizará este valor con el alias seq-opt-lmcut.
  (:functions
    (costeCamino ?l1 - Localizacion ?l2 - Localizacion)
    (total-cost)
  )

  ; ACCIÓN: Viajar
  ;
  ; Mueve un personaje desde una localización origen a una destino.
  ;
  ; CAMBIO respecto al Ejercicio 1:
  ;   El efecto ahora incrementa (total-cost) en (costeCamino ?org ?dst)
  ;   en lugar de tener coste unitario implícito. De este modo, los
  ;   desplazamientos largos (p.ej. Fangorn -> AmonHen, coste 8) pesan
  ;   más que los cortos, y el planificador buscará la ruta de menor coste
  ;   total en lugar de la de menor número de pasos.
  ;
  ; Parámetros:
  ;   ?p   - Personaje que viaja
  ;   ?org - Localización de origen
  ;   ?dst - Localización de destino
  ;
  ; Precondiciones (idénticas al Ejercicio 1):
  ;   - El personaje está en el origen.
  ;   - Existe un camino entre origen y destino.
  ;   - El personaje está disponible.
  ;
  ; Efectos:
  ;   - El personaje pasa al destino y sale del origen.
  ;   - Se destruye el camino si procede.
  ;   - Se acumula el coste del desplazamiento en total-cost.
  ;
  (:action Viajar
    :parameters (?p - Personaje ?org - Localizacion ?dst - Localizacion)
    :precondition
      (and
        (estaEn ?p ?org)           ; el personaje está en el origen
        (camino ?org ?dst)         ; existe camino entre origen y destino
        (disponible ?p)            ; el personaje no está ocupado
      )
    :effect
      (and
        (estaEn ?p ?dst)           ; ahora está en el destino
        (not (estaEn ?p ?org))     ; ya no está en el origen
        ; Destruye el camino Rivendell<->Moria al atravesarlo (ambas dir.)
        (when (caminoDestruible ?org ?dst)
          (and
            (not (camino ?org ?dst))
            (not (camino ?dst ?org))
          )
        )
        ; acumular coste no unitario del desplazamiento
        (increase (total-cost) (costeCamino ?org ?dst))
      )
  )

  ; ACCIÓN: ExtraerRecurso
  ; 
  ; Asigna un personaje a un nodo de recurso, poniéndolo a trabajar.
  ; Una vez asignado, el personaje deja de estar disponible.
  ;
  ; Parámetros:
  ;   ?p  - Personaje que extrae
  ;   ?l  - Localización donde se encuentra el nodo de recurso
  ;   ?r  - Tipo de recurso a extraer
  ;
  ; Precondiciones:
  ;   - El personaje está en la localización del recurso.
  ;   - El personaje está disponible (no trabaja ya en otra tarea).
  ;   - Existe un nodo del tipo de recurso ?r en la localización ?l.
  ;   - El tipo de personaje tiene capacidad para extraer ese recurso.
  ;
  ; Efectos:
  ;   - El personaje queda registrado como trabajando en ?l extrayendo ?r.
  ;   - El personaje deja de estar disponible.
  ;
  (:action ExtraerRecurso
    :parameters (?p - Personaje ?l - Localizacion ?r - TipoRecurso)
    :precondition
      (and
        (estaEn ?p ?l)             ; el personaje está en la localización
        (disponible ?p)            ; el personaje está libre
        ; existe un nodo del tipo de recurso ?r en la localización ?l
        (exists (?n - NodoRecurso)
          (and
            (nodoEn ?n ?l)         ; el nodo está en ?l
            (tipoNodo ?n ?r)       ; el nodo produce el recurso ?r
          )
        )
        ; el tipo de personaje puede extraer ese recurso
        (exists (?tp - TipoPersonaje)
          (and
            (esDeTipo ?p ?tp)      ; ?p es de tipo ?tp
            (puedeExtraer ?tp ?r)  ; ?tp puede extraer ?r
          )
        )
      )
    :effect
      (and
        (trabajando ?p ?l ?r)      ; el personaje queda trabajando
        (not (disponible ?p))      ; ya no está disponible
      )
  )
)
