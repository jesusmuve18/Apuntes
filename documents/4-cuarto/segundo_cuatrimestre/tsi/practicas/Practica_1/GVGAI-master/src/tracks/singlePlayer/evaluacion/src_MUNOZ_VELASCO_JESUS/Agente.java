package tracks.singlePlayer.evaluacion.src_MUNOZ_VELASCO_JESUS;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Queue;
import java.util.ArrayDeque;

import core.game.Observation;
import core.game.StateObservation;
import ontology.Types.ACTIONS;
import core.player.AbstractPlayer;
import tools.ElapsedCpuTimer;

// Clase que incluye todas las funcionalidades comunes a los algoritmos de búsqueda
public class Agente extends AbstractPlayer{
    
    // Constantes para definir el mapa de caracteres (obtenidas empíricamente)
    static final int ITYPE_WALL = 0;
    static final int ITPYPE_FLOOR = 2;
    static final int ITYPE_WATER = 3;
    static final int ITYPE_CATAPULT_E = 7;
    static final int ITYPE_CATAPULT_S = 5;
    static final int ITYPE_CATAPULT_N = 6;
    static final int ITYPE_CATAPULT_W = 8;
    static final int ITYPE_AVATAR = 9;
    static final int ITYPE_COINS = 15;
    static final int ITYPE_KEY = 16;
    static final int ITYPE_GATE = 18;

    // Variables de estado

    ArrayList<Observation>[][] mapaOriginal;    // Mapa de observaciones
    char[][] mapaChar;                          // Mapa con caracteres

    Node nodoObjetivo;          // Nodo objetivo (portal)
    Node nodoInicial;       // Nodo inicial
    long tiempo_ns;         // Tiempo de razonamiento de un agente

    /*  HashMap que se utilizará para traducir posiciones del mapa (coordenadas x,y)
        en posiciones de una cadena binaria (posiciones de la 0 a la 63). Se utilizará
        para almacenar las posiciones de los objetos coleccionables del mapa y poder 
        usar una máscara de bits. 
        
        De esta forma si hay un elemento coleccionable en las coordenadas (x_0, y_0) 
        y es el objeto coleccionable número n_0 que se encuentra se almacenará en el
        mapa hash el elemento <(x_0, y_0), n_0>. Así, cuando se quiera comprobar si 
        en esa casilla hay un objeto coleccionable solo tendrá que usarse
        > itemToIndex((x,y))
        y este devolverá la posición de la cadena de bits si es coleccionable y null
        en otro caso (si no es coleccionable).                                        
        
        De esta forma, si un nodo pasa por dicha casilla y quiere marcar que ya ha
        recogido ese objeto, primero obtendrá el índice como se ha indicado. Una vez
        se ha obtenido dicho índice i se pondrá el bit i de la máscara correspondiente
        al objeto coleccionable dentro del nodo a 1. 
        */
    private HashMap<Integer, Integer> itemToIndex;

    // Constructor
    public Agente(StateObservation stateObs, ElapsedCpuTimer elapsedTimer){ 
        super();

        this.mapaOriginal = stateObs.getObservationGrid();  // Leemos mapa de observaciones
        this.mapaChar = mapToCharMap(mapaOriginal);         // Lo convertimos en mapa de caracteres

        // Obtenemos el nodo inicial y objetivo
        int blockSize = stateObs.getBlockSize();

        // Nodo Inicial
        nodoInicial = new Node(
            (stateObs.getAvatarPosition().x / blockSize), 
            (stateObs.getAvatarPosition().y / blockSize)
        );

        // Suponemos que solo hay un nodo objetivo (si hay más cogemos el último que encontramos)
        for (ArrayList<Observation> obs : stateObs.getPortalsPositions()){
            for (Observation o : obs) {
                nodoObjetivo = new Node((o.position.x/blockSize), (o.position.y/blockSize));
            }
        }

        // Inicializamos el HashMap
        this.itemToIndex = new HashMap<>();

        // Mapear cada objeto coleccionable a un bit del 0 al 63
        int indexCounter = 0;
        for (int x = 0; x < mapaChar.length; x++) {
            for (int y = 0; y < mapaChar[x].length; y++) {
                char c = mapaChar[x][y];
                // Si es moneda, catapulta o llave, le asignamos un bit
                if (c == 'c' || c == 'm' || c == '0' || c == '1' || c == '2' || c == '3' || c == 'k') {
                    if (indexCounter < 64) {
                        itemToIndex.put(getPosKey(x, y), indexCounter++);
                    }
                }
            }
        }

        // Inicializamos el tiempo transcurrido
        tiempo_ns = 0;
    }

    // Método act
    public ACTIONS act(StateObservation stateObs, ElapsedCpuTimer elapsedTimer){
        // Por defecto
        updateT(elapsedTimer);
        return ACTIONS.ACTION_NIL;
    }

    // Devuelve la posKey de una posición (para el mapa de bits)
    public int getPosKey(int pos_x, int pos_y) {
        // Aprovecha que los mapas no son muy grandes para convertir una coordenada en un identificador único
        // Ejemplo: (15, 24) -> 150024
        // (Máxima dimensión del mapa: 9999x9999)
        return pos_x * 10000 + pos_y;
    }

    // Para obtener el índice de un objeto coleccionable (su coordenada)
    // Devuelve -1 si no es coleccionable
    public int getItemIndex(int x, int y) {
        Integer idx = itemToIndex.get(getPosKey(x, y));
        return (idx != null) ? idx : -1;
    }

    // Verifica si un nodo es el objetivo
    public boolean isTarget(Node actual) {
        return ((actual.keys > 0)&& actual.samePositionAs(nodoObjetivo));
    }

    // Convierte un mapa de Observaciones en un mapa de Caracteres (de terreno)
    public char[][] mapToCharMap(ArrayList<Observation>[][] mapaOriginal) {

        // Inicializamos el mapa con las dimensiones adecuadas
        int width = mapaOriginal.length;
        int height = mapaOriginal[0].length;
        char[][] mapa = new char[width][height];

        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                ArrayList<Observation> celda = mapaOriginal[x][y];

                // Asumimos por defecto que es suelo vacío
                char car = '_';

                boolean hayAgua = false;
                boolean hayMoneda = false;

                if (celda != null && celda.size() > 0) {
                    // Recorremos todo lo que hay en esta coordenada
                    for (Observation obs : celda) {

                        // Buscamos el caracter según la prioridad (como si fueran capas)
                        if (obs.itype == ITYPE_WALL)
                            car = 'w';
                        else if (obs.itype == ITYPE_GATE)
                            car = 'g';
                        else if (obs.itype == ITYPE_KEY)
                            car = 'k';
                        else if (obs.itype == ITYPE_CATAPULT_E)
                            car = '0';
                        else if (obs.itype == ITYPE_CATAPULT_S)
                            car = '1';
                        else if (obs.itype == ITYPE_CATAPULT_N)
                            car = '2';
                        else if (obs.itype == ITYPE_CATAPULT_W)
                            car = '3';
                        else if (obs.itype == ITYPE_AVATAR)
                            car = 'A';

                        // Moneda en agua
                        if (obs.itype == ITYPE_WATER)
                            hayAgua = true;
                        if (obs.itype == ITYPE_COINS)
                            hayMoneda = true;
                    }

                    // Si después de mirar toda la celda encontramos combinaciones
                    if (hayAgua && hayMoneda) {
                        car = 'm'; // Moneda sobre agua
                    } else if (hayAgua && !hayMoneda && car == '_') {
                        car = '.'; // Solo agua
                    } else if (!hayAgua && hayMoneda && car == '_') {
                        car = 'c'; // Moneda sobre suelo
                    }
                }
                mapa[x][y] = car;
            }
        }

        return mapa;
    }

    // Imprime un mapa de Caracteres (de terreno)
    public void inspectMapChar() {
        for(int x = 0; x<mapaChar.length; x++) {
            for(int y=0; y<mapaChar[x].length; y++){
                System.out.print(mapaChar[x][y]);
            }
            System.out.print("\n");
        }
    }

    // Devuelve una cola con los movimientos válidos desde la posición del nodo n (en el orden requerido)
    public Queue<ACTIONS> validMoves(Node n) {
        Queue<ACTIONS> moves = new ArrayDeque<>();
        
        // Obtenemos la posición del nodo
        int pos_x = n.pos_x;
        int pos_y = n.pos_y;

        // Obtenemos la dimensión del mapa
        int max_x = mapaChar.length;
        int max_y = mapaChar[0].length;

        // DERECHA
        if(pos_x + 1 < max_x && n.canMoveTo(mapaChar[pos_x+1][pos_y], getItemIndex(pos_x+1, pos_y))) {
            moves.offer(ACTIONS.ACTION_RIGHT);
        }
        // ARRIBA
        if(pos_y > 0 && n.canMoveTo(mapaChar[pos_x][pos_y-1], getItemIndex(pos_x, pos_y-1))) {
            moves.offer(ACTIONS.ACTION_UP);
        }
        // IZQUIERDA
        if(pos_x > 0 && n.canMoveTo(mapaChar[pos_x-1][pos_y], getItemIndex(pos_x-1, pos_y))) {
            moves.offer(ACTIONS.ACTION_LEFT);
        }
        // ABAJO
        if(pos_y + 1 < max_y && n.canMoveTo(mapaChar[pos_x][pos_y+1], getItemIndex(pos_x, pos_y+1))) {
            moves.offer(ACTIONS.ACTION_DOWN);
        }

        return moves;
    }

    // Indica si habrá un muro si el nodo n se mueve en una dirección
    public boolean isWallIfMove(Node n, ACTIONS move) {
        char casilla = terrainOf(n.newPositionXIfMove(move), n.newPositionYIfMove(move));

        // Entendemos que un muro es:
        // - Un muro (literalmente)
        // - Un portal cerrado (sin la llave previamente recogida)
        return (casilla == 'w' ||(casilla == 'g' && (n.keys == 0)));
    }

    // Actualiza un nodo según su posición actual (su estado)
    public void update(Node n) {
        int itemIndex = getItemIndex(n.pos_x, n.pos_y);
        n.update(terrainOf(n), itemIndex);
    }

    // Devuelve el terreno en el que está un nodo
    public char terrainOf(Node node) {
        return terrainOf(node.pos_x, node.pos_y);
    }

    // Devuelve el terreno de una coordenada
    public char terrainOf(int x, int y) {
        return mapaChar[x][y];
    }

    // Devuelve los sucesores de un nodo
    ArrayList<Node> successorsOf(Node node) {
        // Inicializamos los sucesores
        ArrayList<Node> sucesores = new ArrayList<>();

        if(node.in_a_catapult) {            
            // Si está en una catapulta hace NIL y deja de estarlo (empieza a volar en cierta dirección)
            Node child = new Node(node, ACTIONS.ACTION_NIL);
            update(child);

            // Añado el hijo a sucesores
            sucesores.add(child);
            
        } else if(node.is_flying) {                
            // Creo un hijo (sin moverlo)
            Node child = new Node(node, ACTIONS.ACTION_NIL);

            // Si hay un muro dejo de volar
            if(isWallIfMove(node, node.catapult_direction)) {
                // Si puedo pararme (el terreno me lo permite y no muero)
                if(node.canMoveTo(terrainOf(node), getItemIndex(node.pos_x, node.pos_y))) {
                    child.is_flying = false;                // Ya no está volando
                    update(child);                          // Actualizo el estado del hijo
                    sucesores.add(child);                   // Lo meto en sucesores 
                }
            } else {
                // Si no hay ningún muro sigo volando en esa dirección
                child.moveTo(node.catapult_direction);
                update(child);                          // Actualizo el estado del hijo
                sucesores.add(child);                   // Lo meto en sucesores
            }             
        } else {
            // No está volando ni en una catapulta
            Queue<ACTIONS> moves = validMoves(node);

            while (!moves.isEmpty()) {
                // Selecciono la acción
                ACTIONS m = moves.poll();

                // Creo al sucesor
                Node child = new Node(node, m);
                update(child);

                // Si me quedan monedas (sigo vivo)
                if(child.coins >= 0) sucesores.add(child);                   // Lo meto en sucesores
                
            }
        }
        return sucesores;
    }

    // Actualiza el tiempo
    public void updateT(ElapsedCpuTimer elapsedTimer) {
        this.tiempo_ns += elapsedTimer.elapsedNanos();
    }

    // Devuelve el tiempo en ms
    public long getTms() {
        return (tiempo_ns/ 1000000);
    }
}
