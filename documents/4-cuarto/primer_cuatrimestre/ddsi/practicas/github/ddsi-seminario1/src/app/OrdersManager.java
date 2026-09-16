package app;

import java.sql.Connection;
import java.sql.SQLException;
import java.sql.Statement;

import java.time.LocalDate;
import java.sql.ResultSet;
import java.time.format.DateTimeFormatter;

public class OrdersManager {
    
    // Inserta un nuevo pedido en la tabla de pedido con los datos suministrados
    public static void addPedido(Connection con, String Cpedido, String Ccliente, LocalDate date) throws Exception{
        String fecha = date.format(DateTimeFormatter.ofPattern("dd-MM-yyyy"));
        
        try (Statement st = con.createStatement()) {
            try { 
                st.executeUpdate(
                        "INSERT INTO Pedido VALUES ('" + Cpedido + "','" + Ccliente +
                                "', TO_DATE('" + fecha + "', 'DD-MM-YYYY'))"
                ); 
            } catch (Exception e) {
                throw new Exception(e.getMessage(),e);
            }
            try { 
                st.execute("SAVEPOINT creacion_pedido"); 
            } catch (Exception e) {
                throw new Exception("Fallo creando el savepoint \"creacion_pedido\": "+e.getMessage(),e);
            }
            
            System.out.println("Se ha creado un nuevo pedido con codigo "+Cpedido+", cliente "+Ccliente+" y fecha "+fecha);

        } catch (SQLException e) {
            throw new Exception("Fallo creando el nuevo pedido con codigo "+Cpedido+", cliente "+Ccliente+" y fecha "+fecha+". "+e.getMessage(),e);
        } catch (Exception e) {
            throw new Exception("Fallo creando el nuevo pedido con codigo "+Cpedido+", cliente "+Ccliente+" y fecha "+fecha+". "+e.getMessage(),e);
        }
    }
    
    // Añade un detalle de pedido con los datos introducidos
    public static void addDetallePedido(Connection con, String Cpedido ,String Cproducto, int Cantidad) throws Exception{
        
        if (Cantidad <= 0) {
            throw new Exception(
        "Fallo creando el detalle de pedido: La cantidad introducida no es válida (debe ser un número mayor que 0)"
            );
        }
        
        try (Statement st = con.createStatement()) {
            
            // Leo la cantidad disponible
            ResultSet cantidad = st.executeQuery("SELECT cantidad FROM Stock WHERE Cproducto = '" + Cproducto +"'");
            cantidad.next();
            int cantidad_disponible = cantidad.getInt("Cantidad");
          
            System.out.println("Cantidad disponible: " + cantidad_disponible);
            
            if(Cantidad <= cantidad_disponible){
                // Si hay suficiente Stock
                
                // Guardo el estado inicial por si falla la transaccion
                st.execute("SAVEPOINT detalle_pedido");
                
                
                try{
                    // Actualizar Stock disponible
                    st.executeUpdate(
                        "UPDATE Stock SET cantidad = cantidad - " + Cantidad + " WHERE Cproducto = '" + Cproducto +"'" 
                    ); 
                    
                    // Compruebo si existe algúna entrada con el mismo producto y pedido
                    ResultSet rs = st.executeQuery("SELECT COUNT(*) FROM Detalle_Pedido WHERE Cproducto='" + Cproducto + "' AND Cpedido = '" + Cpedido + "'" );
                    rs.next();
                    
                    if (rs.getInt(1) > 0)
                        // Si ya existía en el pedido
                        st.executeUpdate("UPDATE Detalle_Pedido SET cantidad=cantidad+" + Cantidad + " WHERE Cproducto='" + Cproducto + "' AND Cpedido = '" + Cpedido + "'");
                    else
                        // Si es la primera vez que se añade
                        st.executeUpdate("INSERT INTO Detalle_Pedido (Cpedido, Cproducto, Cantidad) VALUES ('" + Cpedido + "', '" + Cproducto + "', " + Cantidad + ")");
                    
                    rs.close();
                    
                } catch (Exception e) {
                    st.execute("ROLLBACK TO detalle_pedido"); // se gestionaria con disparadores
                    throw new Exception(e.getMessage(),e);
                }

            } else {
                // Si no hay suficiente Stock
                throw new Exception("No hay suficiente stock");
            }
                                  
            
            System.out.println(
                "Se ha creado un nuevo detalle pedido con codigo "+Cpedido+", producto "+Cproducto+" y cantidad "+Cantidad
            );

        } catch (SQLException e) {
            throw new Exception("Fallo creando el nuevo detalle pedido con codigo "+Cpedido+", producto "+Cproducto+" y cantidad "+Cantidad+": "+e.getMessage(),e);
        } catch (Exception e) {
            throw new Exception("Fallo creando el nuevo detalle pedido con codigo "+Cpedido+", producto "+Cproducto+" y cantidad "+Cantidad+": "+e.getMessage(),e);
        }
    }
    
    // Elimina todos los detalles del pedido
    public static void deleteDetallesPedido(Connection con) throws Exception{
        try (Statement st = con.createStatement()) {
            st.execute("ROLLBACK TO creacion_pedido");
        } catch (SQLException e) {
            throw new Exception("Fallo eliminando los detalles del pedido. "+e.getMessage(),e);
        } catch (Exception e) {
            throw new Exception("Fallo eliminando los detalles del pedido. "+e.getMessage(),e);
        }
    }
    
    // Cancela el pedido
    public static void cancelarPedido(Connection con) throws Exception{
        try (Statement st = con.createStatement()) {
            st.execute("ROLLBACK");
        } catch (SQLException e) {
            throw new Exception("Fallo cancelando el pedido. "+e.getMessage(),e);
        } catch (Exception e) {
            throw new Exception("Fallo cancelando el pedido. "+e.getMessage(),e);
        }
    }
    
    // Finaliza el pedido guardando definitivamente los cambios
    public static void finalizarPedido(Connection con) throws Exception{
        try (Statement st = con.createStatement()) {
            st.execute("COMMIT");
        } catch (SQLException e) {
            throw new Exception("Fallo guardando el pedido. "+e.getMessage(),e);
        } catch (Exception e) {
            throw new Exception("Fallo guardando el pedido. "+e.getMessage(),e);
        }
    }
    
}
