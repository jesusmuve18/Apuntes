package tracks.singlePlayer.evaluacion.src_MUNOZ_VELASCO_JESUS;

import java.util.LinkedList;
import java.util.Queue;
import java.util.ArrayList;
import java.util.HashMap;

import core.game.StateObservation;
import ontology.Types.ACTIONS;
import tools.ElapsedCpuTimer;
import tracks.singlePlayer.MetricsProvider;

// Agente que implementa el algoritmo LRTA*(k)
public class AgenteLRTAStarK extends Agente {

    // Variables de estado
    private static int k = 5;    // Límite de propagación de la heurística  

    Node nodoActual;

    /*
        La heurística es un mapa hash para poder hacer consultas en O(1) sobre el valor de la heurística de un nodo. 
        La inserción también es O(1)

        El mapaSoportes es un mapa hash para poder actualizar y consultar el mejor vecino de cada nodo en O(1)
    */
    HashMap<Node, Integer> heuristica;
    HashMap<Node, Node> mapaSoportes;

    // Métricas
    int num_acciones;
    int nodos_expandidos;
    int num_actualizaciones_h;
    
    // Constructor
    public AgenteLRTAStarK(StateObservation stateObs, ElapsedCpuTimer elapsedTimer) {
        super(stateObs, elapsedTimer);

        // Inicializo las variables de estado
        nodoActual = super.nodoInicial;
        heuristica = new HashMap<>();
        mapaSoportes = new HashMap<>();

        // Inicializo las métricas
        num_acciones = 0;
        nodos_expandidos = 0;
        num_actualizaciones_h = 0;
    }

    @Override
    public ACTIONS act(StateObservation stateObs, ElapsedCpuTimer elapsedTimer) {

        // Compruebo si es objetivo
        if(isTarget(nodoActual)){
            updateT(elapsedTimer);
            return ACTIONS.ACTION_NIL;
        }

        // Actualizo las métricas
        nodos_expandidos++;

        // Calculo la heurística y propago (hasta k)
        lookaheadUpdateK(nodoActual);

        // Selecciono el mejor vecino
        Node mejor_vecino = null;
        int min = Integer.MAX_VALUE;
        
        ArrayList<Node> sucesores = successorsOf(nodoActual);   // Genero todos los sucesores

        // Recorro los sucesores buscando el mejor vecino (mejor f)
        for (Node sucesor : sucesores) {
            int h_suc = h(sucesor);
            int f = (h_suc == Integer.MAX_VALUE) ? Integer.MAX_VALUE : h_suc + 1; // evito overflow
            
            if(f < min) {
                min = f;
                mejor_vecino = sucesor;
            }
        }

        // Actualizo las métricas
        num_acciones++;

        // Me muevo al mejor vecino
        if(mejor_vecino != null) {
            nodoActual = mejor_vecino;
            updateT(elapsedTimer);
            return nodoActual.last_action;
        } else {
            // No me puedo mover (no tengo vecinos)
            updateT(elapsedTimer);
            return ACTIONS.ACTION_NIL;
        }
    }

    @Override
    public void result(StateObservation stateObservation, ElapsedCpuTimer elapsedCpuTimer) {       
        MetricsProvider.getInstance().setNumAccionesPlan(num_acciones);
        MetricsProvider.getInstance().setNodosExpandidos(nodos_expandidos);
        MetricsProvider.getInstance().setNumActualizacionesTabla(num_actualizaciones_h);
        MetricsProvider.getInstance().setTiempoMilisegundos(getTms());
        MetricsProvider.getInstance().printMetrics();
    }
    
    // Propagar la heurística del nodo actual hasta en k nodos
    public void lookaheadUpdateK(Node actual) {

        // Inicializo una cola con el nodo actual
        Queue<Node> cola = new LinkedList<>();
        cola.add(actual);

        int contador = k - 1;
        ArrayList<Node> sucesores;
        Node mejor_sucesor;
        boolean propagar;

        while (!cola.isEmpty()) {
            // Selecciono el primer nodo
            Node x = cola.poll();

            // Busco el mejor vecino
            int min = Integer.MAX_VALUE;
            mejor_sucesor = null;

            sucesores = successorsOf(x);    // Genero los sucesores (vecinos)

            // Recorro los sucesores buscando al mejor
            for (Node sucesor : sucesores) {
                int h_suc = h(sucesor);
                int f = (h_suc == Integer.MAX_VALUE) ? Integer.MAX_VALUE : h_suc + 1; 
                
                if(f < min) {
                    min = f;
                    mejor_sucesor = sucesor;
                }
            }

            // Actualizamos el soporte de x con el mejor sucesor
            updateSoporte(x, mejor_sucesor);

            propagar = false;

            // Aprendizaje
            if(min != Integer.MAX_VALUE && h(x) < min) { 
                propagar = true;
                updateH(x, min);
            }

            // Propagación del cambio a los nodos cuyo soporte es x
            if(propagar) {
                for(Node sucesor : sucesores) {
                    if(contador > 0 && soporte(sucesor) != null && soporte(sucesor).equals(x)) {
                        cola.add(sucesor);
                        contador--;
                    }
                }
            }
        }
    }

    // Heurística de un nodo (si no existe la calcula)
    public int h(Node n) {
        if (heuristica.containsKey(n)) {
            return heuristica.get(n);
        }
        return n.distanceTo(nodoObjetivo);
    }

    // Actualiza la heurística de un nodo n
    public void updateH(Node n, int newValue) {
        num_actualizaciones_h++;
        heuristica.put(n, newValue);
    }

    // Devuelve el soporte de un nodo n (el mejor vecino)
    public Node soporte(Node n) {
        return mapaSoportes.get(n); 
    }

    // Actualiza el soporte de un nodo n (el nuevo mejor vecino)
    public void updateSoporte(Node n, Node mejor_vecino) {
        mapaSoportes.put(n, mejor_vecino);
    }
}