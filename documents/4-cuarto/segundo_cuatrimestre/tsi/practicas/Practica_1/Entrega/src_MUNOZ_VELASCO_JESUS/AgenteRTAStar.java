package tracks.singlePlayer.evaluacion.src_MUNOZ_VELASCO_JESUS;

import java.util.ArrayList;
import java.util.HashMap;

import core.game.StateObservation;
import ontology.Types.ACTIONS;
import tools.ElapsedCpuTimer;
import tracks.singlePlayer.MetricsProvider;

// Agente que implementa el Algoritmo RTA*
public class AgenteRTAStar extends Agente {

    // Variables de estado
    Node nodoActual;

    /*
        Se ha elegido un mapa Hash para la heurística ya que la consulta sobre la heurística de un nodo,
        la inserción y la actualización se realizan en O(1).
    */
    HashMap<Node, Integer> heuristica;

    // Métricas
    int num_acciones;
    int nodos_expandidos;

    // Constructor
    public AgenteRTAStar(StateObservation stateObs, ElapsedCpuTimer elapsedTimer) {
        super(stateObs, elapsedTimer);

        // Inicializo las variables de estado
        nodoActual = nodoInicial;
        heuristica = new HashMap<>();

        // Inicializo las métricas
        num_acciones = 0;
        nodos_expandidos = 0;
    }

    @Override
    public ACTIONS act(StateObservation stateObs, ElapsedCpuTimer elapsedTimer) {

        if(!isTarget(nodoActual)){  // Si no es objetivo

            // Obtengo los sucesores del nodo actual
            ArrayList<Node> S = successorsOf(nodoActual);

            // Actualizo las métricas
            nodos_expandidos++;

            // Obtengo el primer y segundo mínimo de las f(n) de los sucesores

            int z = Integer.MAX_VALUE;              // argmin
            int segundo_min = Integer.MAX_VALUE;    // segundo minimo
            Node mejorVecino = null;

            // Recorremos los sucesores (estrategia de movimiento)
            for (Node sucesor : S) {
                // Calculo f
                int f = h(sucesor) + sucesor.distanceTo(nodoActual);

                // Actualizamos los mínimos
                if(f<z) {
                    // El mejor pasa a ser el segundo mejor
                    segundo_min = z;
                    z = f;
                    mejorVecino = sucesor;
                } else if(f<segundo_min) {
                    segundo_min = f;
                }
            }

            // Actualizamos la heurística (regla de aprendizaje)
            if(z != Integer.MAX_VALUE) {
                if(segundo_min != Integer.MAX_VALUE) {
                    updateH(nodoActual,Math.max(h(nodoActual), segundo_min));
                } else {
                    updateH(nodoActual,Math.max(h(nodoActual), z));
                }
            }
            
            if(mejorVecino != null) {
                // Si existe un mejor vecino me muevo hacia él
                nodoActual = mejorVecino;
                num_acciones++;
                updateT(elapsedTimer);
                return nodoActual.last_action;
            } else {
                // Si no existen vecinos no me muevo (estoy perdido)
                num_acciones++;
                updateT(elapsedTimer);
                return ACTIONS.ACTION_NIL;
            }
        }

        // Si es objetivo
        num_acciones++;
        updateT(elapsedTimer);
        return ACTIONS.ACTION_NIL;
    }

    @Override
    public void result(StateObservation stateObservation, ElapsedCpuTimer elapsedCpuTimer) {
        MetricsProvider.getInstance().setNumAccionesPlan(num_acciones);
        MetricsProvider.getInstance().setNodosExpandidos(nodos_expandidos);
        MetricsProvider.getInstance().setTiempoMilisegundos(getTms());
        MetricsProvider.getInstance().printMetrics();
    }

    // Devuelve la heurística de un nodo n (si no existe la inicializa)
    public int h(Node n) {
        // Si es la primera vez que pasamos por aquí calculamos la heurística
        if (!heuristica.containsKey(n)) {
            updateH(n, n.distanceTo(nodoObjetivo));
        }
        
        return heuristica.get(n);
    }

    // Actualiza la heurística de un nodo n
    public void updateH(Node n, int newValue) {
        // Si existe lo sobreescribe
        heuristica.put(n, newValue);
    }
    
}
