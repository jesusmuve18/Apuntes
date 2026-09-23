#include <iostream>
#include <fileReader.h>
#include <vector>
#include <util.h>

using namespace std;
int main(int argc, char *argv[]) {
  
    vector<vector<double>> datos = fileReader::read("../data/bupa_set.dat");
    vector<vector<double>> restricciones = fileReader::read("../data/bupa_set_const_15.dat");

    cout<<"Datos leídos:" << endl;
    for(int i=0; i<datos.size(); i++) {
        cout<<"( "<<datos[i]<<")" << endl;
    }

    return 0;
}
