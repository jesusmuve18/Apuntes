package tracks.singlePlayer.evaluacion.src_MUNOZ_VELASCO_JESUS;

import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.HashSet;

import core.game.StateObservation;
import ontology.Types.ACTIONS;
import tools.ElapsedCpuTimer;
import tracks.singlePlayer.MetricsProvider;

// Nota: Se puede heredar de otras clases personalizadas por el alumnado que hereden de AbstractPlayer para generalizar
// los elementos comunes a todos los algoritmos.

public class AgenteProfundidad extends Agente {

    // Variables de estado  

    /*
        "cerrados" es una tabla Hash para poder consultar en O(1) si un nodo se ha visitado o no.

        "plan" es una pila ya que para reconstruirla se empieza desde el nodo final hasta el inicial
        y luego hay que invertirla (al ser una pila esta inversión es natural).
    */
    HashSet<Node> cerrados;
    ArrayDeque<ACTIONS> plan;                   // Pila del plan

    // Métricas
    int nodosExplorados;
    int profundidadMaxima;
    int nAccionesPlan;

    // Constructor
    public AgenteProfundidad(StateObservation stateObs, ElapsedCpuTimer elapsedTimer) {
        super(stateObs, elapsedTimer);

        // Inicializo las variables de estado
        cerrados = new HashSet<>();
        plan = new ArrayDeque<>();
        
        // Inicializo cerrados
        cerrados.add(nodoInicial);

        // Inicializo las métricas
        nodosExplorados = 0;
        profundidadMaxima = 0;
        nAccionesPlan = 0;
    }

    // Método de búsqueda en profundidad
    public boolean DFS_Search(Node u, Node target) {

        // Actualizo la profundidad máxima
        if (u.actual_cost > profundidadMaxima) {
            profundidadMaxima = (int)u.actual_cost;
        }
        // Si he llegado al destino paro y reconstruyo la ruta
        if(isTarget(u)){
            rebuildPlan(u);
            return true;
        }         

        // Obtengo los sucesores
        ArrayList<Node> successors = successorsOf(u);
        
        // Actualizo las métricas
        nodosExplorados++;

        // Recorro los sucesores
        for(Node child : successors) {
            // Si no lo he explorado
            if(!cerrados.contains(child)) {
                // Expando esta rama
                cerrados.add(child);                            // Lo marco como explorado
                if (DFS_Search(child, target)) return true;     // Sigo la búsqueda
            }
        }
        
        // No se ha encontrado solución
        return false;
    }

    // Reconstruye el plan a partir del nodo final
    public void rebuildPlan(Node u) {
        ArrayDeque<Node> nodeRoute = new ArrayDeque<>();
        Node tmp = u;

        while(tmp.parent != null) {
            nodeRoute.add(tmp);
            plan.push(tmp.last_action);
            tmp = tmp.parent;
        }

        nAccionesPlan = plan.size();
    }
    
    @Override
    public ACTIONS act(StateObservation stateObs, ElapsedCpuTimer elapsedTimer) {
        // Calcular el plan solo una vez
        if (plan.isEmpty() && !DFS_Search(nodoInicial, nodoObjetivo)) {            
            updateT(elapsedTimer);
            return ACTIONS.ACTION_NIL;
        }

        // Ejecutar el plan
        if (!plan.isEmpty()) {
            ACTIONS m = plan.poll();
            updateT(elapsedTimer);
            return m;
        }

        updateT(elapsedTimer);
        return ACTIONS.ACTION_NIL;
    }
    
    @Override 
    public void result(StateObservation stateObservation, ElapsedCpuTimer elapsedCpuTimer) {
        MetricsProvider.getInstance().setNumAccionesPlan(nAccionesPlan);
        MetricsProvider.getInstance().setProfundidadMaxima(profundidadMaxima);
        MetricsProvider.getInstance().setNodosExpandidos(nodosExplorados);
        MetricsProvider.getInstance().setTiempoMilisegundos(getTms());
        MetricsProvider.getInstance().printMetrics();
    }
}
