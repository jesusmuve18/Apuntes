package tracks.singlePlayer.evaluacion.src_MUNOZ_VELASCO_JESUS;

import java.util.ArrayDeque;
import java.util.HashMap;
import java.util.ArrayList;
import java.util.PriorityQueue;
import java.util.Queue;
import java.util.Comparator;

import core.game.StateObservation;
import ontology.Types.ACTIONS;
import tools.ElapsedCpuTimer;
import tracks.singlePlayer.MetricsProvider;

// Agente que implementa el Algoritmo A*
public class AgenteAStar extends Agente {

    // Variables de estado  

    /*  Para "abiertos" se ha elegido un priorityQueue con la prioridad especificada en el guión
        (f(n) -> h(n) -> antigUedad) ya que introduce a los nodos en orden de forma óptima y es 
        fácil saber cuál es el mejor (el primero) en tiempo O(1).

        "mapaAbiertos" es una estructura paralela (mapa Hash) cuyo objetivo principal es la consulta
        sobre si un nodo se ha visitado o no (para saber si se ha encontrado un camino mejor). Utilizo
        esta estructura porque la búsqueda es O(1) y guarda para cada nodo el mejor camino por el momento.

        "cerrados" es un historial de los nodos ya explorados. Al igual que el caso anterior, esta
        estructura hace la búsqueda de forma óptima (O(1)).

        "plan" es una pila ya que para reconstruirla se empieza desde el nodo final hasta el inicial
        y luego hay que invertirla (al ser una pila esta inversión es natural).
    
    */
    Queue<Node> abiertos;                       // Cola con prioridad de abiertos
    HashMap<Node, Node> mapaAbiertos;           // Mapa Hash paralelo a abiertos
    HashMap<Node, Node> cerrados;               // Tabla hash de cerrados (para búsqueda y recuperación rápida)
    ArrayDeque<ACTIONS> plan;                   // Pila del plan

    // Métricas
    int nodosExplorados;
    int n_acciones_plan;

    // Índice para la antigüedad de los nodos (se actualiza cada vez que se inserta un nodo en la prioritiQueue)
    int n;

    // Constructor
    public AgenteAStar(StateObservation stateObs, ElapsedCpuTimer elapsedTimer) {
        super(stateObs, elapsedTimer);

        // Defino abiertos como una lista con prioridad según la heurística
        abiertos = new PriorityQueue<>(new Comparator<Node>() {
            @Override
            public int compare(Node n1, Node n2) {
                // Comparo por f(n) = g(n) + h(n)
                int f1 = n1.actual_cost + n1.estimated_cost;
                int f2 = n2.actual_cost + n2.estimated_cost;
                int res = Integer.compare(f1, f2);

                if(res != 0) return res;

                // En caso de empate de f(n), desempatar por h(n)
                res = Integer.compare(n1.estimated_cost, n2.estimated_cost);
                if(res != 0) return res;

                // En caso de empate de h(n), desempatar por antigüedad
                return Integer.compare(n1.id, n2.id);
            }
        });

        // Inicializo las estructuras
        mapaAbiertos = new HashMap<>();
        cerrados = new HashMap<>();
        plan = new ArrayDeque<>();

        // Inicializo el índice de nodos
        n=0;
        
        // Introduzco el nodo inicial
        nodoInicial.id = n++;
        nodoInicial.updateEstimatedCostTo(nodoObjetivo);

        // Inicializo la lista de abiertos y cerrados
        abiertos.add(nodoInicial);
        mapaAbiertos.put(nodoInicial, nodoInicial);
        cerrados.clear();

        // Inicializo las métricas
        nodosExplorados = 0;
        n_acciones_plan = 0;
    }

    // Método A*
    public boolean AStar() {
        
        Node actual;
        ArrayList<Node> sucesores;

        // Mientras haya nodos abiertos
        while(!abiertos.isEmpty()) {

            // Saco el mejor candidato
            actual = abiertos.poll();

            // Si el coste del nodo que acaba de salir es mayor que el que tenemos en el mapaAbiertos lo ignoramos
            if (mapaAbiertos.get(actual).isBetterThan(actual)) {
                continue; 
            }

            // Comprobamos si es objetivo
            if(isTarget(actual)) {
                rebuildPlan(actual); 
                return true;
            }

            // Actualizo las métricas
            nodosExplorados++;

            // Lo meto en cerrados (lo marco como explorado)
            cerrados.put(actual, actual);
            
            // Genero los sucesores del nodo actual
            sucesores = successorsOf(actual);

            for (Node sucesor : sucesores) {
                // Actualizo el sucesor
                sucesor.id = n++;
                sucesor.updateEstimatedCostTo(nodoObjetivo);
                
                if (cerrados.containsKey(sucesor) && sucesor.isBetterThan(cerrados.get(sucesor))) {
                    // Si ya lo he visitado y es mejor
                    cerrados.remove(sucesor);                       // Lo marco como no visitado
                    abiertos.add(sucesor);                          // Lo meto en abiertos
                    mapaAbiertos.put(sucesor, sucesor);             // Actualizo el mejor camino

                } else if (!cerrados.containsKey(sucesor) && !mapaAbiertos.containsKey(sucesor)){
                    // Si no lo he visitado lo meto directamente
                    abiertos.add(sucesor);
                    mapaAbiertos.put(sucesor, sucesor);

                } else if (mapaAbiertos.containsKey(sucesor) && sucesor.isBetterThan(mapaAbiertos.get(sucesor))) { 
                    // Si es mejor que el que está en abiertos
                    abiertos.remove(mapaAbiertos.get(sucesor));     // Lo elimino de abiertos
                    abiertos.add(sucesor);                          // Introduzco el que era mejor
                    mapaAbiertos.put(sucesor, sucesor);             // Lo actualizo en el mapa
                }
            }
        }

        // Se ha agotado la lista de abiertos y no se ha encotrado un objetivo
        return false;
    }

    // Reconstruye el plan a partir del nodo final
    public void rebuildPlan(Node u) {
        ArrayDeque<Node> nodeRoute = new ArrayDeque<>();
        Node tmp = u;

        // Voy metiendo las acciones en la pila enlazando con el padre de cada nodo
        while(tmp.parent != null) {
            nodeRoute.add(tmp);
            plan.push(tmp.last_action);
            tmp = tmp.parent;
        }

        // Actualizo las métricas
        n_acciones_plan = plan.size();
    }
    
    @Override
    public ACTIONS act(StateObservation stateObs, ElapsedCpuTimer elapsedTimer) {
        // Calculamos el plan solo la primera vez
        if (plan.isEmpty() && !AStar()) {
            // Si no se encuentra solución
            updateT(elapsedTimer);
            return ACTIONS.ACTION_NIL;
        }

        // Ejecutamos el plan
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
        MetricsProvider.getInstance().setNumAccionesPlan(n_acciones_plan);
        MetricsProvider.getInstance().setNodosAbiertos(abiertos.size());
        MetricsProvider.getInstance().setNodosCerrados(cerrados.size());
        MetricsProvider.getInstance().setNodosExpandidos(nodosExplorados);
        MetricsProvider.getInstance().setTiempoMilisegundos(getTms());
        MetricsProvider.getInstance().printMetrics();
    }
}
