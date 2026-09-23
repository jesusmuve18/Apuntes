(define (problem ejercicio1)
  (:domain tierra-media)

  ; Objetos
  (:objects

    ; Localizaciones del mapa
    Hobbiton Bree Rivendell HighPass Mirkwood Erebor
    Moria Lothlorien Tharbad Fangorn Isengard
    HelmsDeep Edoras AmonHen MinasTirith DolAmroth
    Tolfolas MinasMorgul DeadMarshes Orodruin
      - Localizacion

    ; Personajes
    Enano1 Enano2 - Personaje
    Hobbit1       - Personaje

    ; Nodos de recursos
    ; Cada nodo representa un yacimiento concreto en una localización.
    ; Se necesitan nodos separados porque una localización puede tener
    ; más de un tipo de recurso (ej. Moria tiene Mineral Y Mithril).
    NodoMineral_Moria, NodoMithril_Moria      - NodoRecurso  ; Moria -> Mineral, Mitrhil
    NodoMineral_Erebor                        - NodoRecurso  ; Erebor -> Mineral
    NodoMadera_Fangorn                        - NodoRecurso  ; Fangorn -> Madera
    NodoMadera_Lothlorien                     - NodoRecurso  ; Lothlorien -> Madera
    NodoMadera_Mirkwood                       - NodoRecurso  ; Mirkwood -> Madera
    NodoAlimento_Hobbiton                     - NodoRecurso  ; Hobbiton -> Alimento
    NodoEspecia_Tolfolas                      - NodoRecurso  ; Tolfolas -> Especia
  )

  ; Estado inicial
  (:init
    ; Posiciones iniciales de los personajes
    (estaEn Enano1  Tharbad)    ; Enano1 visita a su primo en Tharbad
    (estaEn Enano2  Isengard)   ; Enano2 en misión diplomática en Isengard
    (estaEn Hobbit1 Lothlorien) ; Hobbit1 se encuentra en Lothlorien

    ; Disponibilidad de los personajes
    (disponible Enano1)
    (disponible Hobbit1)

    ; Tipos de personaje
    (esDeTipo Enano1  Enano)
    (esDeTipo Enano2  Enano)
    (esDeTipo Hobbit1 Hobbit)

    ; Capacidades de extracción por tipo de personaje
    (puedeExtraer Enano  Mineral)
    (puedeExtraer Enano  Mithril)
    (puedeExtraer Enano  Madera)
    (puedeExtraer Hobbit Alimento)

    ; Ubicación de los nodos de recursos
    (nodoEn NodoMineral_Moria     Moria)
    (nodoEn NodoMithril_Moria     Moria)
    (nodoEn NodoMineral_Erebor    Erebor)
    (nodoEn NodoMadera_Fangorn    Fangorn)
    (nodoEn NodoMadera_Lothlorien Lothlorien)
    (nodoEn NodoMadera_Mirkwood   Mirkwood)
    (nodoEn NodoAlimento_Hobbiton Hobbiton)
    (nodoEn NodoEspecia_Tolfolas  Tolfolas)

    ; Tipo de recurso de cada nodo
    (tipoNodo NodoMineral_Moria     Mineral)
    (tipoNodo NodoMithril_Moria     Mithril)
    (tipoNodo NodoMineral_Erebor    Mineral)
    (tipoNodo NodoMadera_Fangorn    Madera)
    (tipoNodo NodoMadera_Lothlorien Madera)
    (tipoNodo NodoMadera_Mirkwood   Madera)
    (tipoNodo NodoAlimento_Hobbiton Alimento)
    (tipoNodo NodoEspecia_Tolfolas  Especia)

    ; Mapa de caminos
    ; Hobbiton <-> Bree
    (camino Hobbiton Bree)
    (camino Bree Hobbiton)

    ; Bree <-> Rivendell
    (camino Bree Rivendell)
    (camino Rivendell Bree)

    ; Rivendell <-> HighPass
    (camino Rivendell HighPass)
    (camino HighPass Rivendell)

    ; HighPass <-> Mirkwood
    (camino HighPass Mirkwood)
    (camino Mirkwood HighPass)

    ; Mirkwood <-> Erebor
    (camino Mirkwood Erebor)
    (camino Erebor Mirkwood)

    ; Hobbiton <-> Tharbad
    (camino Hobbiton Tharbad)
    (camino Tharbad Hobbiton)

    ; Bree <-> Tharbad
    (camino Bree Tharbad)
    (camino Tharbad Bree)

    ; Tharbad <-> Helm's Deep
    (camino Tharbad HelmsDeep)
    (camino HelmsDeep Tharbad)

    ; Helm's Deep <-> Isengard
    (camino HelmsDeep Isengard)
    (camino Isengard HelmsDeep)

    ; Isengard <-> Fangorn
    (camino Isengard Fangorn)
    (camino Fangorn Isengard)

    ; Fangorn <-> Amon Hen
    (camino Fangorn AmonHen)
    (camino AmonHen Fangorn)

    ; Rivendell <-> Moria  (camino que se destruye)
    (camino Rivendell Moria)
    (camino Moria Rivendell)
    (caminoDestruible Rivendell Moria)
    (caminoDestruible Moria Rivendell)

    ; Moria <-> Lothlorien
    (camino Moria Lothlorien)
    (camino Lothlorien Moria)

    ; Lothlorien <-> Amon Hen
    (camino Lothlorien AmonHen)
    (camino AmonHen Lothlorien)

    ; Helm's Deep <-> Edoras
    (camino HelmsDeep Edoras)
    (camino Edoras HelmsDeep)

    ; Edoras <-> Dol Amroth
    (camino Edoras DolAmroth)
    (camino DolAmroth Edoras)

    ; Dol Amroth <-> Tolfolas
    (camino DolAmroth Tolfolas)
    (camino Tolfolas DolAmroth)

    ; Tolfolas <-> Minas Tirith
    (camino Tolfolas MinasTirith)
    (camino MinasTirith Tolfolas)

    ; Edoras <-> Minas Tirith
    (camino Edoras MinasTirith)
    (camino MinasTirith Edoras)

    ; AmonHen <-> DeadMarshes
    (camino AmonHen DeadMarshes)
    (camino DeadMarshes AmonHen)

    ; DeadMarshes <-> MinasMorgul
    (camino DeadMarshes MinasMorgul)
    (camino MinasMorgul DeadMarshes)

    ; MinasMorgul <-> Orodruin
    (camino MinasMorgul Orodruin)
    (camino Orodruin MinasMorgul)

    ; MinasMorgul <-> MinasTirith
    (camino MinasMorgul MinasTirith)
    (camino MinasTirith MinasMorgul)
  )

  ; Objetivo
  ; Que haya un personaje asignado a un nodo de Mithril y otro personaje asignado a un nodo de Alimento.
  (:goal
    (and
      (trabajando Enano1 Moria Mithril)
      (trabajando Hobbit1 Hobbiton Alimento)
    )
  )
)
