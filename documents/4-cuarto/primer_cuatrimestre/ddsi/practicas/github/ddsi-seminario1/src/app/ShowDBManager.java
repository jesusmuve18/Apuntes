package app;

import java.sql.Connection;
import java.sql.SQLException;
import java.sql.Statement;
import java.sql.ResultSet;

import java.util.Map;
import java.util.HashMap;

public class ShowDBManager {
    public static Map<String, String> mostrarTablas(Connection con) throws Exception{
        
        Map<String, String> tablas = new HashMap<>();
        
        try (Statement st = con.createStatement()) {

            // PEDIDO
            String tabla_pedido = String.format("%-10s %-10s %-12s%n", "Cpedido", "Ccliente", "Fecha_pedido");
            tabla_pedido += "--------------------------------------------------------\n";
            try (ResultSet rs = st.executeQuery(
                    "SELECT Cpedido, Ccliente, TO_CHAR(Fecha_pedido, 'DD-MM-YYYY') AS Fecha_pedido " +
                    "FROM Pedido ORDER BY Cpedido")) {
                while (rs.next()) {
                    tabla_pedido += String.format("%-10s %-10s %-12s%n",
                            rs.getString("Cpedido"),
                            rs.getString("Ccliente"),
                            rs.getString("Fecha_pedido"));
                }
            }
            
            tablas.put("pedido", tabla_pedido);

            // STOCK
            String tabla_stock = String.format("%-10s %10s%n", "Cproducto", "Cantidad");
            tabla_stock += "----------------------------------------\n";
            try (ResultSet rs = st.executeQuery(
                    "SELECT Cproducto, Cantidad FROM Stock ORDER BY Cproducto")) {
                while (rs.next()) {
                    tabla_stock += String.format("%-10s %10d%n",
                            rs.getString("Cproducto"),
                            rs.getInt("Cantidad"));
                }
            }
            
            tablas.put("stock", tabla_stock);

            // DETALLE_PEDIDO
            String tabla_detalle_pedido = String.format("%-10s %-10s %10s%n", "Cpedido", "Cproducto", "Cantidad");
            tabla_detalle_pedido += "-----------------------------------------------------\n";
            try (ResultSet rs = st.executeQuery(
                    "SELECT Cpedido, Cproducto, Cantidad " +
                    "FROM Detalle_Pedido ORDER BY Cpedido, Cproducto")) {
                while (rs.next()) {
                    tabla_detalle_pedido += String.format("%-10s %-10s %10d%n",
                            rs.getString("Cpedido"),
                            rs.getString("Cproducto"),
                            rs.getInt("Cantidad"));
                }
            }
            
            tablas.put("detalle_pedido", tabla_detalle_pedido);

            System.out.println("Listado completo mostrado");

        } catch (SQLException e) {
            System.err.println("Error al mostrar tablas. "+e.getMessage());
            throw new Exception("Error al mostrar tablas. "+e.getMessage(), e);
        }
        
        return tablas;
    }
}
