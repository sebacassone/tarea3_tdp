#include "Simplex.h"
#include <queue>
#include <cmath>
#include <limits>
#include <numeric>
using namespace std;

struct Compare
{
    bool operator()(const Simplex &lhs, const Simplex &rhs) const
    {
        // Primero compara los valores de la solución
        if (lhs.getObjectiveValue() != rhs.getObjectiveValue())
        {
            return lhs.getObjectiveValue() < rhs.getObjectiveValue(); // Mayor valor al frente
        }
        return lhs.getLowerBound() < rhs.getLowerBound(); // Mayor cota inferior al frente
    }
};

class Branch
{
private:
    priority_queue<Simplex, vector<Simplex>, Compare> Live; // Vector de Simplex (Lives)
    // método para agregar problemas a la cola de problemas
    void addBranchProblems(Simplex &problem, int variableIndex, int value);

public:
    // método para resolver el problema original por el método de Branch and Bound
    std::vector<float> solve(Simplex &originalProblem);
    vector<float> greedySolution(Simplex &problem);
};
