(define (problem ejercicio4)
  (:domain tierra-media)

  ; Objetos
  (:objects

    ; Localizaciones del mapa de la Tierra Media
    Hobbiton Bree Rivendell HighPass Mirkwood Erebor
    Moria Lothlorien Tharbad Fangorn Isengard
    HelmsDeep Edoras AmonHen MinasTirith DolAmroth
    Tolfolas MinasMorgul DeadMarshes Orodruin
      - Localizacion

    ; Personajes
    Hobbit1 Hobbit2 Hobbit3 - Personaje   ; en Hobbiton
    Hobbit4                 - Personaje   ; en Bree
    Mago1                   - Personaje   ; en Rivendell
    Mago2                   - Personaje   ; en Isengard
    Elfo1                   - Personaje   ; en Lothlorien
    Enano1                  - Personaje   ; en Fangorn, disponible
    Enano2                  - Personaje   ; en Erebor, disponible
    Humano1                 - Personaje   ; en Edoras, no disponible
    Humano2                 - Personaje   ; en Bree, disponible
    Orco1 Orco2 Orco3       - Personaje   ; en Moria, disponibles
    Corsario1 Corsario2     - Personaje   ; en Dol Amroth, disponibles

    ; Nodos de recursos
    NodoMineral_Moria, NodoMithril_Moria - NodoRecurso   ; Moria -> Mineral y Mithril
    NodoMineral_Erebor                   - NodoRecurso   ; Erebor -> Mineral
    NodoMadera_Fangorn                   - NodoRecurso   ; Fangorn -> Madera
    NodoMadera_Lothlorien                - NodoRecurso   ; Lothlorien -> Madera
    NodoMadera_Mirkwood                  - NodoRecurso   ; Mirkwood -> Madera
    NodoAlimento_Hobbiton                - NodoRecurso   ; Hobbiton -> Alimento
    NodoEspecia_Tolfolas                 - NodoRecurso   ; Tolfolas -> Especia
  )

  ; Estado inicial
  (:init

    ; Posiciones iniciales de los personajes
    (estaEn Hobbit1 Hobbiton)
    (estaEn Hobbit2 Hobbiton)
    (estaEn Hobbit3 Hobbiton)
    (estaEn Hobbit4 Bree)
    (estaEn Mago1   Rivendell)
    (estaEn Mago2   Isengard)
    (estaEn Elfo1   Lothlorien)
    (estaEn Enano1  Fangorn)
    (estaEn Enano2  Erebor)
    (estaEn Humano1 Edoras)
    (estaEn Humano2 Bree)
    (estaEn Orco1   Moria)
    (estaEn Orco2   Moria)
    (estaEn Orco3   Moria)
    (estaEn Corsario1 DolAmroth)
    (estaEn Corsario2 DolAmroth)

    ; Disponibilidad de los personajes
    (disponible Hobbit1)
    (disponible Hobbit2)
    (disponible Hobbit3)
    (disponible Hobbit4)
    (disponible Mago1)
    (disponible Mago2)
    (disponible Elfo1)
    (disponible Enano1)
    (disponible Enano2)
    ; Humano1 no está disponible
    (disponible Humano2)
    (disponible Orco1)
    (disponible Orco2)
    (disponible Orco3)
    (disponible Corsario1)
    (disponible Corsario2)

    ; Tipos de personaje
    (esDeTipo Hobbit1 Hobbit)
    (esDeTipo Hobbit2 Hobbit)
    (esDeTipo Hobbit3 Hobbit)
    (esDeTipo Hobbit4 Hobbit)
    (esDeTipo Mago1   Mago)
    (esDeTipo Mago2   Mago)
    (esDeTipo Elfo1   Elfo)
    (esDeTipo Enano1  Enano)
    (esDeTipo Enano2  Enano)
    (esDeTipo Humano1 Humano)
    (esDeTipo Humano2 Humano)
    (esDeTipo Orco1   Orco)
    (esDeTipo Orco2   Orco)
    (esDeTipo Orco3   Orco)
    (esDeTipo Corsario1 Corsario)
    (esDeTipo Corsario2 Corsario)

    ; Capacidades de extracción por tipo de personaje
    (puedeExtraer Enano   Mineral)
    (puedeExtraer Enano   Mithril)
    (puedeExtraer Enano   Madera)
    (puedeExtraer Hobbit  Alimento)
    (puedeExtraer Orco    Mineral)
    (puedeExtraer Orco    Madera)
    (puedeExtraer Humano  Madera)
    (puedeExtraer Corsario Especia)

    ; Despliegue explícito de todas las combinaciones posibles (para optimización)
    ; Enanos
    (puedeExtraerPersonaje Enano1 Mineral)
    (puedeExtraerPersonaje Enano1 Mithril)
    (puedeExtraerPersonaje Enano1 Madera)
    (puedeExtraerPersonaje Enano2 Mineral)
    (puedeExtraerPersonaje Enano2 Mithril)
    (puedeExtraerPersonaje Enano2 Madera)
    ; Hobbits
    (puedeExtraerPersonaje Hobbit1 Alimento)
    (puedeExtraerPersonaje Hobbit2 Alimento)
    (puedeExtraerPersonaje Hobbit3 Alimento)
    (puedeExtraerPersonaje Hobbit4 Alimento)
    ;Orcos
    (puedeExtraerPersonaje Orco1 Mineral)
    (puedeExtraerPersonaje Orco1 Madera)
    (puedeExtraerPersonaje Orco2 Mineral)
    (puedeExtraerPersonaje Orco2 Madera)
    (puedeExtraerPersonaje Orco3 Mineral)
    (puedeExtraerPersonaje Orco3 Madera)
    ; Humanos
    (puedeExtraerPersonaje Humano1 Madera)
    (puedeExtraerPersonaje Humano2 Madera)
    ; Corsarios
    (puedeExtraerPersonaje Corsario1 Especia)
    (puedeExtraerPersonaje Corsario2 Especia)

    ; Ubicación de los nodos de recursos
    (nodoEn NodoMineral_Moria      Moria)
    (nodoEn NodoMithril_Moria      Moria)
    (nodoEn NodoMineral_Erebor     Erebor)
    (nodoEn NodoMadera_Fangorn     Fangorn)
    (nodoEn NodoMadera_Lothlorien  Lothlorien)
    (nodoEn NodoMadera_Mirkwood    Mirkwood)
    (nodoEn NodoAlimento_Hobbiton  Hobbiton)
    (nodoEn NodoEspecia_Tolfolas   Tolfolas)

    ; Tipo de recurso de cada nodo
    (tipoNodo NodoMineral_Moria     Mineral)
    (tipoNodo NodoMithril_Moria     Mithril)
    (tipoNodo NodoMineral_Erebor    Mineral)
    (tipoNodo NodoMadera_Fangorn    Madera)
    (tipoNodo NodoMadera_Lothlorien Madera)
    (tipoNodo NodoMadera_Mirkwood   Madera)
    (tipoNodo NodoAlimento_Hobbiton Alimento)
    (tipoNodo NodoEspecia_Tolfolas  Especia)

    ; Predicado auxiliar hayNodoTipo: precalculado para cada par (recurso, loc.)
    ; Evita cuantificadores exists en las precondiciones de ExtraerRecurso,
    ; lo que reduce el coste de traducción y acelera la búsqueda del planificador.
    (hayNodoTipo Mithril  Moria)
    (hayNodoTipo Mineral  Moria)
    (hayNodoTipo Mineral  Erebor)
    (hayNodoTipo Madera   Fangorn)
    (hayNodoTipo Madera   Lothlorien)
    (hayNodoTipo Madera   Mirkwood)
    (hayNodoTipo Alimento Hobbiton)
    (hayNodoTipo Especia  Tolfolas)

    ; Ubicación de los objetos especiales
    (objetoEn Anillo  Rivendell)   ; el Anillo está en Rivendell
    (objetoEn Espada  Lothlorien)  ; la Espada está en Lothlorien

    ; Lugar de destrucción del anillo
    (lugarDestruccion Orodruin)    ; el Anillo debe destruirse en Orodruin

    ; Recursos necesarios para cada tipo de edificio
    (recursoNecesario TorreHechiceria Mineral)
    (recursoNecesario TorreHechiceria Madera)
    (recursoNecesario Extractor       Madera)

    ; Lugar de creación del Uruk-Hai
    (lugarCreacionUruk Isengard)

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

    ; Rivendell <-> Moria (camino que se destruye)
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

    ; Amon Hen <-> Dead Marshes
    (camino AmonHen DeadMarshes)
    (camino DeadMarshes AmonHen)

    ; Dead Marshes <-> Minas Morgul
    (camino DeadMarshes MinasMorgul)
    (camino MinasMorgul DeadMarshes)

    ; Minas Morgul <-> Orodruin
    (camino MinasMorgul Orodruin)
    (camino Orodruin MinasMorgul)

    ; Minas Morgul <-> Minas Tirith
    (camino MinasMorgul MinasTirith)
    (camino MinasTirith MinasMorgul)
  )

  ; Objetivo
  ;
  ; Se deben satisfacer simultáneamente:
  ;   - El Anillo Único ha sido destruido.
  ;   - Se ha creado un Uruk-Hai en la Torre de Hechicería de Isengard.
  ;   - Un Humano está en Bree.
  (:goal
    (and
      (anilloDestruido)                ; el Anillo ha sido destruido
      (urukHaiCreado)                  ; el Uruk-Hai ha sido creado
      (exists (?h - Personaje)         ; algún Humano en Bree
        (and
          (esDeTipo ?h Humano)
          (estaEn ?h Bree)
        )
      )
    )
  )
)
