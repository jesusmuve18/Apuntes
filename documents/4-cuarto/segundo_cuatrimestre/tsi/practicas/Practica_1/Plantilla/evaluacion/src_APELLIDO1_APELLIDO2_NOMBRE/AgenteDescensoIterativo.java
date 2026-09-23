package tracks.singlePlayer.evaluacion.src_APELLIDO1_APELLIDO2_NOMBRE;

import core.game.StateObservation;
import core.player.AbstractPlayer;
import ontology.Types.ACTIONS;
import tools.ElapsedCpuTimer;
import tracks.singlePlayer.MetricsProvider;

// Nota: Se puede heredar de otras clases personalizadas por el alumnado que hereden de AbstractPlayer para generalizar
// los elementos comunes a todos los algoritmos.

public class AgenteDescensoIterativo extends AbstractPlayer {

    public AgenteDescensoIterativo(StateObservation stateObs, ElapsedCpuTimer elapsedTimer) {
        super();

        // El constructor puede inicializar todas las estructuras iniciales y hasta el nodo inicial pero no puede hacer 
        // nada del proceso de búsqueda
    }

    @Override
    public ACTIONS act(StateObservation stateObs, ElapsedCpuTimer elapsedTimer) {
        // Este método debe encargarse de computar el plan y devolver la siguiente acción

        // Al encontrar el plan se deben fijar TODAS las métricas complementarias correspondientes al algoritmo
        MetricsProvider.getInstance().setNumAccionesPlan(-1);

        // ....
        // ....

        
        // Y se deben mostrar los resultados por pantalla mediante este método (sólo una vez al encontrar el plan)
        MetricsProvider.getInstance().printMetrics();

        return ACTIONS.ACTION_NIL;
    }
    
}
