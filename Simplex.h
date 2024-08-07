#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>
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
    Simplex copy();
    std::vector<float> getSolution();
    void printProblemMatrix();
    void printSolution();
    float getObjectiveValue() const { return zCotaSuperior; }
    float getLowerBound() const { return zCotaInferior; }
    bool getOptimal() const
    {
        return isOptimal;
    }
    bool getFeasible() const { return isFeasible; }
    std::vector<float> getObjectiveCoefficients() const { return // retornar la primera submatriz de la matriz a
                                                          std::vector<float>(initialA[0].begin() + 1, initialA[0].end()); }
    std::vector<float> getWeights() const { return // retornar la primera columna de la matriz a
                                            std::vector<float>(initialA[1].begin() + 1, initialA[1].end()); }
    int getNumVariables() const { return n; }
    bool isFeasible(std::vector<float> solution) const
    {
        // Verificar si la solución es factible
        for (int i = 0; i < m; ++i)
        {
            float sum = 0;
            for (int j = 1; j <= n; ++j)
            {
                sum += a[i + 1][j] * solution[j];
            }
            if (a[i + 1][0] > sum + EPS)
            {
                return false;
            }
        }
        return true;
    }
};
