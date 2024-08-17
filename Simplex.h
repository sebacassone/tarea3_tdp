#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <limits>
#define EPS 1.0e-6

/*
    Descripcion de la clase Simplex:
    Esta clase representa un solucionador de problemas de optimizacion
    lineal con variables reales [x1,....,xn]., mediante el metodo Simplex.
    este busca maximizar una funcion con restricciones asociadas.
*/
class Simplex
{
private:
    int m;           // Numero de restricciones
    int n;           // Numero de variables
    bool isSolved;   // Si no es resuelto significa que no es factible.
    bool isOptimal;  // Si es resuelto y esOptimal es falso, significa que es no acotado.
    bool isFeasible; // Si es resuelto y esFeasible es falso, significa que no es factible.
    float zCotaSuperior;
    float zCotaInferior;
    int icase;
    std::vector<int> izrov;
    std::vector<int> iposv;
    std::vector<float> solution;
    int simplx();
    void maxValue(int mm, std::vector<int> ll, int nll, int iabf, int *kp, float *bmax);
    void locatePivot(int *ip, int kp);
    void exchangeParameters(int i1, int k1, int ip, int kp);
    float fabs(float x);

public:
    std::vector<std::vector<float>> initialA; // Matriz del problema.
    std::vector<std::vector<float>> a;        // Matriz en donde se genera la solucion.
    int m1;                                   // numero de restricciones <=
    int m2;                                   // ... >=
    int m3;                                   // igualdad

    Simplex(std::string file_name);
    Simplex(std::vector<std::vector<float>> a, int m1, int m2, int m3);
    ~Simplex();
    std::vector<float> solve(); // el retorno es [Z, x1, x2, ..., xn]
    void insertConstraint(float b, int var, int type);
    void insertComplexConstraint(std::vector<float> coeffs, float b, int type);
    Simplex copy();
    std::vector<float> getSolution();
    void printProblemMatrix();
    void printSolution();
    float getObjectiveValue() const;
    float getLowerBound() const;
    bool getOptimal() const;
    bool getFeasible() const;
    std::vector<float> getObjectiveCoefficients() const;
    std::vector<float> getWeights() const;
    int getNumVariables() const;
    float getMaxWeight() const;
    float calculateObjectiveValue(const std::vector<float> &solution) const;
    bool isConstraintFeasible(float b, int var, int type);
};
