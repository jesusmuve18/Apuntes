package app;

import java.sql.Connection;
import java.sql.Statement;
import java.sql.SQLException;

public class Initializer {

    public static void init(Connection con) throws Exception {

        try (Statement st = con.createStatement()) {
            
            // Eliminacion de tablas
            try { 
                st.executeUpdate("DROP TABLE Detalle_Pedido CASCADE CONSTRAINTS"); 
            } catch (Exception e) {
                e.printStackTrace();
            }
            
            try { 
                st.executeUpdate("DROP TABLE Pedido CASCADE CONSTRAINTS"); 
            } catch (Exception e) {
                e.printStackTrace();
            }
            
            try { 
                st.executeUpdate("DROP TABLE Stock CASCADE CONSTRAINTS"); 
            } catch (Exception e) {
                e.printStackTrace();
            }
            
            // Creacion de tablas
            st.executeUpdate(
                "CREATE TABLE Stock(" +
                " Cproducto VARCHAR2(10) PRIMARY KEY," +
                " Cantidad NUMBER)"
            );

            st.executeUpdate(
                "CREATE TABLE Pedido(" +
                " Cpedido VARCHAR2(10) PRIMARY KEY," +
                " Ccliente VARCHAR2(10)," +
                " Fecha_pedido DATE)"
            );

            st.executeUpdate(
                "CREATE TABLE Detalle_Pedido(" +
                " Cpedido VARCHAR2(10)," +
                " Cproducto VARCHAR2(10)," +
                " Cantidad NUMBER," +
                " CONSTRAINT detalle_pk PRIMARY KEY (Cpedido, Cproducto)," +
                " CONSTRAINT detalle_pedido_fk FOREIGN KEY (Cpedido) REFERENCES Pedido(Cpedido)," +
                " CONSTRAINT detalle_stock_fk FOREIGN KEY (Cproducto) REFERENCES Stock(Cproducto))"
            );
            
            // Insercion de tuplas
            st.executeUpdate("INSERT INTO Stock (Cproducto, Cantidad) VALUES ('PRD001', 150)");
            st.executeUpdate("INSERT INTO Stock (Cproducto, Cantidad) VALUES ('PRD002', 60)");
            st.executeUpdate("INSERT INTO Stock (Cproducto, Cantidad) VALUES ('PRD003', 80)");
            st.executeUpdate("INSERT INTO Stock (Cproducto, Cantidad) VALUES ('PRD004', 45)");
            st.executeUpdate("INSERT INTO Stock (Cproducto, Cantidad) VALUES ('PRD005', 200)");
            st.executeUpdate("INSERT INTO Stock (Cproducto, Cantidad) VALUES ('PRD006', 35)");
            st.executeUpdate("INSERT INTO Stock (Cproducto, Cantidad) VALUES ('PRD007', 110)");
            st.executeUpdate("INSERT INTO Stock (Cproducto, Cantidad) VALUES ('PRD008', 95)");
            st.executeUpdate("INSERT INTO Stock (Cproducto, Cantidad) VALUES ('PRD009', 75)");
            st.executeUpdate("INSERT INTO Stock (Cproducto, Cantidad) VALUES ('PRD010', 140)");
            
            st.executeUpdate("INSERT INTO Pedido (Cpedido, Ccliente, Fecha_pedido) VALUES ('PED001', 'CLI001', DATE '2025-10-02')");
            st.executeUpdate("INSERT INTO Pedido (Cpedido, Ccliente, Fecha_pedido) VALUES ('PED002', 'CLI002', DATE '2025-10-05')");
            st.executeUpdate("INSERT INTO Pedido (Cpedido, Ccliente, Fecha_pedido) VALUES ('PED003', 'CLI003', DATE '2025-10-08')");
            st.executeUpdate("INSERT INTO Pedido (Cpedido, Ccliente, Fecha_pedido) VALUES ('PED004', 'CLI004', DATE '2025-10-12')");
            st.executeUpdate("INSERT INTO Pedido (Cpedido, Ccliente, Fecha_pedido) VALUES ('PED005', 'CLI005', DATE '2025-10-15')");
            st.executeUpdate("INSERT INTO Pedido (Cpedido, Ccliente, Fecha_pedido) VALUES ('PED006', 'CLI006', DATE '2025-10-20')");
            st.executeUpdate("INSERT INTO Pedido (Cpedido, Ccliente, Fecha_pedido) VALUES ('PED007', 'CLI007', DATE '2025-10-25')");
            st.executeUpdate("INSERT INTO Pedido (Cpedido, Ccliente, Fecha_pedido) VALUES ('PED008', 'CLI008', DATE '2025-10-28')");
            st.executeUpdate("INSERT INTO Pedido (Cpedido, Ccliente, Fecha_pedido) VALUES ('PED009', 'CLI009', DATE '2025-11-01')");
            st.executeUpdate("INSERT INTO Pedido (Cpedido, Ccliente, Fecha_pedido) VALUES ('PED010', 'CLI010', DATE '2025-11-03')");
            
            st.executeUpdate("INSERT INTO Detalle_Pedido (Cpedido, Cproducto, Cantidad) VALUES ('PED001', 'PRD001', 2)");
            st.executeUpdate("INSERT INTO Detalle_Pedido (Cpedido, Cproducto, Cantidad) VALUES ('PED001', 'PRD003', 1)");
            st.executeUpdate("INSERT INTO Detalle_Pedido (Cpedido, Cproducto, Cantidad) VALUES ('PED002', 'PRD002', 5)");
            st.executeUpdate("INSERT INTO Detalle_Pedido (Cpedido, Cproducto, Cantidad) VALUES ('PED003', 'PRD005', 3)");
            st.executeUpdate("INSERT INTO Detalle_Pedido (Cpedido, Cproducto, Cantidad) VALUES ('PED004', 'PRD004', 4)");
            st.executeUpdate("INSERT INTO Detalle_Pedido (Cpedido, Cproducto, Cantidad) VALUES ('PED005', 'PRD001', 1)");
            st.executeUpdate("INSERT INTO Detalle_Pedido (Cpedido, Cproducto, Cantidad) VALUES ('PED006', 'PRD007', 6)");
            st.executeUpdate("INSERT INTO Detalle_Pedido (Cpedido, Cproducto, Cantidad) VALUES ('PED007', 'PRD008', 2)");
            st.executeUpdate("INSERT INTO Detalle_Pedido (Cpedido, Cproducto, Cantidad) VALUES ('PED008', 'PRD006', 1)");
            st.executeUpdate("INSERT INTO Detalle_Pedido (Cpedido, Cproducto, Cantidad) VALUES ('PED010', 'PRD010', 7)");
            
            st.executeUpdate("COMMIT"); // Esto dice que toda la operacion es una sola transaccion

            System.out.println("Base de datos inicializada correctamente");

        } catch (SQLException e) {
            throw new Exception("Fallo inicializando la base de datos. "+e.getMessage(),e);
        } catch (Exception e) {
            throw new Exception("Fallo inicializando la base de datos. "+e.getMessage(),e);
        }
    }
}
