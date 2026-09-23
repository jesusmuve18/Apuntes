package tracks.singlePlayer.evaluacion.src_MUNOZ_VELASCO_JESUS;

import ontology.Types.ACTIONS;

// Clase que representa un estado del juego (nodo de un árbol de estados)
public class Node {
    // Constantes 
    public static int MAX_COINS = 5;        // Número máximo de monedas que se pueden tener en el inventario
    
    // Variables de estado

    // Posición
    int pos_x;
    int pos_y;

    // Métricas
    int coins;                              // Número de monedas recogidas
    int actual_cost;                        // Coste actual
    int estimated_cost;                     // Heurística
    int keys;                               // Número de llaves recogidas
    boolean is_flying;                      // Está volando (o no)
    boolean in_a_catapult;                  // Está en una catapulta
    ACTIONS catapult_direction;             // Dirección de vuelo
    
    // Historial
    long coins_bitmask;                     // Cada bit es una moneda (1=recogida, 0=no)
    long catapults_bitmask;                 // Cada bit es una catapulta usada
    long keys_bitmask;                      // Cada bit es una llave

    // Construcción de la ruta
    Node parent;                            // Nodo padre
    ACTIONS last_action;                    // Última acción (para reconstruir la ruta)

    // Identificador de antigüedad
    int id;

    // Constructor por defecto
    public Node(int pos_x, int pos_y) {
        this.pos_x =  pos_x;
        this.pos_y = pos_y;
        this.coins = 0;
        this.actual_cost = 0;
        this.keys = 0;
        this.is_flying = false;
        this.in_a_catapult = false;

        this.coins_bitmask = 0L;
        this.catapults_bitmask = 0L;
        this.keys_bitmask = 0L;

        this.parent = null;
        this.last_action = null;
    }

    // Constructor de doubles
    public Node(double pos_x, double pos_y) {
        this((int) pos_x, (int) pos_y);
    }

    // Constructor a partir de un nodo existente y una acción
    public Node(Node node, ACTIONS action) {
        this.pos_x = node.pos_x;
        this.pos_y = node.pos_y;
        this.coins = node.coins;
        this.actual_cost = node.actual_cost + 1;
        this.estimated_cost = node.estimated_cost;
        this.keys = node.keys;
        this.is_flying = node.is_flying;
        this.catapult_direction = node.catapult_direction;
        this.in_a_catapult = node.in_a_catapult;

        this.coins_bitmask = node.coins_bitmask;
        this.catapults_bitmask = node.catapults_bitmask;
        this.keys_bitmask = node.keys_bitmask;
        
        this.parent = node;
        this.last_action = action;

        moveTo(action);
    }

    // Operador de comparación de posición
    public boolean samePositionAs(Node node) {
        return (this.pos_x == node.pos_x && this.pos_y == node.pos_y);
    }

    // Distancia desde el nodo actual hasta otro nodo
    public int distanceTo(Node node) {
        // Distancia Manhattan: |x1 - x2| + |y1 - y2|
        return Math.abs(this.pos_x - node.pos_x) + Math.abs(this.pos_y - node.pos_y);
    }

    // Nueva posición de X del nodo actual si se moviera según la dirección a
    public int newPositionXIfMove(ACTIONS a) {
        if (a == ACTIONS.ACTION_RIGHT) return this.pos_x + 1;
        if (a == ACTIONS.ACTION_LEFT) return this.pos_x - 1;
        return this.pos_x;
    }

    // Nueva posición de Y del nodo actual si se moviera según la dirección a
    public int newPositionYIfMove(ACTIONS a) {
        if (a == ACTIONS.ACTION_UP) return this.pos_y - 1;
        if (a == ACTIONS.ACTION_DOWN) return this.pos_y + 1;
        return this.pos_y;
    }

    // Método de comprobación de terreno
    public boolean canMoveTo(char casilla, int itemIndex) {
        long bit = (1L << itemIndex);

        return (
            casilla == '_' ||   // Césped
            casilla == 'c' ||   // Moneda
            ((casilla == '0' || casilla == '1' || casilla == '2'|| casilla == '3' ) 
                && ((catapults_bitmask & bit) != 0 || this.coins > 0)) ||  // Catapulta
            casilla == 'k' ||
            (casilla == 'g' && this.keys > 0) ||   // Portal
            casilla == 'A' // Casilla inicial
        );
    }

    // Mover el nodo a una nueva posición
    public void moveTo(ACTIONS action) {
        this.pos_x = newPositionXIfMove(action);
        this.pos_y = newPositionYIfMove(action);
    }

    // Devuelve la dirección de una catapulta
    public ACTIONS getDirectionFromChar(char c) {
        ACTIONS direction = null;

        switch (c) {
            case '0':
                direction = ACTIONS.ACTION_RIGHT;
                break;
            case '1':
                direction = ACTIONS.ACTION_DOWN;
                break;
            case '2':
                direction = ACTIONS.ACTION_UP;
                break;
            case '3':
                direction = ACTIONS.ACTION_LEFT;
                break;
        }
        return direction;
    }

    // Actualizar el nodo según la casilla de terreno en la que esté
    public void update(char casilla, int itemIndex) {
        if(in_a_catapult) in_a_catapult = false;  // Siempre supongo que deja de estar en una catapulta

        // Si el itemIndex es válido (-1 significa que no es coleccionable)
        if (itemIndex != -1) {
            long bit = (1L << itemIndex); // Creamos la máscara para este objeto

            switch (casilla) {
                case 'c': case 'm': // Monedas
                    if (((coins_bitmask & bit) == 0) && coins < MAX_COINS) { // Si el bit es 0 (no recogida) y la puedo recoger
                        coins += 1;
                        coins_bitmask |= bit; // Marcamos como recogida (ponemos el bit a 1)
                    }
                    break;
                case '0': case '1': case '2': case '3': // Catapultas
                    // Si hay una catapulta por la que no he pasado
                    if ((catapults_bitmask & bit) == 0) {
                        if(!is_flying && coins > 0) {
                            coins -= 1;
                            is_flying = true;
                        }
                        in_a_catapult = true;
                        catapult_direction = getDirectionFromChar(casilla);
                        catapults_bitmask |= bit; // Marcamos como usada
                    }
                    break;
                case 'k':
                    if((keys_bitmask & bit) == 0) {
                        keys +=1;
                        keys_bitmask |= bit;    // Marcamos como recogida
                    }
                    break;
            }
        }
    }

    // Método de consulta del estado del nodo
    public void inspect() {
        System.out.println("---------------------------------------------------");
        System.out.println("Posición:                       (" + this.pos_x + "," + this.pos_y +")");
        System.out.println("Monedas:                        " + coins);
        System.out.println("Monedas recogidas:              " +  Long.toBinaryString(coins_bitmask));
        // System.out.println("Monedas restantes:              " + coins_left);
        System.out.println("Coste de la ruta actual:        " + actual_cost);
        System.out.println("Coste estimado hasta objetivo:  " + estimated_cost);
        System.out.println("Llaves:                         " + keys);
        System.out.println("Llaves  recogidas:              " +  Long.toBinaryString(keys_bitmask));
        // System.out.println("Número de llaves restantes:     " + keys_left);
        System.out.println("Está volando:                   " + is_flying);
        System.out.println("Está en una catapulta:          " + in_a_catapult);
        System.out.println("Dirección de la catapulta:      " + catapult_direction);
        System.out.println("Catapultas pasadas:             " +  Long.toBinaryString(catapults_bitmask));
        // System.out.println("Número de catapultas restantes: " + catapults_left);
        System.out.println("Última acción:                  " + last_action);
        System.out.println("---------------------------------------------------");
    }

    // Actualiza su costo estimado según la casilla objetivo
    public void updateEstimatedCostTo(Node objetivo){
        this.estimated_cost = this.distanceTo(objetivo);
    }
    
    // Método de comparación de nodos
    public boolean isBetterThan(Node n) {
        return ((this.actual_cost) < (n.actual_cost));
    }

    // Clave posicional
    public String getPositionKey() {
        return this.pos_x + "," + this.pos_y;
    }

    // Para usar una tabla o mapa Hash
    @Override
    public boolean equals(Object obj) {
        // Si son exactamente el mismo objeto en memoria, son iguales
        if (this == obj) return true;
        
        // Si el otro objeto es nulo o no es de la clase Node, no son iguales
        if (obj == null || getClass() != obj.getClass()) return false;
        
        // Convertimos el Object genérico de Java a la clase Node
        Node node = (Node) obj;
        
        // Comparamos el estado
        return (
            this.pos_x == node.pos_x &&
            this.pos_y == node.pos_y &&
            this.coins == node.coins &&
            this.keys == node.keys &&
            this.is_flying == node.is_flying &&
            this.in_a_catapult == node.in_a_catapult &&
            this.catapult_direction == node.catapult_direction &&
            this.coins_bitmask == node.coins_bitmask &&
            this.catapults_bitmask == node.catapults_bitmask &&
            this.keys_bitmask == node.keys_bitmask
        );
    }

    // Para usar una tabla o mapa Hash
    @Override
    public int hashCode() {
        // Le pasamos exactamente las mismas variables que usamos en el equals
        return java.util.Objects.hash(
            pos_x, pos_y, coins, keys, is_flying, in_a_catapult, catapult_direction, coins_bitmask, catapults_bitmask, keys_bitmask
        );
    }
}
