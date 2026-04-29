package bd;

import java.sql.Connection;
import java.sql.DriverManager;
import java.util.Properties;
import java.io.InputStream;
import java.sql.SQLException;

public class Conexion {

    private static String url;
    private static String user;
    private static String pass;

    static {
        try {
            Properties props = new Properties();
            InputStream input = Conexion.class.getClassLoader().getResourceAsStream("config.properties");
            props.load(input);

            String host = props.getProperty("db.host");
            String port = props.getProperty("db.port");
            String service = props.getProperty("db.service");

            url = "jdbc:oracle:thin:@//" + host + ":" + port + "/" + service;
            user = props.getProperty("db.username");
            pass = props.getProperty("db.password");

            Class.forName("oracle.jdbc.driver.OracleDriver");

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static Connection conectar() throws Exception {
        
        // Establezco las propiedades de la conexión
        Properties props = new Properties();
        props.put("user", user);
        props.put("password", pass);

        // Tiempo máximo en milisegundos para I/O en cualquier sentencia
        props.put("oracle.jdbc.ReadTimeout", "5000");  // 5 segundos

        // Tiempo máximo para conectar al servidor
        props.put("oracle.net.CONNECT_TIMEOUT", "5000"); // 5 segundos

        Connection con = DriverManager.getConnection(url, props);
        
        if(con!=null) {
            System.out.println("Conexión exitosa a Oracle");
            
            // Desactivo el commit 
            con.setAutoCommit(false);
        }
        
        return con;
    }
    
    public static void desconectar(Connection con) throws Exception {
        try {
            // hago un ping al servidor con un tiempo maximo de 5 segundos para comprobar la conexion
            if (con != null && con.isValid(5)) {
                con.close();
                System.out.println("Conexión cerrada correctamente");
            } else {
                System.out.println("No se ha podido contactar con el servidor");
                throw new Exception("No se ha podido contactar con el servidor");
            }
        } catch (SQLException e) {
            throw new Exception("Error al cerrar la conexión", e);
        }
    }
}