#include "problem.h"
#include <vector>
#include <cmath>
#include <limits>


struct Constraint {
    int i, j;       // Indices de los puntos
    int tipo;       // 1 para Must-Link, -1 para Cannot-Link
};

struct Point {
    std::vector<double> coordinate;
};

// Para calcular rápidamente infeasibility con solo pequeños cambios
template<>
class SolutionFactoringInfo<int> {
public:
    int infeasibility;                  // Número de restricciones incumplidas
    std::vector<Point> centroids;            // Centroides actuales
    std::vector<int> n_puntos_cluster;       // Número de puntos por cluster
    std::vector<double> distancia_ic;   // Distancia intra-cluster
    double desviacionGeneral;           // Desviación general     
    tFitness fitness;                   // Función fitness     

    // Constructor por defecto
    SolutionFactoringInfo() = default;

    // Constructor
    SolutionFactoringInfo(int inf, const std::vector<Point> &centroids, const std::vector<int> &n_puntos_cluster, const std::vector<double> &distancia_ic, double desviacionGeneral, tFitness fitness) {
        this->infeasibility = inf;
        this->centroids = centroids;
        this->n_puntos_cluster = n_puntos_cluster;
        this->distancia_ic = distancia_ic;
        this->desviacionGeneral = desviacionGeneral;
        this-> fitness = fitness;
    }
    
    // Destructor
    virtual ~SolutionFactoringInfo() = default; 
};

class ClusteringProblem : public Problem<int> {
private:
    std::vector<Point> dataset;                     // Datos del problema
    int k;                                          // Número de clusters
    std::vector<Constraint> Vres;                   // Vector de restricciones
    double lambda;                                  // Factor de escalado

    
public:
    // Constructor
    ClusteringProblem(std::vector<Point> data, std::vector<Constraint> restrictions, int k) : dataset(data), Vres(restrictions), k(k){
        // Calculo la distancia máxima entre dos puntos del dataset
        double D = 0.0;

        int n = dataset.size();

        // Comparamos cada punto con todos los demás
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                double d = distance(dataset[i], dataset[j]);
                if (d > D) {
                    D = d;
                }
            }
        }

        lambda = (D/restrictions.size());
    }

    // Crear un SolutionFactoringInfo
    SolutionFactoringInfo<int>* generateFactoringInfo(const tSolution<int> &solution) {
        int inf = infeasibility(solution);
        std::vector<Point> centroides = getCentroids(solution);          
        std::vector<int> n_puntos_cluster(k, 0); 
        std::vector<double> distancia_ic(k,0);
        double desviacionGeneral = 0;

        // Cuento el número de puntos de cada cluster
        for (int i = 0; i < dataset.size(); i++) n_puntos_cluster[solution[i]]++;

        // Calculo la distancia intracluster
        for (int i=0; i < dataset.size(); i++) {
            int cluster = solution[i];
            distancia_ic[cluster] += distance(dataset[i], centroides[cluster]);
        }

        // Calculo la desviación general
        for (int j = 0; j<k; j++){
            int n = n_puntos_cluster[j];
            if (n > 0) {
                desviacionGeneral += (distancia_ic[j]/n);
            }
        }       

        desviacionGeneral /= k;

        tFitness fit = fitness(inf, desviacionGeneral);

        return new SolutionFactoringInfo<int>(inf, centroides, n_puntos_cluster, distancia_ic, desviacionGeneral, fit);
    }

    // Actualizar un SolutionFactoringIngo
    void updateSolutionFactoringInfo(SolutionFactoringInfo<int> *solution_info, const tSolution<int> &solution, unsigned pos_change, int new_value) override {
        int old_value = solution[pos_change];
        if (old_value == new_value) return; // Si no cambia de cluster, no hacemos nada

        // Actualizamos infeasibility
        solution_info->infeasibility = infeasibility(solution, solution_info, pos_change, new_value);

        // Actualizamos centroides
        int c_old = old_value;
        int c_new = new_value;
        int dim = dataset[0].coordinate.size();
        
        int n_old = solution_info->n_puntos_cluster[c_old];
        int n_new = solution_info->n_puntos_cluster[c_new];

        // Extraemos el punto del centroide viejo: (Media_vieja * N - Punto) / (N - 1)
        if (n_old > 1) {
            for (int d = 0; d < dim; d++) {
                double sumaVieja = solution_info->centroids[c_old].coordinate[d] * n_old;
                solution_info->centroids[c_old].coordinate[d] = (sumaVieja - dataset[pos_change].coordinate[d]) / (n_old - 1);
            }
        } else { // Si se queda vacío, el centroide es 0
            for (int d = 0; d < dim; d++) solution_info->centroids[c_old].coordinate[d] = 0.0;
        }

        // Añadimos el punto al centroide nuevo: (Media_vieja * N + Punto) / (N + 1)
        for (int d = 0; d < dim; d++) {
            double sumaVieja = solution_info->centroids[c_new].coordinate[d] * n_new;
            solution_info->centroids[c_new].coordinate[d] = (sumaVieja + dataset[pos_change].coordinate[d]) / (n_new + 1);
        }

        // Actualizamos la cantidad de puntos
        solution_info->n_puntos_cluster[c_old]--;
        solution_info->n_puntos_cluster[c_new]++;

        // Actualizamos la distancia intra-cluster
        // Como el centroide se movió, recalculamos las distancias solo de los puntos en c_old y c_new
        double old_dist_ic_old = solution_info->distancia_ic[c_old];
        double old_dist_ic_new = solution_info->distancia_ic[c_new];
        
        double new_dist_ic_old = 0.0;
        double new_dist_ic_new = 0.0;

        for (size_t i = 0; i < dataset.size(); i++) {
            if (i == pos_change) {
                // El punto que se mueve, ahora se mide respecto a c_new
                new_dist_ic_new += distance(dataset[i], solution_info->centroids[c_new]);
            } else if (solution[i] == c_old) {
                new_dist_ic_old += distance(dataset[i], solution_info->centroids[c_old]);
            } else if (solution[i] == c_new) {
                new_dist_ic_new += distance(dataset[i], solution_info->centroids[c_new]);
            }
        }

        solution_info->distancia_ic[c_old] = new_dist_ic_old;
        solution_info->distancia_ic[c_new] = new_dist_ic_new;

        // Actualizamos la desviación general
        // Media antigua de estos dos clústeres
        double vieja_contrib_old = (n_old > 0) ? (old_dist_ic_old / n_old) : 0.0;
        double vieja_contrib_new = (n_new > 0) ? (old_dist_ic_new / n_new) : 0.0;

        // Media nueva de estos dos clústeres
        int new_n_old = solution_info->n_puntos_cluster[c_old];
        int new_n_new = solution_info->n_puntos_cluster[c_new];
        double nueva_contrib_old = (new_n_old > 0) ? (new_dist_ic_old / new_n_old) : 0.0;
        double nueva_contrib_new = (new_n_new > 0) ? (new_dist_ic_new / new_n_new) : 0.0;

        // Fórmula para actualizar la media global en O(1)
        double sum_desv = (solution_info->desviacionGeneral * k) - vieja_contrib_old - vieja_contrib_new + nueva_contrib_old + nueva_contrib_new;
        
        solution_info->desviacionGeneral = sum_desv / k;
        solution_info->fitness = fitness(solution_info->infeasibility, solution_info->desviacionGeneral);
    }

    // Obtener los centroides
    vector<Point> getCentroids(const tSolution<int> &solution) {
        int dim = dataset[0].coordinate.size();     // Dimensión del espacio en el que se está trabajando
        
        // Centroides de cada cluster
        std::vector<Point> centroides(k);
        for(int j = 0; j < k; j++) {
            centroides[j].coordinate.assign(dim, 0.0);
        }

        std::vector<int> n_puntos_cluster(k, 0);                // Número de puntos para cada cluster

        // Recorro todos los vectores sumándolos a su cluster
        for (int i = 0; i < dataset.size(); i++) {
            int cluster = solution[i];

            for(int d = 0; d<dim; d++){
                centroides[cluster].coordinate[d] += dataset[i].coordinate[d];
            }

            n_puntos_cluster[cluster]++;
        }

        // Divido entre el número de cada cluster para calcular la media
        for (int c = 0; c < k; c++) {
            int n = n_puntos_cluster[c];
            if (n > 0) {
                for(int d = 0; d < dim; d++){ 
                    centroides[c].coordinate[d] /= n;
                }
            }
        }

        return centroides;
    }

    // Calcula la desviación global de una solución (para el fitness)
    double globalDesviation(const tSolution<int> &solution) {
        int dim = dataset[0].coordinate.size();     // Dimensión del espacio en el que se está trabajando
        
        
        std::vector<Point> centroides = getCentroids(solution);     // Centroides de cada cluster
        std::vector<int> n_puntos_cluster(k, 0);                    // Número de puntos para cada cluster

        // Cuento el número de puntos por cluster
        for (int i = 0; i < dataset.size(); i++) n_puntos_cluster[solution[i]]++;

        std::vector<double> distancia_ic(k,0);  // Distancia intra-cluster

        // Acumulo las distancias
        for (int i=0; i < dataset.size(); i++) {
            int cluster = solution[i];
            distancia_ic[cluster] += distance(dataset[i], centroides[cluster]);
        }

        double desviacionGeneral = 0;   // Desviación general

        // Sumo la media de las distancias
        for (int j = 0; j<k; j++){
            int n = n_puntos_cluster[j];
            if (n > 0) {
                desviacionGeneral += (distancia_ic[j]/n);
            }
        }

        return (desviacionGeneral/k); 
    }

    // Calcula el número de restricciones incumplidas (para el fitness)
    int infeasibility(const tSolution<int> &solution) {
        int inf = 0;

        for (const auto &res : Vres) {
            bool juntos = (solution[res.i] == solution[res.j]);

            if (res.tipo == 1 && !juntos) {
                inf++; // Error: Deberían estar juntos (Must-Link)
            } else if (res.tipo == -1 && juntos) {
                inf++; // Error: No deberían estar juntos (Cannot-Link)
            }
        }
        return inf;
    }

    // Ijnfeasibility a partir de una modificación
    int infeasibility(const tSolution<int> &solution, SolutionFactoringInfo<int> *solution_info, unsigned pos_change, int new_value) {
        int old_value = solution[pos_change];
        if (old_value == new_value) return (*solution_info).infeasibility; // Si no cambia de cluster, no hacemos nada

        // Si hay cambio miro qué restricciones se incumplen o cumplen ahora
        int deltaInf = 0;
        for (const auto &res : Vres) {
            if (res.i == (int)pos_change || res.j == (int)pos_change) {
                int vecino = (res.i == (int)pos_change) ? res.j : res.i;
                int clusterVecino = solution[vecino];

                // Si antes la rompía, la restamos. Si ahora la rompe, la sumamos.
                if ((res.tipo == 1 && old_value != clusterVecino) || (res.tipo == -1 && old_value == clusterVecino)) deltaInf--;
                if ((res.tipo == 1 && new_value != clusterVecino) || (res.tipo == -1 && new_value == clusterVecino)) deltaInf++;
            }
        }

        return ((*solution_info).infeasibility + deltaInf);
    }
    
    // Fitness a partir de infeasibility y desviacion
    tFitness fitness(int infeasibility, double gdesviation) {
        return (gdesviation + lambda * infeasibility);
    }

    // Fitness desde cero
    tFitness fitness(const tSolution<int> &solution) override {
        double C = globalDesviation(solution);
        int inf = infeasibility(solution);

        return fitness(inf, C);
    }

    // Fitness a partir de una modificación
    tFitness fitness(const tSolution<int> &solution, SolutionFactoringInfo<int> *solution_info, unsigned pos_change, int new_value) override {
        // Si no hay cambio, calculamos el fitness con la info actual directamente
        if (solution[pos_change] == new_value) return fitness(solution_info->infeasibility, solution_info->desviacionGeneral);

        // Si cambia actualizamos una copia (para no modificarlo)
        SolutionFactoringInfo<int> tmp_info = *solution_info;
        updateSolutionFactoringInfo(&tmp_info, solution, pos_change, new_value);

        return fitness(tmp_info.infeasibility, tmp_info.desviacionGeneral);
    }

    // Crear solución (aleatoria)
    tSolution<int> createSolution() override {
        tSolution<int> sol(dataset.size());
        for (size_t i = 0; i < sol.size(); ++i) {
            sol[i] = Random::get<int>(0, k - 1);
        }
        return sol;
    }

    // Número de puntos en el dataset
    size_t getSolutionSize() override {
        return dataset.size();
    }

    // Rango: Los IDs de los clusters [0, K-1]
    std::pair<int, int> getSolutionDomainRange() override {
        return {0, k - 1};
    }

    // Calcula si una solución es válida o no
    bool isValid(const tSolution<int> &solution) override {
        std::vector<bool> hasPoints(k, false);
        for (int cluster : solution) hasPoints[cluster] = true;
        
        for (bool present : hasPoints) if (!present) return false;
        return true;
    }

    // Arreglar una solución
    void fix(tSolution<int> &solution) override {
        std::vector<int> n_puntos(k, 0);
        for (int cluster : solution) {
            n_puntos[cluster]++;
        }

        // Buscamos clusters vacíos
        for (int i = 0; i < k; i++) {
            if (n_puntos[i] == 0) {
                // Buscamos un punto de un cluster que tenga más de uno para no dejarlo vacío también
                bool arreglado = false;
                for (int p = 0; p < solution.size() && !arreglado; p++) {
                    int cluster_actual = solution[p];
                    if (n_puntos[cluster_actual] > 1) {
                        solution[p] = i; // Movemos el punto al cluster vacío
                        n_puntos[cluster_actual]--;
                        n_puntos[i]++;
                        arreglado = true;
                    }
                }
            }
        }
    }

    // Obtener un punto por su índice
    Point getPoint(int i) {
        return dataset[i];
    }

    // Obtener el valor de lambda
    double getLambda() {
        return this->lambda;
    }

    // Obtener el valor de K
    int getK() {
        return this->k;
    }

    // Función auxiliar para calcular distancia euclídea
    static double distance(const Point &a, const Point &b) {
        double d = 0;                       // Distancia acumulada
        int dim = a.coordinate.size();      // Dimensión

        for(int i=0; i<dim; i++) {
            d += std::pow(a.coordinate[i] - b.coordinate[i], 2);
        }

        return std::sqrt(d);
    }

};