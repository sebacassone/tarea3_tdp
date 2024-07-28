#include "Simplex.h"
#include <queue>
#include <cmath>
#include <limits>
using namespace std;

class Branch
{
private:
    queue<Simplex> Live; // Vector de Simplex (Lives)
    void addBranchProblems(Simplex &problem, int variableIndex, float value);

public:
    Branch(Simplex s);
    ~Branch();
    std::vector<float> solve(Simplex &originalProblem);
    void printSolution();
    Simplex getOptimalSolution();
};
