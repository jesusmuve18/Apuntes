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

#include "aggAlgorithm.h"
#include "ageAlgorithm.h"
#include "memeticAlgorithm.h"

#include "esAlgorithm.h"
#include "bmbAlgorithm.h"
#include "ilsAlgorithm.h"
#include "ilsesAlgorithm.h"

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
    bool csv_output_format = true; // para salida formateada en csv

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

    // Read the data from the files
    vector<Point> data = readDataSet(filename +"_set.dat");
    vector<Constraint> Vres = readVConstraints(filename + "_set_const_"+ to_string(ratio) +".dat");
    vector<vector<int>> Mres = readMConstraints(filename + "_set_const_"+ to_string(ratio) +".dat");

    // General parameters
    int tam_pob = 50;
    int n_evals = 100000;

    // AM Parameters
    double p_cruce_am = 0.8;                    // Probabilidad de cruce en AM
    double p_mutacion_am = 0.1;                 // Probabilidad de mutación en AM
    int max_bls_failures = 0.1 * tam_pob;       // Máximo número de fallos permitido en bls
    int n_gens_to_apply_bls = 10;               // Cada cuántas generaciones se aplicará BLS
    int maxevals_bls = 100;                     // Máximo de evaluaciones permitidas por cada ejecución de BLS
    aggType agg_type_for_am = aggType::FIXED_SEGMENT;   // AGG que mejor que mejor resultado ha proporcionado

    // Parameters for ES
    double phi_es = 0.3;
    double mu_es = 0.2;

    // Parameters for BMB
    int n_arranques_bmb = 5;
    int maxevals_bl_bmb = 20000;

    // Parameters for ILS
    int n_arranques_ils = 5;
    int maxevals_bl_ils = 20000;
    double p_mutacion_ils = 0.2;

    // Parameters for ILS-ES
    int n_arranques_ils_es = 5;
    int maxevals_es = 20000;
    double p_mutacion_es = 0.2;
    double phi_ils_es = 0.3;
    double mu_ils_es = 0.2;

    // Create the algorithms

    // Practica 1
    RandomSearch<int> ralg = RandomSearch<int>();
    GreedySearch greedy = GreedySearch();
    LocalSearch localsearch = LocalSearch();

    // Practica 2
    MemeticAlgorithm am_best = MemeticAlgorithm(amType::BEST,   agg_type_for_am, tam_pob, p_cruce_am, p_mutacion_am, max_bls_failures, n_gens_to_apply_bls, maxevals_bls);

    // Practica 3
    ESAlgorithm es = ESAlgorithm(phi_es, mu_es);
    BMBAlgorithm bmb = BMBAlgorithm(n_arranques_bmb, maxevals_bl_bmb);
    ILSAlgorithm ils = ILSAlgorithm(n_arranques_ils, maxevals_bl_ils, p_mutacion_ils);
    ILSESAlgorithm ils_es = ILSESAlgorithm(n_arranques_ils_es, maxevals_es, p_mutacion_es, phi_ils_es, mu_ils_es);

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
    vector<pair<string, MH<int> *>> algoritmos = {
        make_pair("Greedy", &greedy),
        make_pair("Random", &ralg),
        make_pair("BL", &localsearch),
        make_pair("AM-BEST (Mejor P2)", &am_best),
        make_pair("BMB", &bmb),
        make_pair("ILS-BL", &ils),
        make_pair("ES", &es),
        make_pair("ILS-ES", &ils_es)
    };

    Problem<int> *problem = dynamic_cast<Problem<int> *>(&cproblem);

    for (int i = 0; i < algoritmos.size(); i++)
    {
        Random::seed(seed);
        auto mh = algoritmos[i].second;

        auto start = std::chrono::high_resolution_clock::now(); // Comenzamos a medir el tiempo
        ResultMH result = mh->optimize(*problem, n_evals);
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
