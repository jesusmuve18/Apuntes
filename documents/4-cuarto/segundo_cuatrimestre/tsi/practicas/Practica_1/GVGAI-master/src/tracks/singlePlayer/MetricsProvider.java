package tracks.singlePlayer;

public class MetricsProvider {

    private static MetricsProvider instance;

    private Integer numAccionesPlan;
    private Integer nodosExpandidos;
    private Integer profundidadMaxima;
    private Integer nodosAbiertos;
    private Integer nodosCerrados;
    private Integer numActualizacionesTabla;
    private Integer lvl;
    private String agente;

    private Long tiempoMilisegundos;

    private MetricsProvider() {}


    // Obtención del singleton
    public static MetricsProvider getInstance() {
        if (instance == null) {
            instance = new MetricsProvider();
        }
        return instance;
    }

    // --- SETTERS ---

    public void setNumAccionesPlan(int numAccionesPlan) {
        this.numAccionesPlan = numAccionesPlan;
    }

    public void setNodosExpandidos(int nodosExplorados) {
        this.nodosExpandidos = nodosExplorados;
    }

    public void setProfundidadMaxima(int profundidad) {
        this.profundidadMaxima = profundidad;
    }

    public void setNodosAbiertos(int nodosAbiertos) {
        this.nodosAbiertos = nodosAbiertos;
    }

    public void setNodosCerrados(int nodosCerrados) {
        this.nodosCerrados = nodosCerrados;
    }

    public void setTiempoMilisegundos (long tiempoMilisegundos) {
        this.tiempoMilisegundos = tiempoMilisegundos;
    }
    
    public void setAgente(String agent) {
        this.agente = agent;
    }

    public void setMapa(int mapa) {
        this.lvl = mapa;
    }

    public void setNumActualizacionesTabla(int numActualizacionesTabla) {
        this.numActualizacionesTabla = numActualizacionesTabla;
    }


    /**
     * Imprime todas las métricas fijadas hasta el momento
     */
    public void printMetrics() {
        System.out.println("------ MÉTRICAS DE EJECUCIÓN ------");
        if (agente != null)
            System.out.println("Agente: " + agente);

        if (lvl != null)
            System.out.println("Mapa: " + lvl);
        
        if (numAccionesPlan != null) 
            System.out.println("N.º de acciones del plan: " + numAccionesPlan);
        
        if (nodosExpandidos != null) 
            System.out.println("Nodos explorados:         " + nodosExpandidos);
            
        if (profundidadMaxima != null) 
            System.out.println("Profundidad:              " + profundidadMaxima);
            
        if (nodosAbiertos != null) 
            System.out.println("Nodos en abiertos:        " + nodosAbiertos);
            
        if (nodosCerrados != null) 
            System.out.println("Nodos en cerrados:        " + nodosCerrados);

        if (numActualizacionesTabla != null) 
            System.out.println("N.º actualizaciones Tabla H: " + numActualizacionesTabla);

        if (tiempoMilisegundos != null)
            System.out.println("Tiempo ejecución (ms):        " + tiempoMilisegundos);
            
        System.out.println("-----------------------------------");
    }

    /**
     * Limpia las métricas para una nueva ejecución.
     */
    public void reset() {
        this.numAccionesPlan = null;
        this.nodosExpandidos = null;
        this.profundidadMaxima = null;
        this.nodosAbiertos = null;
        this.nodosCerrados = null;
		this.numActualizacionesTabla = null;
        this.tiempoMilisegundos = null;
    }
}