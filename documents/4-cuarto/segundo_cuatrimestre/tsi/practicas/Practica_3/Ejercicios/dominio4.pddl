(define (domain tierra-media)

  ; Requisitos
  (:requirements 
    :typing                     ; permite usar tipos en parámetros y predicados
    :equality                   ; permite comparar objetos con =
    :conditional-effects        ; permite usar 'when' en efectos
    :existential-preconditions  ; permite usar 'exists' en precondiciones
    :universal-preconditions    ; permite usar 'forall' en precondiciones
  )

  ; Tipos
  ; Jerarquía:
  ;   object
  ;     - Localizacion
  ;     - Recurso
  ;           - TipoRecurso
  ;           - NodoRecurso
  ;     - Personaje
  ;     - TipoPersonaje
  ;     - Objeto
  ;     - TipoEdificio 
  ;
  (:types
    Localizacion  - object  ; lugares del mapa (Hobbiton, Bree, Rivendell, etc.)
    Recurso       - object
    TipoRecurso   - Recurso ; tipo de recurso extraíble (Mineral, Mithril, Madera, etc.)
    NodoRecurso   - Recurso ; nodo/yacimiento de un recurso en una localización concreta
    Personaje     - object  ; instancias concretas de personajes (Enano1, Hobbit1, etc.)
    TipoPersonaje - object  ; categoría de personaje (Enano, Hobbit, Mago, Elfo, etc.)
    Objeto        - object  ; ítems físicos portables (Anillo, ChalecoMithril, Espada)
    TipoEdificio  - object  ; categoría de edificio construible (TorreHechiceria, Extractor)
  )

  ; Constantes
  ; - TipoPersonaje: razas definidas
  ; - TipoRecurso:   recursos extraíbles del mapa
  ; - Objeto:        los tres ítems que la Comunidad debe reunir para destruir el Anillo
  ; - TipoEdificio): los dos tipos de edificio que se pueden construir
  (:constants
    Enano Hobbit Mago Elfo Orco Humano Corsario  - TipoPersonaje
    Mineral Mithril Madera Especia Alimento      - TipoRecurso
    Anillo ChalecoMithril Espada                 - Objeto
    TorreHechiceria Extractor                    - TipoEdificio
  )

  ; Predicados
  (:predicates

    ; (estaEn ?p ?l)
    ;   ?p - Personaje  |  ?l - Localizacion
    ;   Cierto si el personaje ?p se encuentra actualmente en la localización ?l.
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
    ;   Un personaje que trabaja deja de estar disponible.
    (trabajando ?p - Personaje ?l - Localizacion ?r - TipoRecurso)

    ; (disponible ?p)
    ;   ?p - Personaje
    ;   Cierto si el personaje no está actualmente asignado a ninguna tarea de extracción.
    ;   Un personaje no disponible no puede viajar ni extraer recursos.
    (disponible ?p - Personaje)

    ; (puedeExtraer ?tp ?tr)
    ;   ?tp - TipoPersonaje  |  ?tr - TipoRecurso
    ;   Cierto si los personajes de tipo ?tp tienen capacidad para extraer el recurso ?tr.
    (puedeExtraer ?tp - TipoPersonaje ?tr - TipoRecurso)

    ; (esDeTipo ?p ?tp)
    ;   ?p - Personaje  |  ?tp - TipoPersonaje
    ;   Relaciona una instancia concreta de personaje con su raza.
    (esDeTipo ?p - Personaje ?tp - TipoPersonaje)

    ; (comunidadFormada)
    ;   Sin parámetros. Se activa cuando se ejecuta la acción formarComunidad.
    ;   A partir de ese momento los miembros viajan juntos (viajarComunidad).
    (comunidadFormada)

    ; (esMiembro ?p)
    ;   ?p - Personaje
    ;   Cierto si el personaje pertenece a la Comunidad del Anillo.
    ;   Se establece al ejecutar formarComunidad sobre ese personaje.
    (esMiembro ?p - Personaje)

    ; (tieneObjeto ?p ?o)
    ;   ?p - Personaje  |  ?o - Objeto
    ;   Cierto si el personaje ?p porta actualmente el objeto ?o.
    (tieneObjeto ?p - Personaje ?o - Objeto)

    ; (objetoEn ?o ?l)
    ;   ?o - Objeto  |  ?l - Localizacion
    ;   Cierto si el objeto ?o está en la localización ?l (aún sin recoger).
    (objetoEn ?o - Objeto ?l - Localizacion)

    ; (portadorAnillo ?p)
    ;   ?p - Personaje
    ;   Cierto si ?p es el portador del Anillo Único.
    (portadorAnillo ?p - Personaje)

    ; (anilloDestruido)
    ;   Sin parámetros.
    ;   Cierto si se ha destruido el Anillo Único.
    (anilloDestruido)

    ; (lugarDestruccion ?l)
    ;   ?l - Localizacion
    ;   Marca dónde debe destruirse el Anillo.
    (lugarDestruccion ?l - Localizacion)

    ; (chalecoMaterializado)
    ;   Sin parámetros.
    ;   Se activa cuando un Mago ejecuta materializarChaleco.
    ;   Evita que el chaleco se materialice más de una vez.
    (chalecoMaterializado)

    ; (comunidadEstaEn ?l)
    ;   ?l - Localizacion
    ;   Marca dónde se encuentra la comunidad como unidad.
    (comunidadEstaEn ?l - Localizacion)

    ; (hayNodoTipo ?r ?l)
    ;   PREDICADO AUXILIAR DE RENDIMIENTO.
    ;   Cierto si existe al menos un nodo del tipo de recurso ?r en la localización ?l.
    ;   Se precalcula en el fichero de problema para evitar exists en precondiciones,
    ;   lo que reduce el coste de traducción de SAS+ y acelera la búsqueda.
    (hayNodoTipo ?r - TipoRecurso ?l - Localizacion)

    ; (puedeExtraerPersonaje ?p ?r)
    ;   PREDICADO AUXILIAR DE RENDIMIENTO.
    ;   Precalculado en el problema. Verdadero si la raza de ?p
    ;   puede extraer ?r. Evita el exists sobre TipoPersonaje
    ;   en ExtraerRecurso, que es la acción más instanciada.
    (puedeExtraerPersonaje ?p - Personaje ?r - TipoRecurso)


    ; (edificioEn ?e ?l)
    ;   ?e - TipoEdificio  |  ?l - Localizacion
    ;   Cierto si el edificio de tipo ?e ha sido construido en la localización ?l.
    (edificioEn ?e - TipoEdificio ?l - Localizacion)

    ; (recursoNecesario ?e ?r)
    ;   ?e - TipoEdificio  |  ?r - TipoRecurso
    ;   Cierto si para construir el edificio de tipo ?e se necesita el recurso ?r.
    (recursoNecesario ?e - TipoEdificio ?r - TipoRecurso)

    ; (urukHaiCreado)
    ;   Sin parámetros.
    ;   Cierto si se ha creado al menos un Uruk-Hai.
    (urukHaiCreado)

    ; (tieneEspecia ?p)
    ;   ?p - Personaje
    ;   Cierto si el corsario ?p porta físicamente Especia consigo.
    (tieneEspecia ?p - Personaje)

    ; (especiaEn ?l)
    ;   ?l - Localizacion
    ;   Cierto si hay Especia disponible en la localización ?l.
    (especiaEn ?l - Localizacion)

    ; (lugarCreacionUruk ?l)
    ;   ?l - Localizacion
    ;   Marca dónde debe crearse el Uruk-Hai.
    (lugarCreacionUruk ?l - Localizacion)
  )

  ; ACCIÓN: Viajar  (modificada II)
  ;
  ; Mueve un personaje individual entre dos localizaciones.
  ; Restricción: si la Comunidad está formada, solo pueden usar esta acción los
  ; personajes que NO pertenezcan a ella. Los miembros usan viajarComunidad.
  ;
  ; Cuando un Corsario que porta Especia llega a una
  ; localización, se activa el predicado (especiaEn ?dst) para que CrearUrukHai
  ; pueda comprobar la presencia de Especia sin necesidad de una acción de depósito.
  ;
  ; Parámetros:
  ;   ?p   - Personaje que viaja
  ;   ?org - Localización de origen
  ;   ?dst - Localización de destino
  ;
  ; Precondiciones:
  ;   - El personaje está en el origen.
  ;   - Existe un camino entre origen y destino.
  ;   - El personaje está disponible (no está trabajando).
  ;   - O bien la comunidad no está formada, o bien el personaje no es miembro.
  ;
  ; Efectos:
  ;   - El personaje está ahora en el destino.
  ;   - El personaje ya no está en el origen.
  ;   - Se destruye el camino Rivendell<->Moria si se atraviesa.
  ;   - Si porta Especia, se actualiza (especiaEn) en origen y destino.
  ;
  (:action Viajar
    :parameters (?p - Personaje ?org - Localizacion ?dst - Localizacion)
    :precondition
      (and
        (estaEn ?p ?org)           ; el personaje está en el origen
        (camino ?org ?dst)         ; existe camino entre origen y destino
        (disponible ?p)            ; el personaje no está ocupado
        ; si la comunidad está formada el personaje no puede ser miembro de ella
        (not (and (comunidadFormada) (esMiembro ?p)))
      )
    :effect
      (and
        (estaEn ?p ?dst)           ; ahora está en el destino
        (not (estaEn ?p ?org))     ; ya no está en el origen
        ; Destrucción del camino si procede
        (when (caminoDestruible ?org ?dst)
          (and
            (not (camino ?org ?dst))
            (not (camino ?dst ?org))
          )
        )
        ; si el personaje porta Especia, la lleva consigo al destino.
        (when (tieneEspecia ?p)
          (and
            (especiaEn ?dst)
            (not (especiaEn ?org))
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
  ;   - El personaje está disponible.
  ;   - Existe un nodo del tipo de recurso ?r en la localización ?l.
  ;   - El tipo de personaje tiene capacidad para extraer ese recurso.
  ;   - Si el recurso a extraer es Mineral:
  ;       - Debe haber un extractor en esa localización.
  ;
  ; Efectos:
  ;   - El personaje queda registrado como trabajando en ?l extrayendo ?r.
  ;   - El personaje deja de estar disponible.
  ;
  (:action ExtraerRecurso
    :parameters (?p - Personaje ?l - Localizacion ?r - TipoRecurso)
    :precondition
      (and
        (estaEn ?p ?l)                ; el personaje está en la localización
        (disponible ?p)               ; el personaje está libre
        ; existe un nodo del tipo de recurso ?r en la localización ?l
        ; Verificación mediante predicado auxiliar (más eficiente que exists)
        (hayNodoTipo ?r ?l)           ; optimizado
        ; el tipo de personaje puede extraer ese recurso
        (puedeExtraerPersonaje ?p ?r) ; optimizado
        (or 
          (not (= ?r Mineral))        ; si es de tipo mineral 
          (edificioEn Extractor ?l)   ; debe haber un extractor en esa localización
        )
      )
    :effect
      (and
        (trabajando ?p ?l ?r)      ; el personaje queda trabajando
        (not (disponible ?p))      ; ya no está disponible
      )
  )

  ; ACCIÓN: formarComunidad  (1 Hobbit + 1 Mago)
  ;
  ; Reúne a los 2 personajes que constituirán la Comunidad del Anillo
  ; (un Hobbit y un Mago) en una misma localización.
  ;
  ; Parámetros:
  ;   ?p1  - Primer miembro  (Hobbit)
  ;   ?p2  - Segundo miembro (Mago)
  ;   ?l   - Localización donde se forma la Comunidad
  ;
  ; Precondiciones:
  ;   - La comunidad no está aún formada.
  ;   - Ambos personajes están disponibles.
  ;   - Ambos personajes están en la misma localización ?l.
  ;   - ?p1 es un Hobbit y ?p2 es un Mago.
  ;
  ; Efectos:
  ;   - La comunidad queda marcada como formada.
  ;   - Ambos personajes pasan a ser miembros de la comunidad.
  ;   - Se olvida la posición individual (ya se mueve en conjunto).
  ;   - Se inicializa la posición de la comunidad.
  ;
  (:action formarComunidad
    :parameters (?p1 - Personaje ?p2 - Personaje ?l - Localizacion)
    :precondition
      (and
        (not (comunidadFormada))   ; la comunidad no existe todavía
        (disponible ?p1)           ; ?p1 está libre
        (disponible ?p2)           ; ?p2 está libre
        (estaEn ?p1 ?l)            ; ambos están en la misma localización
        (estaEn ?p2 ?l)
        (esDeTipo ?p1 Hobbit)      ; ?p1 debe ser Hobbit
        (esDeTipo ?p2 Mago)        ; ?p2 debe ser Mago
      )
    :effect
      (and
        (comunidadFormada)         ; la comunidad queda constituida
        (esMiembro ?p1)            ; ?p1 es miembro
        (esMiembro ?p2)            ; ?p2 es miembro
        (not (estaEn ?p1 ?l))      ; se olvida la posición individual de cada miembro
        (not (estaEn ?p2 ?l))
        (comunidadEstaEn ?l)       ; se inicializa la posición de la comunidad
      )
  )

  ; ACCIÓN: viajarComunidad  (2 miembros: 1 Hobbit + 1 Mago)
  ;
  ; Mueve a los 2 miembros de la Comunidad conjuntamente de una localización a otra.
  ; En cuanto la Comunidad se forma, todos sus miembros deben desplazarse juntos;
  ; ninguno puede moverse individualmente (restricción reflejada en la acción Viajar).
  ;
  ; Parámetros (orden exigido: primero los personajes, luego origen y destino):
  ;   ?p1  - Primer miembro de la Comunidad (Hobbit)
  ;   ?p2  - Segundo miembro de la Comunidad (Mago)
  ;   ?org - Localización de origen
  ;   ?dst - Localización de destino
  ;
  ; Precondiciones:
  ;   - La comunidad está formada.
  ;   - Ambos personajes son miembros.
  ;   - Ambos están disponibles (no trabajando).
  ;   - La comunidad está en la localización origen.
  ;   - Existe camino entre origen y destino.
  ;   - ?p1 es Hobbit y ?p2 es Mago (restricción para reducir el espacio de búsqueda).
  ;
  ; Efectos:
  ;   - La comunidad se mueve al destino.
  ;   - Se destruye el camino si procede.
  ;
  (:action viajarComunidad
    :parameters (?p1 - Personaje ?p2 - Personaje ?org - Localizacion ?dst - Localizacion)
    :precondition
      (and
        (comunidadFormada)         ; la comunidad debe existir
        (esMiembro ?p1)            ; ?p1 pertenece a la comunidad
        (esMiembro ?p2)            ; ?p2 pertenece a la comunidad
        (disponible ?p1)           ; ?p1 no está trabajando
        (disponible ?p2)           ; ?p2 no está trabajando
        (comunidadEstaEn ?org)     ; la Comunidad está en el origen
        (camino ?org ?dst)         ; existe camino entre origen y destino
        (esDeTipo ?p1 Hobbit)      ; ayuda al grounding (optimización)
        (esDeTipo ?p2 Mago)
      )
    :effect
      (and
        (comunidadEstaEn ?dst)          ; la comunidad se mueve al destino
        (not (comunidadEstaEn ?org))    ; la comunidad ya no está en el origen
        ; Destrucción del camino si procede
        (when (caminoDestruible ?org ?dst)
          (and
            (not (camino ?org ?dst))
            (not (camino ?dst ?org))
          )
        )
      )
  )

  ; ACCIÓN: materializarChaleco
  ;
  ; Un Mago miembro de la Comunidad crea el Chaleco de Mithril en una localización
  ; donde se esté extrayendo Mithril activamente.
  ;
  ; Parámetros (en el orden exigido por el enunciado):
  ;   ?mago - Personaje de tipo Mago que materializa el chaleco
  ;   ?l    - Localización donde se materializa (debe haber extracción de Mithril activa)
  ;
  ; Precondiciones:
  ;   - La comunidad está formada.
  ;   - ?mago es de tipo Mago y miembro de la Comunidad.
  ;   - ?mago está disponible.
  ;   - La comunidad está en la localización ?l.
  ;   - Hay algún personaje extrayendo Mithril en ?l.
  ;   - El chaleco no ha sido materializado todavía.
  ;
  ; Efectos:
  ;   - El ChalecoMithril aparece en la localización ?l (listo para recoger).
  ;   - Se marca que el chaleco ya ha sido materializado.
  ;
  (:action materializarChaleco
    :parameters (?mago - Personaje ?l - Localizacion)
    :precondition
      (and
        (comunidadFormada)                    ; la comunidad debe existir
        (esDeTipo ?mago Mago)                 ; debe ser un Mago
        (esMiembro ?mago)                     ; debe pertenecer a la Comunidad
        (disponible ?mago)                    ; el Mago no está trabajando
        (comunidadEstaEn ?l)                  ; la Comunidad está en la localización
        (not (chalecoMaterializado))          ; solo se puede materializar una vez
        ; Debe haber alguien extrayendo Mithril en ?l
        (exists (?p - Personaje)
          (trabajando ?p ?l Mithril)
        )
      )
    :effect
      (and
        (objetoEn ChalecoMithril ?l)          ; el chaleco aparece en ?l
        (chalecoMaterializado)                ; marcamos que ya está materializado
      )
  )

  ; ACCIÓN: recogerObjeto
  ;
  ; Un personaje recoge un objeto que se encuentra en su localización.
  ; Los objetos que se pueden recoger son: Anillo, ChalecoMithril, Espada.
  ;
  ; Orden de recogida:
  ;   1. Primero se debe recoger el Anillo (en Rivendell).
  ;   2. Después (en cualquier orden) el ChalecoMithril y la Espada.
  ;   El mismo personaje que recoge el Anillo debe recoger también
  ;   el ChalecoMithril y la Espada (se convierte en portadorAnillo).
  ;
  ; Parámetros:
  ;   ?p - Personaje que recoge el objeto
  ;   ?l - Localización donde se recoge
  ;   ?o - Objeto a recoger
  ;
  ; Precondiciones:
  ;   - La comunidad está formada.
  ;   - ?p es miembro de la Comunidad y está disponible.
  ;   - La Comunidad está en ?l.
  ;   - El objeto ?o está en ?l.
  ;   - Si recoge el Anillo: ?p debe ser Hobbit.
  ;   - Si recoge ChalecoMithril o Espada: ?p ya debe ser portadorAnillo.
  ;
  ; Efectos:
  ;   - ?p pasa a tener el objeto ?o.
  ;   - El objeto deja de estar en la localización.
  ;   - Si el objeto es el Anillo, ?p se convierte en portadorAnillo.
  ;
  (:action recogerObjeto
    :parameters (?p - Personaje ?l - Localizacion ?o - Objeto)
    :precondition
      (and
        (comunidadFormada)                ; la comunidad debe estar formada
        (esMiembro ?p)                    ; ?p debe ser miembro
        (disponible ?p)                   ; ?p no está trabajando
        (comunidadEstaEn ?l)              ; la Comunidad está en la localización
        (objetoEn ?o ?l)                  ; el objeto está en esa localización
        ; Si es el Anillo: solo lo puede recoger un Hobbit
        (or (not (= ?o Anillo)) (esDeTipo ?p Hobbit))
        ; Si es ChalecoMithril o Espada: ?p debe ser ya el portador del Anillo
        (or (= ?o Anillo) (portadorAnillo ?p))
      )
    :effect
      (and
        (tieneObjeto ?p ?o)               ; ?p ahora porta el objeto
        (not (objetoEn ?o ?l))            ; el objeto ya no está en el suelo
        ; Si el objeto es el Anillo, ?p se convierte en portador
        (when (= ?o Anillo)
          (portadorAnillo ?p)
        )
      )
  )

  ; ACCIÓN: destruirAnillo
  ;
  ; El portador del Anillo lo arroja al fuego en el lugar de destrucción (Orodruin).
  ; Debe llevar consigo los tres objetos (Anillo, ChalecoMithril, Espada).
  ;
  ; Parámetros:
  ;   ?p - Personaje que destruye el Anillo (debe ser el portador)
  ;   ?l - Localización donde se destruye (debe coincidir con lugarDestruccion)
  ;
  ; Precondiciones:
  ;   - ?p es el portador del Anillo y porta los tres objetos.
  ;   - La Comunidad está en la localización ?l.
  ;   - ?l es el lugarDestruccion definido en el problema.
  ;   - El Anillo no ha sido destruido todavía.
  ;
  ; Efectos:
  ;   - El Anillo queda destruido.
  ;   - ?p deja de ser portador y de tener el Anillo.
  ;
  (:action destruirAnillo
    :parameters (?p - Personaje ?l - Localizacion)
    :precondition
      (and
        (portadorAnillo ?p)               ; ?p lleva el Anillo
        (tieneObjeto ?p Anillo)           ; confirma que lo porta
        (tieneObjeto ?p ChalecoMithril)   ; también lleva el Chaleco
        (tieneObjeto ?p Espada)           ; también lleva la Espada
        (comunidadEstaEn ?l)              ; la comunidad está en la localización
        (lugarDestruccion ?l)             ; esa localización es la correcta
        (not (anilloDestruido))           ; el anillo aún no ha sido destruido
      )
    :effect
      (and
        (anilloDestruido)                 ; el anillo ha sido destruido
        (not (portadorAnillo ?p))         ; ?p ya no es portador
        (not (tieneObjeto ?p Anillo))     ; ya no porta el Anillo
      )
  )

  ; ACCIÓN: Construir
  ;
  ; Un personaje construye un edificio en una localización.
  ; Esta acción es genérica: sirve tanto para construir la TorreHechiceria
  ; (construida por un Mago externo a la Comunidad) como el Extractor
  ; (construido por un Humano).
  ;
  ; La acción verifica por sí misma si se dispone de los recursos necesarios
  ;
  ; Restricciones de personaje por tipo de edificio:
  ;   - TorreHechiceria: debe construirla un Mago que no sea miembro de la Comunidad.
  ;   - Extractor:       debe construirlo un Humano.
  ;
  ; Parámetros (en el orden exigido por el enunciado):
  ;   ?constructor - Personaje que construye el edificio
  ;   ?e           - Tipo de edificio a construir
  ;   ?l           - Localización donde se construye
  ;
  ; Precondiciones:
  ;   - ?constructor está en ?l y está disponible.
  ;   - No existe ya un edificio de tipo ?e en ?l (unicidad por localización).
  ;   - Si ?e es TorreHechiceria: 
  ;       - ?constructor es Mago y no es miembro de la Comunidad.
  ;   - Si ?e es Extractor: 
  ;       - ?constructor es Humano.
  ;   - Para todos los recursos necesarios para ?e, existe algún personaje
  ;     extrayendo ese recurso. En caso de ser la TorreHechicería, este no puede ser un enano.
  ;
  ; Efectos:
  ;   - El edificio ?e queda construido en ?l.
  ;   - El constructor vuelve a estar disponible.
  ;
  (:action Construir
    :parameters (?constructor - Personaje ?e - TipoEdificio ?l - Localizacion)
    :precondition
      (and
        (estaEn ?constructor ?l)              ; el constructor está en ?l
        (disponible ?constructor)             ; el constructor está libre
        (not (edificioEn ?e ?l))              ; no hay ya ese edificio en ?l
        (or
          (and 
            (= ?e TorreHechiceria)            ; si es TorreHechiceria 
            (esDeTipo ?constructor Mago)      ; el constructor debe ser un Mago
            (not (esMiembro ?constructor))    ; el constructor no es miembro de la comunidad
          )
          (and                  
            (= ?e Extractor)                  ; Si es Extractor
            (esDeTipo ?constructor Humano)    ; el constructor debe ser un Humano
          )
        )
        ; Para todos los recursos que necesita el edificio ?e,
        ; debe existir algún personaje extrayéndolo.
        (forall (?r - TipoRecurso)
          (imply
            (recursoNecesario ?e ?r)
            (exists (?trabajador - Personaje ?l2 - Localizacion)
              (and
                (trabajando ?trabajador ?l2 ?r)
                (or 
                  (not (= ?e TorreHechiceria))            ; si es TorreHechiceria 
                  (not (esDeTipo ?trabajador Enano))      ; el extractor no puede ser enano
                )
              )
            )
          )
        )
      )
    :effect
      (and
        (edificioEn ?e ?l)                    ; el edificio queda construido
        ; El constructor vuelve a estar disponible al terminar la construcción
        (disponible ?constructor)
      )
  )

  ; ACCIÓN: RecogerEspecia
  ;
  ; Un Corsario recoge la Especia que está siendo extraída en una localización.
  ; A diferencia de los demás recursos, la Especia debe ser transportada
  ; físicamente hasta su destino (Isengard) por el Corsario.
  ; El proceso completo es:
  ;   1. Corsario1 extrae Especia en Tolfolas (ExtraerRecurso).
  ;   2. Corsario2 la recoge en Tolfolas (RecogerEspecia) — queda como tieneEspecia.
  ;   3. Corsario2 viaja hasta Isengard llevando la Especia (Viajar actualiza especiaEn).
  ;
  ; Parámetros:
  ;   ?corsario - Personaje de tipo Corsario que recoge la Especia
  ;   ?l        - Localización donde se recoge (que es donde se extrae)
  ;
  ; Precondiciones:
  ;   - ?corsario está en ?l y está disponible.
  ;   - ?corsario es de tipo Corsario.
  ;   - Hay algún personaje extrayendo Especia en ?l.
  ;   - El corsario no porta ya Especia (evita recoger dos veces).
  ;
  ; Efectos:
  ;   - ?corsario pasa a portar Especia (tieneEspecia).
  ;   - Se activa (especiaEn ?l) para marcar disponibilidad local inmediata.
  ;
  (:action RecogerEspecia
    :parameters (?corsario - Personaje ?l - Localizacion)
    :precondition
      (and
        (estaEn ?corsario ?l)                 ; el corsario está en ?l
        (disponible ?corsario)                ; está disponible
        (esDeTipo ?corsario Corsario)         ; debe ser Corsario
        (not (tieneEspecia ?corsario))        ; no porta ya Especia
        ; Debe haber alguien extrayendo Especia en ?l
        (exists (?extractor - Personaje)
          (trabajando ?extractor ?l Especia)
        )
      )
    :effect
      (and
        (tieneEspecia ?corsario)              ; el corsario porta la Especia
        (especiaEn ?l)                        ; hay Especia disponible en ?l
      )
  )

  ; ACCIÓN: CrearUrukHai
  ;
  ; Un Mago (externo a la Comunidad) crea un Uruk-Hai en la localización
  ; donde se encuentra la TorreHechiceria y ha llegado la Especia.
  ;
  ; Parámetros:
  ;   ?p    - Personaje de tipo Mago (no perteneciente a la Comunidad)
  ;   ?l    - Localización donde se crea el Uruk-Hai
  ;
  ; Precondiciones:
  ;   - Debe existir una TorreHechiceria en ?l.
  ;   - Debe haber Especia en ?l (transportada físicamente por un Corsario).
  ;   - El Mago debe estar en ?l, disponible y NO ser miembro de la Comunidad.
  ;   - El Uruk-Hai no ha sido creado todavía.
  ;
  ; Efectos:
  ;   - Se marca que el Uruk-Hai ha sido creado.
  ;
  (:action CrearUrukHai
    :parameters (?p - Personaje ?l - Localizacion)
    :precondition
      (and
        (esDeTipo ?p Mago)                 ; debe ser un Mago
        (not (esMiembro ?p))               ; no puede pertenecer a la Comunidad
        (estaEn ?p ?l)                     ; el personaje está en ?l
        (disponible ?p)                    ; el personaje está disponible
        (edificioEn TorreHechiceria ?l)    ; la Torre está en ?l
        (especiaEn ?l)                     ; hay Especia en ?l
        (lugarCreacionUruk ?l)             ; ?l es el lugar designado para la creación
        (not (urukHaiCreado))              ; no se ha creado ya un Uruk-Hai
      )
    :effect
      (and
        (urukHaiCreado)                       ; el Uruk-Hai ha sido creado
      )
  )
)
