package UI;

import javax.swing.*;
import java.awt.*;

import java.time.LocalDate;

import java.util.Map;

import app.Initializer;
import app.OrdersManager;
import app.ShowDBManager;

import bd.Conexion;

import java.sql.Connection;

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
     * Mostrara 4 botones:
     * 1. Borrado y nueva creación de las tablas e inserción de 10 tuplas 
     *    predefinidas en el código en la tabla Stock.
     * 2. Dar de alta nuevo pedido
     * 3. Mostrar contenido de las tablas de la BD
     * 4. Salir del programa y cerrar conexión a BD
     */
    public static int main_menu_x = -1, main_menu_y=-1;
    public static int main_menu_w = -1, main_menu_h = -1;
    
    public static void MainMenu(Connection con) {
        // Creamos la ventana
        JFrame frame = new JFrame("Menú principal");
        frame.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
        frame.setLayout(new GridLayout(4, 1)); // 4 filas, 1 columna
        
        if(main_menu_w >0 && main_menu_h>0) {
            frame.setSize(main_menu_w, main_menu_h);
        } else {
            frame.setSize(400, 300);
        }
        
        if(main_menu_x >0 && main_menu_y>0) {
            frame.setLocation(main_menu_x,main_menu_y);
        } else {
            frame.setLocationRelativeTo(null); // Centra la ventana
        }

        // Creamos los botones directamente (no necesitas 4 paneles)
        JButton boton1 = new JButton("Borrado, creación e inserción inicial");
        JButton boton2 = new JButton("Dar de alta nuevo pedido");
        JButton boton3 = new JButton("Mostrar contenido de las tablas");
        JButton boton4 = new JButton("Salir del programa y cerrar la conexión");

        // Añadimos los botones a la ventana
        frame.add(boton1);
        frame.add(boton2);
        frame.add(boton3);
        frame.add(boton4);
        
        frame.setVisible(true);
        
        // Acciones de cada botón
        boton1.addActionListener(e -> {
            try{
                // Inicializo la base de datos
                Initializer.init(con);
                
                // Si hay ventana de tablas la cierro
                if(frameTablas != null) {
                    frameTablas.dispose();
                }
                
                Informar(
                        "Resultado",
                        "Se ha inicializado correctamente la base de datos"
                );
            } catch (Exception ex) {
                // Informo del error
                InformarError(ex);
            }  
        });
        
        boton2.addActionListener(e -> {
            // Abro la ventana para introducir los detalles del pedido
            IntroducirDetallesPedido(frame, con);
            
            // Cierro esta ventana
            frame.dispose();
        });
        
        boton3.addActionListener(e -> {
            MostrarContenidoTablas(frame, con);
        });
        
        boton4.addActionListener(e -> {
            try{
                // Me desconecto del servidor
                Conexion.desconectar(con);
                
                // Informo del resultado
                Informar(
                "Resultado",
                        "Se ha cerrado correctamente la conexión"
                );
                
                System.exit(0);
            } catch (Exception ex){   
                // Informo del error
                InformarError(ex);
            }
            
            // Cierro la ventana
            frame.dispose();
        });        
        
        frame.addWindowListener(new java.awt.event.WindowAdapter() {            
            @Override
            public void windowClosed(java.awt.event.WindowEvent e) {
                // Cerrado con la X o con dispose()
                
                // Guardar posición y tamaño actuales
                main_menu_x = frame.getX();
                main_menu_y = frame.getY();
                main_menu_w = frame.getWidth();
                main_menu_h = frame.getHeight();
                
            }
        });
    }
    
    /**
     * Menú de pedidos
     * @param parent Frame de la ventana padre (solo para la posición relativa)
     * @param con Conexion abierta con el servidor
     * 
     * Mostrara 4 botones:
     * 1. Añadir detalle de producto
     * 2. Eliminar todos los detalles de producto
     * 3. Cancelar pedido
     * 4. Finalizar pedido
     */
    public static int menu_pedido_x = -1, menu_pedido_y=-1;
    public static int menu_pedido_w = -1, menu_pedido_h = -1;
    
    public static void MenuPedido(Frame parent, Connection con, String Cpedido) {
        JFrame frame = new JFrame("Dar de alta pedido - "+Cpedido);
        frame.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
        frame.setLayout(new GridLayout(4, 1)); // 4 filas, 1 columna
        
        if(menu_pedido_w >0 && menu_pedido_h>0) {
            frame.setSize(menu_pedido_w, menu_pedido_h);
        } else {
            frame.setSize(400, 300);
        }
        
        if(menu_pedido_x >0 && menu_pedido_y>0) {
            frame.setLocation(menu_pedido_x,menu_pedido_y);
        } else {
            frame.setLocationRelativeTo(parent); // Centra la ventana
        }

        // Creamos los botones directamente (no necesitas 4 paneles)
        JButton boton1 = new JButton("Añadir detalle de producto");
        JButton boton2 = new JButton("Eliminar todos los detalles de producto");
        JButton boton3 = new JButton("Cancelar pedido");
        JButton boton4 = new JButton("Finalizar pedido");

        // Añadimos los botones a la ventana
        frame.add(boton1);
        frame.add(boton2);
        frame.add(boton3);
        frame.add(boton4);
        
        frame.setVisible(true);
        
        // Acciones de cada botón
        boton1.addActionListener(e -> {             
            // Abro la ventana para introducir los detalles del producto
            IntroducirDetallesProducto(frame, con, Cpedido);
            
            // Cierro esta ventana
            frame.dispose();
        });
        
        boton2.addActionListener(e -> {
            
            // Elimino los detalles del pedido
            try{
                // Ejecuto la sentencia SQL
               OrdersManager.deleteDetallesPedido(con); 
               
               // Informo del resultado
               Informar(
                       "Resultado",
                       "Se han eliminado todos los detalles del pedido"
               );
               
               // Muestro el contenido de las tablas
                MostrarContenidoTablas(frame, con);
            } catch (Exception ex){
                InformarError(ex);
            }
        });
        
        boton3.addActionListener(e -> {
            
            // Cancelo el pedido
            try{
                // Ejecuto la sentencia SQL
                OrdersManager.cancelarPedido(con);
                
                // Informo del resultado
                Informar(
                        "Resultado",
                        "Se ha eliminado el pedido y todos sus detalles"
                );
                
                // Muestro el contenido de las tablas
                MostrarContenidoTablas(frame, con);
            } catch (Exception ex) {
                // Informo del error
                InformarError(ex);
            }

            // Cierro esta ventana
            frame.dispose();
            
            // Vuelvo al menu principal
            MainMenu(con);
        });
        
        boton4.addActionListener(e -> {
            
            // Guardar permanentemente los cambios
            try{
                // Ejecuto la sentencia SQL
               OrdersManager.finalizarPedido(con); 
               
               // Informo del resultado
               Informar(
                       "Resultado",
                       "Se han guardado permanentemente los cambios"
               );
            } catch (Exception ex) {
                // Informo del error
                InformarError(ex);
            }
            
            // Cierro esta ventana
            frame.dispose();
            
            // Vuelvo al menu principal
            MainMenu(con);
        });
        
        frame.addWindowListener(new java.awt.event.WindowAdapter() {
                        
            @Override
            public void windowClosed(java.awt.event.WindowEvent e) {
                // Cerrado con la X o con dispose() 
                
                // Guardar posición y tamaño actuales
                menu_pedido_x = frame.getX();
                menu_pedido_y = frame.getY();
                menu_pedido_w = frame.getWidth();
                menu_pedido_h = frame.getHeight();  
            }
        });
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
            LocalDate date = LocalDate.now();            
                        
            try {             
                // Ejecuto la sentencia SQL
                OrdersManager.addPedido(con, txtCpedido.getText(), txtCcliente.getText(), date );
                                
                // Muestro el contenido de las tablas
                MostrarContenidoTablas(frame, con);
                                
                // Informo del resultado
                Informar(
                    "Resultado",
                    "Se ha creado un pedido con código "+ txtCpedido.getText() +" para el cliente "+txtCcliente.getText() + " con fecha "+date
                );
                
                // Abro la nueva ventana
                MenuPedido(frame, con ,txtCpedido.getText());
                                
            } catch (Exception ex) {
                // Informo del error
                InformarError(ex);
                
                // Vuelvo al menú principal
                MainMenu(con);
            }
            
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
     * Ventana para introducir los detalles del producto
     * @param parent Frame de la ventana padre (solo para la posición relativa)
     * @param con Conexion abierta con el servidor
     * @param Cpedido Codigo del pedido al que se añadiran los productos
     * 
     * Mostrara 2 campos rellenables:
     * 1. Codigo producto
     * 2. Cantidad
     * 
     * y un boton:
     * - Añadir
     */
    public static int int_detalles_producto_x = -1, int_detalles_producto_y=-1;
    public static int int_detalles_producto_w = -1, int_detalles_producto_h = -1;
    
    public static void IntroducirDetallesProducto(Frame parent, Connection con, String Cpedido) {
        // Creamos la ventana
        JFrame frame = new JFrame("Añadir detalle de producto - Pedido "+Cpedido);
        
        frame.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
        frame.setLayout(new GridBagLayout());
        GridBagConstraints gbc = new GridBagConstraints();
        
        if(int_detalles_producto_w >0 && int_detalles_producto_h>0) {
            frame.setSize(int_detalles_producto_w, int_detalles_producto_h);
        } else {
            frame.setSize(400, 200);
        }
        
        if(int_detalles_producto_x >0 && int_detalles_producto_y>0) {
            frame.setLocation(int_detalles_producto_x,int_detalles_producto_y);
        } else {
            frame.setLocationRelativeTo(parent); // Centra la ventana
        }        
        
        gbc.insets = new Insets(10, 10, 10, 10); // márgenes opcionales

        // Codigo producto
        gbc.gridx = 0;
        gbc.gridy = 0;
        frame.add(new JLabel("Código producto:"), gbc);
        gbc.gridx = 1;
        gbc.weightx = 1;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        JTextField txtCproducto =new JTextField(10);
        frame.add(txtCproducto, gbc);
        
        // Cantidad
        gbc.gridx = 0;
        gbc.gridy = 1;
        gbc.weightx = 0;
        frame.add(new JLabel("Cantidad:"), gbc);
        gbc.gridx = 1;
        JSpinner spinner = new JSpinner(
            new SpinnerNumberModel(0, 0, 9999, 1) // valor inicial, mínimo, máximo, paso
        );
        spinner.setEditor(new JSpinner.NumberEditor(spinner, "#"));
        frame.add(spinner, gbc);
        
        // Guardar
        gbc.gridx = 0;
        gbc.gridy = 2;
        gbc.gridwidth = 2;
        gbc.anchor = GridBagConstraints.CENTER;
        JButton botonGuardar = new JButton("Añadir");
        frame.add(botonGuardar, gbc);
        
        frame.setVisible(true);
        
        // Comportamiento del boton
        botonGuardar.addActionListener(e -> {
            // Añado el pedido correspondiente
            try{
                // Ejecuto la sentencia SQL
                OrdersManager.addDetallePedido(con, Cpedido, txtCproducto.getText(), (int)spinner.getValue());
            
                // Informo del resultado
                Informar(
                        "Resultado",
                        "Se han añadido "+ (int)spinner.getValue() +" unidades del producto "+txtCproducto.getText()
                );
            } catch (Exception ex){
                // Informo del error
                InformarError(ex);
            }
            
            // Muestro el contenido de las tablas
            MostrarContenidoTablas(frame, con);
            
            // Abro el menú
            MenuPedido(parent, con, Cpedido);
            
            // Cierro esta ventana
            frame.dispose();
        });
        
        // Comportamiento de cerrar ventana
        frame.addWindowListener(new java.awt.event.WindowAdapter() {
            
            @Override
            public void windowClosed(java.awt.event.WindowEvent e) {
                // Cerrado con X o con dispose()
                
                // Guardar posición y tamaño actuales
                int_detalles_producto_x = frame.getX();
                int_detalles_producto_y = frame.getY();
                int_detalles_producto_w = frame.getWidth();
                int_detalles_producto_h = frame.getHeight();
            }
            
            @Override
            public void windowClosing(java.awt.event.WindowEvent e) {
                // Cerrado con X
                
                // Abro el menú
                MenuPedido(parent, con, Cpedido);

                // Cierro esta ventana
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
            Map<String,String> tablas=ShowDBManager.mostrarTablas(con);  
             DisplayTablas(
                    parent,
                    tablas.get("stock"), 
                    tablas.get("pedido"), 
                    tablas.get("detalle_pedido")
            );
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

