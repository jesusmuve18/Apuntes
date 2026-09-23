(define (problem ejercicio5)
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
    NodoMineral_Moria NodoMithril_Moria  - NodoRecurso
    NodoMineral_Erebor                   - NodoRecurso
    NodoMadera_Fangorn                   - NodoRecurso
    NodoMadera_Lothlorien                - NodoRecurso
    NodoMadera_Mirkwood                  - NodoRecurso
    NodoAlimento_Hobbiton                - NodoRecurso
    NodoEspecia_Tolfolas                 - NodoRecurso
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

    ; Mapa de caminos (con costes)
    ; Hobbiton <-> Bree
    (camino Hobbiton Bree)              (= (costeCamino Hobbiton Bree) 1)
    (camino Bree Hobbiton)              (= (costeCamino Bree Hobbiton) 1)

    ; Bree <-> Rivendell
    (camino Bree Rivendell)             (= (costeCamino Bree Rivendell) 1)
    (camino Rivendell Bree)             (= (costeCamino Rivendell Bree) 1)

    ; Rivendell <-> HighPass
    (camino Rivendell HighPass)         (= (costeCamino Rivendell HighPass) 2)
    (camino HighPass Rivendell)         (= (costeCamino HighPass Rivendell) 2)

    ; HighPass <-> Mirkwood
    (camino HighPass Mirkwood)          (= (costeCamino HighPass Mirkwood) 2)
    (camino Mirkwood HighPass)          (= (costeCamino Mirkwood HighPass) 2)

    ; Mirkwood <-> Erebor
    (camino Mirkwood Erebor)            (= (costeCamino Mirkwood Erebor) 2)
    (camino Erebor Mirkwood)            (= (costeCamino Erebor Mirkwood) 2)

    ; Hobbiton <-> Tharbad
    (camino Hobbiton Tharbad)           (= (costeCamino Hobbiton Tharbad) 1)
    (camino Tharbad Hobbiton)           (= (costeCamino Tharbad Hobbiton) 1)

    ; Bree <-> Tharbad
    (camino Bree Tharbad)               (= (costeCamino Bree Tharbad) 1)
    (camino Tharbad Bree)               (= (costeCamino Tharbad Bree) 1)

    ; Tharbad <-> Helm's Deep
    (camino Tharbad HelmsDeep)          (= (costeCamino Tharbad HelmsDeep) 3)
    (camino HelmsDeep Tharbad)          (= (costeCamino HelmsDeep Tharbad) 3)

    ; Helm's Deep <-> Isengard
    (camino HelmsDeep Isengard)         (= (costeCamino HelmsDeep Isengard) 1)
    (camino Isengard HelmsDeep)         (= (costeCamino Isengard  HelmsDeep) 1)

    ; Isengard <-> Fangorn
    (camino Isengard Fangorn)           (= (costeCamino Isengard Fangorn) 1)
    (camino Fangorn Isengard)           (= (costeCamino Fangorn Isengard) 1)

    ; Fangorn <-> Amon Hen
    (camino Fangorn AmonHen)            (= (costeCamino Fangorn AmonHen) 8)
    (camino AmonHen Fangorn)            (= (costeCamino AmonHen Fangorn) 8)

    ; Rivendell <-> Moria  (camino que se destruye)
    (camino Rivendell Moria)            (= (costeCamino Rivendell Moria) 3)
    (camino Moria Rivendell)            (= (costeCamino Moria Rivendell) 3)
    (caminoDestruible Rivendell Moria)  
    (caminoDestruible Moria Rivendell)

    ; Moria <-> Lothlorien
    (camino Moria Lothlorien)           (= (costeCamino Moria Lothlorien) 3)
    (camino Lothlorien Moria)           (= (costeCamino Lothlorien Moria) 3)

    ; Lothlorien <-> Amon Hen
    (camino Lothlorien AmonHen)         (= (costeCamino Lothlorien AmonHen) 5)
    (camino AmonHen Lothlorien)         (= (costeCamino AmonHen Lothlorien) 5)

    ; Helm's Deep <-> Edoras
    (camino HelmsDeep Edoras)           (= (costeCamino HelmsDeep Edoras) 1)
    (camino Edoras HelmsDeep)           (= (costeCamino Edoras HelmsDeep) 1)

    ; Edoras <-> Dol Amroth
    (camino Edoras DolAmroth)           (= (costeCamino Edoras DolAmroth) 2)
    (camino DolAmroth Edoras)           (= (costeCamino DolAmroth Edoras) 2)

    ; Dol Amroth <-> Tolfolas
    (camino DolAmroth Tolfolas)         (= (costeCamino DolAmroth Tolfolas) 1)
    (camino Tolfolas DolAmroth)         (= (costeCamino Tolfolas DolAmroth) 1)

    ; Tolfolas <-> Minas Tirith
    (camino Tolfolas MinasTirith)       (= (costeCamino Tolfolas MinasTirith) 2)
    (camino MinasTirith Tolfolas)       (= (costeCamino MinasTirith Tolfolas) 2)

    ; Edoras <-> Minas Tirith
    (camino Edoras MinasTirith)         (= (costeCamino Edoras MinasTirith) 2)
    (camino MinasTirith Edoras)         (= (costeCamino MinasTirith Edoras) 2)

    ; AmonHen <-> DeadMarshes
    (camino AmonHen DeadMarshes)        (= (costeCamino AmonHen DeadMarshes) 2)
    (camino DeadMarshes AmonHen)        (= (costeCamino DeadMarshes AmonHen) 2)

    ; DeadMarshes <-> MinasMorgul
    (camino DeadMarshes MinasMorgul)    (= (costeCamino DeadMarshes MinasMorgul) 2)
    (camino MinasMorgul DeadMarshes)    (= (costeCamino MinasMorgul DeadMarshes) 2)

    ; MinasMorgul <-> Orodruin
    (camino MinasMorgul Orodruin)       (= (costeCamino MinasMorgul Orodruin) 3)
    (camino Orodruin MinasMorgul)       (= (costeCamino Orodruin MinasMorgul) 3)

    ; MinasMorgul <-> MinasTirith
    (camino MinasMorgul MinasTirith)    (= (costeCamino MinasMorgul MinasTirith) 2)
    (camino MinasTirith MinasMorgul)    (= (costeCamino MinasTirith MinasMorgul) 2)

    ; inicialización del acumulador de coste total
    (= (total-cost) 0)
  )

  ; Objetivo: idéntico al Ejercicio 1
  ; Que haya un personaje asignado a un nodo de Mithril y otro personaje asignado a un nodo de Alimento.
  (:goal
    (and
      (trabajando Enano1  Moria    Mithril)
      (trabajando Hobbit1 Hobbiton Alimento)
    )
  )

  ; Métrica de optimización
  ; minimizar coste total
  (:metric minimize (total-cost))

)
