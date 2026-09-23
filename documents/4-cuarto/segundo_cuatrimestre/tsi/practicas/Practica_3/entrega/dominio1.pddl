(define (domain tierra-media)

  ; Requisitos
  ; :typing       -> permite usar tipos en parámetros y predicados
  (:requirements :typing)

  ; Tipos
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
    Recurso       - object  ; 
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
  ;   ?p - Personaje  |  ?l - Localizacion
  ;   Cierto si el personaje ?p se encuentra actualmente en la localización ?l.
  (:predicates
    (estaEn ?p - Personaje ?l - Localizacion)

    ; (nodoEn ?n ?l)
    ;   ?n - NodoRecurso  |  ?l - Localizacion
    ;   Cierto si el nodo de recurso ?n está ubicado en la localización ?l.
    (nodoEn ?n - NodoRecurso ?l - Localizacion)

    ; (tipoNodo ?n ?r)
    ;   ?n - NodoRecurso  |  ?r - TipoRecurso
    ;   Relaciona un nodo con el tipo de recurso que produce (ej. Moria -> Mithril).
    (tipoNodo ?n - NodoRecurso ?r - TipoRecurso)

    ; (camino ?l1 ?l2)
    ;   ?l1 ?l2 - Localizacion
    ;   Cierto si existe un camino (en cualquier dirección) entre ?l1 y ?l2.
    (camino ?l1 - Localizacion ?l2 - Localizacion)

    ; (caminoDestruible ?l1 ?l2)
    ;   ?l1 ?l2 - Localizacion
    ;   Cierto si el camino entre ?l1 y ?l2 se destruye al pasar por él.
    (caminoDestruible ?l1 - Localizacion ?l2 - Localizacion)

    ; (trabajando ?p ?l ?r)
    ;   ?p - Personaje  |  ?l - Localizacion  |  ?r - TipoRecurso
    ;   Cierto si el personaje ?p está extrayendo el recurso ?r en ?l.
    (trabajando ?p - Personaje ?l - Localizacion ?r - TipoRecurso)

    ; (disponible ?p)
    ;   ?p - Personaje
    ;   Cierto si el personaje no está actualmente asignado a ninguna tarea de extracción. 
    (disponible ?p - Personaje)

    ; (puedeExtraer ?tp ?tr)
    ;   ?tp - TipoPersonaje  |  ?tr - TipoRecurso
    ;   Cierto si los personajes de tipo ?tp tienen capacidad para extraer el recurso de tipo ?tr. 
    (puedeExtraer ?tp - TipoPersonaje ?tr - TipoRecurso)

    ; (esDeTipo ?p ?tp)
    ;   ?p - Personaje  |  ?tp - TipoPersonaje
    ;   Relaciona una instancia concreta de personaje con su raza.
    (esDeTipo ?p - Personaje ?tp - TipoPersonaje)
  )

  ; ACCIÓN: Viajar
  ;
  ; Mueve un personaje desde una localización origen a una destino.
  ;
  ; Parámetros:
  ;   ?p   - Personaje que viaja
  ;   ?org - Localización de origen
  ;   ?dst - Localización de destino
  ;
  ; Precondiciones:
  ;   - El personaje está en el origen.
  ;   - Existe un camino entre origen y destino.
  ;   - El personaje está disponible (no está trabajando en ninguna tarea).
  ;
  ; Efectos:
  ;   - El personaje está ahora en el destino.
  ;   - El personaje ya no está en el origen.
  ;   - Se elimina el camino si procede
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
        ; Destrucción del camino si procede (ambas direcciones).
        (when (caminoDestruible ?org ?dst)
          (and 
            (not (camino ?org ?dst)) 
            (not (camino ?dst ?org))
          )
        )
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
