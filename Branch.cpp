#include "Branch.h"

void Branch::addBranchProblems(Simplex &problem, int variableIndex, float value)
{
    Simplex p1 = problem.copy();
    Simplex p2 = problem.copy();

    // Agregar restricciones para ramificar
    p1.insertConstraint(std::ceil(value), variableIndex, 1);  // x >= ceil(value)
    p2.insertConstraint(std::floor(value), variableIndex, 2); // x <= floor(value)

    Live.push(p1);
    Live.push(p2);
}

vector<float> Branch::solve(Simplex &originalProblem)
{
    Live.push(originalProblem);
    float bestObjectiveValue = -numeric_limits<float>::max();
    vector<float> bestSolution;

    while (!Live.empty())
    {
        // Extrae el primer elemento de la cola
        Simplex currentProblem = Live.front();
        Live.pop();

        // Resuelve el problema actual
        vector<float> currentSolution = currentProblem.solve();

        // Verifica si el problema actual es factible
        if (!currentProblem.getFeasible())
            continue;

        // Verifica si la solución actual es optimo
        if (currentProblem.getOptimal() && currentProblem.getObjectiveValue() > bestObjectiveValue)
        {
            bestObjectiveValue = currentProblem.getObjectiveValue();
            bestSolution = currentSolution;
            break;
        }

        int worstVariableIndex = -1;
        float worstValue = 0.0;

        // Encuentra la variable con la fracción más grande
        for (int i = 1; i < currentSolution.size(); ++i)
        {
            if (fabs(currentSolution[i] - round(currentSolution[i])) > worstValue) // fabs = valor absoluto
            {
                worstVariableIndex = i;
                worstValue = fabs(currentSolution[i] - round(currentSolution[i]));
            }
        }

        // Si no hay variables con fracciones, entonces el problema actual ya tiene una solución entera
        if (worstVariableIndex != -1)
        {
            addBranchProblems(currentProblem, worstVariableIndex, currentSolution[worstVariableIndex]);
        }
    }

    if (bestSolution.empty())
    {
        cout << "No solution found\n";
        return {};
    }
    else
    {
        return bestSolution;
    }
}
