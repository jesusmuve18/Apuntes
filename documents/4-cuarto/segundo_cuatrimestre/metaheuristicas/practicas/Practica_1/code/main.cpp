#include <iostream>
#include <chrono>

#include <random.hpp>
#include <string>
#include <util.h>

// Real problem class
// #include "pincrem.h"
#include "clusteringPloblem.h"

// All algorithms
#include "greedy.h"
#include "randomsearch.h"
#include "localsearch.h"

// For reading Files
#include <fileReader.h>

using namespace std;

// For reading dataSet from file
vector<Point> readDataSet(string ruta, char delimitador = ',') {
    vector<vector<double>> raw_dataset = fileReader::read(ruta, delimitador);

    vector<Point> dataset;
    dataset.reserve(raw_dataset.size()); 

    for(int i = 0; i < raw_dataset.size(); i++) {
        Point p;
        p.coordinate = raw_dataset[i]; 
        dataset.push_back(p);
    }

    return dataset;
}

// For reading constraints from file (vector form)
vector<Constraint> readVConstraints(string ruta, char delimitador = ','){
    vector<vector<double>> Mres = fileReader::read(ruta, delimitador);  // Matriz de restricciones
    std::vector<Constraint> Vres;                                       // Vector de restricciones

    // Recorremos la matriz para rellenar el vector
    for (int i = 0; i < Mres.size(); ++i) {
        for (int j = i + 1; j < Mres[i].size(); ++j) {
            
            if (Mres[i][j] != 0) {
                Constraint c;
                c.i = i;
                c.j = j;
                c.tipo = Mres[i][j]; // Guardará 1 o -1
                
                Vres.push_back(c);
            }
        }
    }

    return Vres;
}

// For reading constraints from file
vector<vector<int>> readMConstraints(string ruta, char delimitador = ','){
    vector<vector<double>> Mread = fileReader::read(ruta, delimitador);  // Matriz de restricciones
    vector<vector<int>> Mres;

    Mres.reserve(Mread.size()); 

    for (const auto& row : Mread) {
        Mres.emplace_back(row.begin(), row.end());
    }

    return Mres;
}

int main(int argc, char *argv[])
{
    bool csv_output_format = true; // 

    long int seed;
    string filename ;
    int ratio;
    int k;

    if (argc == 1) {
        seed = 42;
        filename = "../data/zoo";
        ratio = 30;
        k = 7;
    } else if (argc == 2)
    {
        seed = atoi(argv[1]);
    } else if (argc == 5)
    {
        seed = atoi(argv[1]);
        filename = argv[2];
        ratio = atoi(argv[3]);
        k = atoi(argv[4]);

    }  else {
        cout << "Error: The execution must be: " << argv[0] << " <seed>" << endl;
        cout << "                              " << argv[0] << " <seed> <filename> <ratio> <k>" << endl;
        return 1;
    }

    // Read de data from de files
    vector<Point> data = readDataSet(filename +"_set.dat");
    vector<Constraint> Vres = readVConstraints(filename + "_set_const_"+ to_string(ratio) +".dat");
    vector<vector<int>> Mres = readMConstraints(filename + "_set_const_"+ to_string(ratio) +".dat");


    // Create the algorithms
    RandomSearch<int> ralg = RandomSearch<int>();
    GreedySearch rgreedy = GreedySearch();
    LocalSearch rlocalsearch = LocalSearch();

    // Create the specific problem
    ClusteringProblem cproblem = ClusteringProblem(data, Vres, k);

    if (!csv_output_format) {
        cout << "Random seed: " << seed << endl;
        cout << "Dataset name: " << filename << endl;
        cout << "Ratio: " << to_string(ratio) << endl;
        cout << "k=" << k << endl;
        cout << "Loaded dataset:  " << data.size() << " elements " << endl;
        cout << "Loaded constraints matrix: " << Mres.size() << "x" << Mres[0].size() << endl;
        cout << "Lambda: " << cproblem.getLambda() << endl;
        cout << "Constraints: " << Vres.size() << endl;
    } else {
        // Imprimimos la cabecera del CSV
        cout << "Algorithm,Seed,Dataset,Ratio,K,Fitness,Global_Deviation,Infeasibility,Evaluations,Time_ms" << endl;
    }

    // Solve using evaluations
    vector<pair<string, MH<int> *>> algoritmos = {make_pair("RandomSearch", &ralg),
                                                  make_pair("Greedy", &rgreedy),
                                                  make_pair("LocalSearch", &rlocalsearch)};
    Problem<int> *problem = dynamic_cast<Problem<int> *>(&cproblem);

    for (int i = 0; i < algoritmos.size(); i++)
    {
        Random::seed(seed);
        auto mh = algoritmos[i].second;

        auto start = std::chrono::high_resolution_clock::now(); // Comenzamos a medir el tiempo
        ResultMH result = mh->optimize(*problem, 100000);
        auto stop = std::chrono::high_resolution_clock::now();  // Detenemos el cronómetro
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

        // Extracción de métricas
        double final_infeasibility = ((double)cproblem.infeasibility(result.solution)/Vres.size());
        double final_deviation = cproblem.globalDesviation(result.solution);

        if (csv_output_format) {
            cout << algoritmos[i].first << ","
                 << seed << ","
                 << filename << ","
                 << ratio << ","
                 << k << ","
                 << result.fitness << ","
                 << final_deviation << ","
                 << final_infeasibility << ","
                 << result.evaluations << ","
                 << duration.count() << endl;
        } else {
            cout << "===================================" << endl;
            cout << "Used Algorithm: " << algoritmos[i].first << endl;
            cout << "Best solution: " << result.solution << endl;
            cout << "Infeasibility: " << final_infeasibility << endl;
            cout << "Best fitness: " << result.fitness << endl;
            cout << "Evaluations: " << result.evaluations << endl;
            cout << "Global desviation: " << final_deviation << endl;
            cout << "Time passed: " << duration.count() << "ms" << endl;
        }
    }

    return 0;
}
