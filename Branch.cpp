#include "Branch.h"

/*
* Agrega dos problemas a la cola de problemas (Dos problemas para ramificar
por el método de Branch and Bound)
* @param problem: Problema actual
* @param variableIndex: Índice de la variable con fracción
* @param value: Valor de la variable con fracción
*/
void Branch::addBranchProblems(Simplex &problem, int variableIndex, int value)
{
    // Se clonan los problemas
    Simplex p1 = problem.copy();
    Simplex p2 = problem.copy();

    // Agregar restricciones para ramificar
    p1.insertConstraint(value, variableIndex, 1);     // x <= int(value)
    p2.insertConstraint(value + 1, variableIndex, 2); // x >= int(value) + 1

    Live.push(p1);
    Live.push(p2);
}

/*
 * Resuelve el problema original por el método de Branch and Bound
 * @param originalProblem: Problema original
 * @return: Solución del problema original
 */
vector<float> Branch::solve(Simplex &originalProblem)
{
    // Se agrega el problema original al vector de problemas
    Live.push(originalProblem);
    // Se hace de esta manera para si o si tener una solución
    float bestObjectiveValue = -numeric_limits<float>::max();
    vector<float> bestSolution;

    while (!Live.empty())
    {
        // Extrae el primer elemento de la cola
        Simplex currentProblem = Live.top();
        Live.pop();

        // Resuelve el problema actual
        vector<float> currentSolution = currentProblem.solve();

        // Verifica si el problema actual es factible
        if (!currentProblem.getFeasible())
            continue;

        // Verifica si la solución actual es optimo
        if (currentProblem.getOptimal() &&
            currentProblem.getObjectiveValue() > bestObjectiveValue)
        {
            bestSolution = currentSolution;
            bestObjectiveValue = currentProblem.getObjectiveValue();
            return bestSolution;
        }

        int worstVariableIndex = -1;
        float worstValue = 0.0;

        // Encuentra la variable con la fracción más grande
        for (int i = 1; i < currentSolution.size(); ++i)
        {
            int parteEntera = currentSolution[i];
            float parteFraccionaria = currentSolution[i] - parteEntera;
            // Si la parte fraccionaria no es cero entonces es una variable con fracción
            if (parteFraccionaria != 0.0 && parteFraccionaria > worstValue)
            {
                worstValue = parteFraccionaria;
                worstVariableIndex = i;
            }
        }

        // Si no hay variables con fracciones, entonces el problema actual ya tiene una solución entera
        if (worstVariableIndex != -1)
        {
            addBranchProblems(currentProblem, worstVariableIndex, (int)currentSolution[worstVariableIndex]);
        }
    }
    // Si la solución está vacía, entonces no se encontró solución
    if (bestSolution.empty())
    {
        cout << "No se encontró solución\n";
        return {};
    }
    else
    {
        return bestSolution;
    }
}
