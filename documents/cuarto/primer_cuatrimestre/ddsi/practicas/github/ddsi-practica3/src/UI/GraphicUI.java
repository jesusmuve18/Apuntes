package UI;

import javax.swing.*;
import java.awt.*;
import java.sql.Connection;

import bd.Conexion;

public class GraphicUI {
    
    /**
     * Inicializa la interfaz gráfica una vez establecida la conexion
     * @param args 
     */
    public static void main(String[] args) {
        
        // Establezco un estilo para la interfaz (estilo de sistema)
        try {
            UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
        } catch (Exception ignored) {}
        
        Connection con = null;
        
        try {
            // Me conecto al servidor
            con = Conexion.conectar();
            
            // Informo del resultado
            Informar(
                    "Resultado",
                    "Conexión exitosa con el servidor"
            );
            
            // Muestro el menu principal
            MainMenu(con);
        } catch (Exception e) {
            // Informo del error
            InformarError(e);
        }        
    }
    
    /**
     * Menú principal de la aplicacion
     * @param con Conexion abierta con el servidor
     * 
     * Mostrara 5 botones:
     * 1. Subsistema de gestion de ligas
     * 2. Subsistema de gestion de equipos
     * 3. Subsistema de gestion de partidos
     * 4. Subsistema de gestion de jugadores
     * 5. Subsistema de gestion de personal
     */
    
    public static JFrame main_menu_frame;
    
    public static void MainMenu(Connection con) {
        
        if(main_menu_frame==null) {
            // Creamos la ventana
            main_menu_frame = new JFrame("Menú principal");
            main_menu_frame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
            main_menu_frame.setLayout(new GridLayout(5, 1)); // 5 filas, 1 columna
            main_menu_frame.setSize(400, 300);
            main_menu_frame.setLocationRelativeTo(null); // Centra la ventana
            
            // Creamos los botones
            JButton boton_ligas = new JButton("Gestion de ligas");
            JButton boton_equipos = new JButton("Gestion de equipos");
            JButton boton_partidos = new JButton("Gestion de partidos");
            JButton boton_jugadores = new JButton("Gestion de jugadores");
            JButton boton_personal = new JButton("Gestion de personal");

            // Añadimos los botones a la ventana
            main_menu_frame.add(boton_ligas);
            main_menu_frame.add(boton_equipos);
            main_menu_frame.add(boton_partidos);
            main_menu_frame.add(boton_jugadores);
            main_menu_frame.add(boton_personal);
            
            // Defino el comportamiento de cada boton
            boton_ligas.addActionListener(e -> {
                UI.LigasUI.MainMenu(main_menu_frame);
                main_menu_frame.setVisible(false);
            });

            boton_equipos.addActionListener(e -> {
                UI.EquiposUI.MainMenu(main_menu_frame);
                main_menu_frame.setVisible(false);
            });

            boton_partidos.addActionListener(e -> {
                UI.PartidosUI.MainMenu(main_menu_frame);
                main_menu_frame.setVisible(false);
            });

            boton_jugadores.addActionListener(e -> {
                UI.JugadoresUI.MainMenu(main_menu_frame);
                main_menu_frame.setVisible(false);
            });
            
            boton_personal.addActionListener(e -> {
                UI.PersonalUI.MainMenu(main_menu_frame);
                main_menu_frame.setVisible(false);
            });
            
            // Deino el comportamiento al cerrar la ventana
            main_menu_frame.addWindowListener(new java.awt.event.WindowAdapter() {            
                @Override
                public void windowClosed(java.awt.event.WindowEvent e) {
                    // Cerrado con la X o con dispose()
                    try {
                        Conexion.desconectar(con);
                        
                        // Informo del resultado
                        Informar(
                        "Resultado",
                                "Se ha cerrado correctamente la conexión"
                        );
                        
                        // Cierro todas las ventanas
                        System.exit(0);
                    } catch (Exception ex) {
                        InformarError(ex);
                    }
                }
            });
            
        }

        main_menu_frame.setVisible(true);
    }
    
    
    /**
     * Ventana para introducir los detalles del pedido
     * @param parent Frame de la ventana padre (solo para la posición relativa)
     * @param con Conexion abierta con el servidor
     * 
     * Mostrara 2 campos rellenables:
     * 1. Codigo producto
     * 2. Codigo cliente
     * 
     * y un boton:
     * - Añadir
     * 
     * Calculara la fecha en el momento de pulsar el boton
     */
    public static int int_detalles_pedido_x = -1, int_detalles_pedido_y=-1;
    public static int int_detalles_pedido_w = -1, int_detalles_pedido_h = -1;
    
    public static void  IntroducirDetallesPedido(Frame parent, Connection con) {
        // Creamos la ventana
        JFrame frame = new JFrame("Detalles del pedido");
        
        frame.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
        frame.setLayout(new GridBagLayout());
        GridBagConstraints gbc = new GridBagConstraints();
        
        if(int_detalles_pedido_w >0 && int_detalles_pedido_h>0) {
            frame.setSize(int_detalles_pedido_w, int_detalles_pedido_h);
        } else {
            frame.setSize(300, 200);
        }
        
        if(int_detalles_pedido_x >0 && int_detalles_pedido_y>0) {
            frame.setLocation(int_detalles_pedido_x,int_detalles_pedido_y);
        } else {
            frame.setLocationRelativeTo(parent); // Centra la ventana
        }
        
        
        gbc.insets = new Insets(10, 10, 10, 10); // márgenes opcionales

        // Codigo pedido
        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.weightx = 0;
        frame.add(new JLabel("Código pedido:"), gbc);
        gbc.gridx = 1;
        gbc.weightx = 1;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        JTextField txtCpedido =new JTextField(10);
        frame.add(txtCpedido, gbc);
        
        // Codigo cliente
        gbc.gridx = 0;
        gbc.gridy = 1;
        gbc.weightx = 0;
        frame.add(new JLabel("Código cliente:"), gbc);
        gbc.gridx = 1;
        gbc.weightx = 1;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        JTextField txtCcliente =new JTextField(10);
        frame.add(txtCcliente, gbc);
        
        // Aceptar
        gbc.gridx = 0;
        gbc.gridy = 2;
        gbc.gridwidth = 2;
        gbc.anchor = GridBagConstraints.CENTER;
        JButton botonGuardar = new JButton("Añadir");
        frame.add(botonGuardar, gbc);
        
        frame.setVisible(true);
        
        // Comportamiento boton
        botonGuardar.addActionListener(e -> {
            
            
            // Cierro esta ventana
            frame.dispose();
        });
        
        // Comportamiento de cerrar ventana
        frame.addWindowListener(new java.awt.event.WindowAdapter() {            
            @Override
            public void windowClosed(java.awt.event.WindowEvent e) {                
                // Guardar posición y tamaño actuales
                int_detalles_pedido_x = frame.getX();
                int_detalles_pedido_y = frame.getY();
                int_detalles_pedido_w = frame.getWidth();
                int_detalles_pedido_h = frame.getHeight();
            }
            
            @Override
            public void windowClosing(java.awt.event.WindowEvent e) {                
                // Cerrado con X
                
                // Vuelvo al menú principal
                MainMenu(con);
                
                // Cierro la ventana
                frame.dispose();
            }
        });
    }
    
    /**
     * Ventana para mostrar las tablas creadas
     * @param parent Frame de la ventana padre (solo se usa para la posición de la ventana)
     * @param tabla_stock Cadena con el contenido de la tabla STOCK
     * @param tabla_pedido Cadena con el contenido de la tabla PEDIDO
     * @param tabla_detalle_pedido Cadena con el contenido de la tabla DETALLE_PEDIDO
     * 
     * Mostrara 3 columnas, una por cada tabla con:
     * 1. Nombre de la tabla
     * 2. Cabecera y contenido de la tabla 
     * 
     * Además, si la ventana se abre cuando ya estaba abierta tan solo modificará su contenido
     * (no se abrirá de nuevo)
     */
    
    // Variables estáticas para reutilizar la ventana de DisplayTablas
    private static JFrame frameTablas = null;
    private static JTextArea txt_stock; 
    private static JTextArea txt_pedido;
    private static JTextArea txt_detalle_pedido;
    
    public static int display_tablas_x = -1, display_tablas_y=-1;
    public static int display_tablas_w = -1, display_tablas_h = -1;
    
    public static void DisplayTablas(JFrame parent, String tabla_stock, String tabla_pedido, String tabla_detalle_pedido) {

        // Si ya existe la ventana actualiza los textos
        if (frameTablas != null && frameTablas.isShowing()) {
            txt_stock.setText(tabla_stock);
            txt_pedido.setText(tabla_pedido);
            txt_detalle_pedido.setText(tabla_detalle_pedido);
            frameTablas.toFront();
            return;
        }

        // Si no existe se crea
        frameTablas = new JFrame("Visualización de tablas");
        frameTablas.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        
        if(display_tablas_w >0 && display_tablas_h>0) {
            frameTablas.setSize(display_tablas_w, display_tablas_h);
        } else {
            frameTablas.setSize(850, 400);
        }
        
        if(display_tablas_x >0 && display_tablas_y>0) {
            frameTablas.setLocation(display_tablas_x,display_tablas_y);
        } else {
            if (parent != null) {
                frameTablas.setLocation(parent.getX() + parent.getWidth() + 10, parent.getY());
            } else {
                frameTablas.setLocationRelativeTo(null);
            }
        }  

        // Cuando se cierre, liberar referencia
        frameTablas.addWindowListener(new java.awt.event.WindowAdapter() {
            @Override
            public void windowClosed(java.awt.event.WindowEvent e) {
                // Guardar posición y tamaño actuales
                display_tablas_x = frameTablas.getX();
                display_tablas_y = frameTablas.getY();
                display_tablas_w = frameTablas.getWidth();
                display_tablas_h = frameTablas.getHeight();
                
                frameTablas = null;
            }
        });

        frameTablas.setLayout(new GridBagLayout());
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.insets = new Insets(10, 10, 10, 10);

        // Títulos
        gbc.gridy = 0;
        gbc.fill = GridBagConstraints.NONE;
        gbc.anchor = GridBagConstraints.CENTER;
        gbc.weighty = 0;

        gbc.gridx = 0;
        gbc.weightx = 1.0;
        frameTablas.add(new JLabel("Stock", SwingConstants.CENTER), gbc);

        gbc.gridx = 1;
        frameTablas.add(new JLabel("Pedidos", SwingConstants.CENTER), gbc);

        gbc.gridx = 2;
        frameTablas.add(new JLabel("Detalle Pedido", SwingConstants.CENTER), gbc);

        // Tablas
        gbc.gridy = 1;
        gbc.fill = GridBagConstraints.BOTH;
        gbc.anchor = GridBagConstraints.CENTER;
        gbc.weighty = 1.0;

        // STOCK
        gbc.gridx = 0;
        txt_stock = new JTextArea(tabla_stock);
        txt_stock.setEditable(false);
        frameTablas.add(new JScrollPane(txt_stock), gbc);

        // PEDIDOS
        gbc.gridx = 1;
        txt_pedido = new JTextArea(tabla_pedido);
        txt_pedido.setEditable(false);
        frameTablas.add(new JScrollPane(txt_pedido), gbc);

        // DETALLE PEDIDO
        gbc.gridx = 2;
        txt_detalle_pedido = new JTextArea(tabla_detalle_pedido);
        txt_detalle_pedido.setEditable(false);
        frameTablas.add(new JScrollPane(txt_detalle_pedido), gbc);

        frameTablas.setVisible(true);
    }
    
    /**
     * Muestra el contenido de las tablas (hace la solicitud SQL y la muestra con la ventana DisplayTablas)
     * @param parent Frame de la ventana padre (solo se usa para la posición de la ventana)
     * @param con Conexión activa con la base de datos
     */
    public static void MostrarContenidoTablas(JFrame parent, Connection con) {
        // Muestro el contenido de las tablas
        try{
            /*Map<String,String> tablas=ShowDBManager.mostrarTablas(con);  
             DisplayTablas(
                    parent,
                    tablas.get("stock"), 
                    tablas.get("pedido"), 
                    tablas.get("detalle_pedido")
            );*/
        } catch (Exception ex) {
            // Informo del error
            InformarError(ex);
        }
    }
    
    /**
     * Ventana para mostrar una pestaña de información
     * @param title Título de la ventana
     * @param message Mensaje que se muestra en la ventana
     */
    public static void Informar(String title, String message) {
        JOptionPane.showMessageDialog(
            null,
            message,
            title,
            JOptionPane.INFORMATION_MESSAGE
        );
    }
    
    /**
     * Ventana para mostrar una pestaña de error provocado por una excepción
     * @param ex Excepción de la que se desea informar
     */
    public static void InformarError(Exception ex) {
        JOptionPane.showMessageDialog(
            null,
            "Error: "+ex.getMessage(),
            "Error",
            JOptionPane.ERROR_MESSAGE
        );
    }
}

