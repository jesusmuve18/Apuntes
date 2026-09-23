#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

class fileReader {
public:
    /**
     * Método estático que lee archivos numéricos.
     * @param ruta El nombre o ruta del archivo .dat
     * @param delimitador El carácter que separa los números (por defecto la coma)
     */
    static vector<vector<double>> read(string ruta, char delimitador = ',') {
        vector<vector<double>> matriz;
        ifstream archivo(ruta);

        if (!archivo.is_open()) {
            throw runtime_error("No se pudo abrir el archivo: " + ruta);
        }

        string linea;
        while (getline(archivo, linea)) {
            if (linea.empty()) continue;

            vector<double> fila;
            stringstream ss(linea);
            string celda;

            // Extraemos según el delimitador proporcionado
            while (getline(ss, celda, delimitador)) {
                fila.push_back(stod(celda));
            }

            if (!fila.empty()) {
                matriz.push_back(fila);
            }
        }

        archivo.close();
        return matriz;
    }
};