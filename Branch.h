#ifndef BRANCH_H
#define BRANCH_H
#include <queue> // fabs y floor
#include <limits>
#include <numeric>
#include "Compare.h"
#include "Element.h"

using namespace std;

/*
 * Branch class:
 * Esta clase representa un solucionador de problemas de optimización lineal
 * con variables reales [x1,....,xn]., mediante el método de Branch and Bound.
 * Este busca maximizar una función con restricciones asociadas.
 */
class Branch
{
private:
    float greedyValue;                                      // Valor de la solución greedy
    priority_queue<Simplex, vector<Simplex>, Compare> Live; // Vector de Simplex (Lives)
    // método para agregar problemas a la cola de problemas
    void addBranchProblems(Simplex &problem, int variableIndex, int value);
    // método para comparar dos elementos
    bool compare(const Element &a, const Element &b);

public:
    // método para resolver el problema original por el método de Branch and Bound
    std::vector<float> solve(Simplex &originalProblem);
    // método para resolver el problema greedy
    vector<float> greedy(const Simplex &problem, const vector<float> &costos, const vector<float> &pesos);
};

#endif
