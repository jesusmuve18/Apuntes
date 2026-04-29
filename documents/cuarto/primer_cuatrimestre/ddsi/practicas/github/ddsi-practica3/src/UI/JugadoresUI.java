package UI;

// Paquetes para interfaz
import javax.swing.*;
import java.awt.*;

// Paquetes para funcionalidad
import app.Jugadores;

public class JugadoresUI {
   
    public static JFrame main_menu_frame;
    
    /**
     * Menú principal del gestor de Jugadores
     * @param parent Ventana a la que volver cuando se cierre esta
     * 
     * Mostrara 5 botones:
     * 1. Dar de Alta un jugador
     * 2. Dar de Baja un jugador
     * 3. Modificar los datos de un jugador
     * 4. Consultar estadisticas de jugador
     * 5. Consultar informacion de jugador
     */   
    public static void MainMenu(JFrame parent) {
        
        if(main_menu_frame==null) {
            // Creamos la ventana
            main_menu_frame = new JFrame("Subsistema Gestion de Jugadores");
            main_menu_frame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
            main_menu_frame.setLayout(new GridLayout(5, 1)); // 5 filas, 1 columna
            main_menu_frame.setSize(400, 300);
            main_menu_frame.setLocationRelativeTo(null); // Centra la ventana
            
            // Creamos los botones
            JButton boton1 = new JButton("Dar de Alta un jugador");
            JButton boton2 = new JButton("Dar de Baja un jugador");
            JButton boton3 = new JButton("Modificar los datos de un jugador");
            JButton boton4 = new JButton("Consultar estadisticas de jugador");
            JButton boton5 = new JButton("Consultar informacion de jugador");

            // Añadimos los botones a la ventana
            main_menu_frame.add(boton1);
            main_menu_frame.add(boton2);
            main_menu_frame.add(boton3);
            main_menu_frame.add(boton4);
            main_menu_frame.add(boton5);
            
            // Defino el comportamiento de cada boton
            boton1.addActionListener(e -> {
                UI.GraphicUI.Informar(
                        "No implementado", 
                        "Funcionalidad aun no implementada");
            });

            boton2.addActionListener(e -> {
                UI.GraphicUI.Informar(
                        "No implementado", 
                        "Funcionalidad aun no implementada");
            });

            boton3.addActionListener(e -> {
                UI.GraphicUI.Informar(
                        "No implementado", 
                        "Funcionalidad aun no implementada");
            });

            boton4.addActionListener(e -> {
                UI.GraphicUI.Informar(
                        "No implementado", 
                        "Funcionalidad aun no implementada");
            });
            
            boton5.addActionListener(e -> {
                UI.GraphicUI.Informar(
                        "No implementado", 
                        "Funcionalidad aun no implementada");
            });
            
            // Deino el comportamiento al cerrar la ventana
            main_menu_frame.addWindowListener(new java.awt.event.WindowAdapter() {            
                @Override
                public void windowClosed(java.awt.event.WindowEvent e) {
                    // Cerrado con la X o con dispose()
                    main_menu_frame.setVisible(false);
                    parent.setVisible(true);
                }
            });
            
        }

        main_menu_frame.setVisible(true);
    }
}
