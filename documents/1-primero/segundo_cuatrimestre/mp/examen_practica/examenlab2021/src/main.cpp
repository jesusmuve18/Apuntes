#include <iostream>
#include <fstream>
#include <fstream>
using namespace std;

int nalumnos = 51;
const string _nombres[] = {"JOSE ALGAR FERNANDEZ", "JOAQUIN ARCILA PEREZ", "GERARDO ARENAS NASRAWIN", "CARMEN AZORIN MARTI", "RUBEN BALLESTEROS RIVERO", "MARTA BENITEZ HERNANDEZ", "MANUEL VICENTE BOLAÑOS QUESADA", "CLARA BOLIVAR PELAEZ", "LORENA CACERES ARIAS", "PEDRO HAIMAR CASTILLO GARCIA", "LUCIA CEPEDA GONZALEZ", "MARIA CRIBILLES PEREZ", "MANUEL DIAZ-MECO TERRES", "PABLO GALVEZ ORTIGOSA", "JESUS SAMUEL GARCIA CARBALLO", "CARLOS GARCIA JIMENEZ", "BORA GOKER SOLA", "JAVIER GOMEZ LOPEZ", "ANA GRACIANI DONAIRE", "JOSE ALBERTO HOCES CASTRO", "SHAO JIE HU CHEN", "ADRIAN JAEN FUENTES", "AARON JERONIMO FERNANDEZ", "NOURA LACHHAB BOUHMADI", "LAURA LAZARO SORALUCE", "RAMON LIRIA SANCHEZ", "ENRIQUE LOPEZ GARCIA", "CARLOTA LOPEZ QUESADA", "PAULA MANGANO GONZALEZ", "JOSE MARIA MARTIN MARTIN", "JAIME MARTINEZ BRAVO", "FRANCISCO NICOLAS MEDINA ROSAS", "MARIO MEGIAS MATEO", "JOSE LUIS MERA CARDOSO", "CRISTOBAL MERINO SAEZ", "QUINTIN MESA ROMERO", "ALVARO MOLINA ALVAREZ", "JUAN MOLINA MOLINA", "EDUARDO MOLINA VELAZQUEZ", "CLARA ORTEGA SEVILLA", "CARLOS MANUEL PERALES GOMEZ", "PEDRO PLAZAS MUNUERA", "CARLOS QUESADA PEREZ", "EDUARDO RODRIGUEZ CAO", "TOMAS RODRIGUEZ HERNAEZ", "ELSA RODRIGUEZ MACMICHAEL", "MARIO RUBIO VENZAL", "MANUEL RUIZ AGUILAR", "SOLEDAD RUIZ GOMEZ", "DAVID TORONJO MENDOZA", "ELENA TORRES FERNANDEZ"};

void Introduccion(const string &nombre) {
    int _i, _tipo;
    string aux;
    cout << "Buscando (" << nombre << ")" << endl;
    for (_i = 1; _i <= nalumnos && _nombres[_i - 1] != nombre; _i++);
    if (_i <= nalumnos) {
        cerr << "Alumno: " << nombre << endl;
        cerr << "Fichero: " << "1." + to_string(_i) + ".xhtml" << endl;
        cerr << "Variante: ";
        _tipo = nombre[0] % 4;
        switch (nombre[0] % 4) {
            case 0: cerr << "<class=" << endl;
                break;
            case 1: cerr << "<href=" << endl;
                break;
            case 2: cerr << "<sup>" << endl;
                break;
            case 3: cerr << "<h3>" << endl;
                break;
        }
        cerr << endl << endl << endl;
        //        cout << nombre << " " << _i << " " << _tipo << endl;
    } else {
        cout << "Por favor introduce tu nombre tal y como aparece en el array nombres[] " << endl;
        exit(1);
    }
}

/**
 * @brief Implementa la función del enunciado 
 * @param v1 Primer vector
 * @param nv1 Número de elementos de @a v1
 * @param v3 Vector resultado. Debe reservarse memoria dentro de la función
 * @param nv3 Número de elementos de @a v3
 */
void funcion(string const *v1, int nv1, string * &v3, int &nv3) {
}

int main() {
    ifstream findice, ficapitulo;
    ofstream fosalida;
    string nameindice = "./data/index.dat",
            namesalida = "./data/salida.dat",
            namecapitulo; // Copia aquí el nombre de tu fichero de trabajo
    string *capitulo = nullptr, *seleccion = nullptr;
    int ncapitulo, nseleccion;

    string linea, cadena;
    bool salir;

    // Consulta tu nombre, fichero de trabajo y variante
    Introduccion("PERICO DE LOS PALOTES"); // Copia aquí tu nombre
    
    // Abrir fichero de indice. Comprobar sólo error de apertura

    // Buscar namecapitulo en el fichero de índce y leer el número de líneas

    // Reservar memoria para *capitulo
    
    // Abrir fichero namecapitulo dentro de "./data/"

    // Leer las líneas en memoria dinámica
    
    // Calcular la lista de cadenas de la variante
    funcion(capitulo, ncapitulo, seleccion, nseleccion);
    
    // Guardar el resultado en "./data/salida.dat"

    // Liberar la memoria
    return 0;
}

