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

    // Resuelve los problemas
    p1.solve();
    p2.solve();

    if (p1.getFeasible() && p1.getObjectiveValue() >= p1.getLowerBound())
    {
        Live.push(p1);
    }
    if (p2.getFeasible() && p2.getObjectiveValue() >= p2.getLowerBound())
    {
        Live.push(p2);
    }
}

/*
 * Resuelve el problema original por el método de Branch and Bound
 * @param originalProblem: Problema original
 * @return: Solución del problema original
 */
vector<float> Branch::solve(Simplex &originalProblem)
{

    // Se hace de esta manera para si o si tener una solución
    vector<float> bestSolution;
    vector<float> currentSolution = originalProblem.solve();
    // Se agrega el problema original al vector de problemas
    Live.push(originalProblem);

    while (!Live.empty())
    {
        // Extrae el primer elemento de la cola
        Simplex currentProblem = Live.top();
        Live.pop();

        // Se obtiene la solución
        currentSolution = currentProblem.getSolution();

        // Verifica si el problema actual es factible
        if (!currentProblem.getFeasible())
            continue;

        // Verifica si la solución actual es optimo
        if (currentProblem.getOptimal())
        {
            bestSolution = currentSolution;
            return bestSolution;
        }

        int worstVariableIndex = -1;
        float worstDifference = 0.0;

        // Encuentra la variable con la fracción más cercana a 0.5, es decir, más lejos de los enteros
        for (int i = 1; i < currentSolution.size(); ++i)
        {
            float parteFraccionaria = currentSolution[i] - floor(currentSolution[i]);
            // Calcula la distancia de la parte fraccionaria a los enteros más cercanos (0 o 1)
            float diferencia = min(parteFraccionaria, 1 - parteFraccionaria);
            // Si la parte fraccionaria no es cero y la diferencia es mayor que la peor diferencia encontrada hasta ahora
            if (parteFraccionaria != 0.0 && diferencia > worstDifference)
            {
                worstDifference = diferencia;
                worstVariableIndex = i;
            }
        }

        // Si no hay variables con fracciones, entonces el problema actual ya tiene una solución entera
        if (worstVariableIndex != -1)
        {
            addBranchProblems(currentProblem, worstVariableIndex, (int)currentSolution[worstVariableIndex]);
        }
    }
    cout << "No solution found" << endl;
    return {};
}

vector<float> Branch::greedySolution(Simplex &problem)
{
    vector<float> weights = problem.getWeights();
    vector<float> values = problem.getObjectiveCoefficients();
    float maxWeight = problem.getMaxWeight();

    vector<int> indices(weights.size());
    iota(indices.begin(), indices.end(), 0);

    // Ordenar los índices de los ítems por la relación valor/peso en orden descendente
    sort(indices.begin(), indices.end(), [&](int a, int b)
         { return (values[a] / weights[a]) > (values[b] / weights[b]); });

    vector<float> solution(weights.size(), 0.0);
    float currentWeight = 0.0;
    float totalValue = 0.0;

    for (int i : indices)
    {
        if (currentWeight + weights[i] <= maxWeight)
        {
            solution[i] = 1.0; // Tomar el ítem completo
            currentWeight += weights[i];
            totalValue += values[i];
        }
        else
        {
            // Si el ítem no cabe completamente, puedes agregar una fracción de él.
            float fraction = (maxWeight - currentWeight) / weights[i];
            solution[i] = fraction;
            totalValue += values[i] * fraction;
            break; // La mochila está llena
        }
    }

    // `totalValue` es la suma de valores de ítems seleccionados.
    return solution;
}

// Método para agregar una restricción basada en la solución Greedy
void Branch::addGreedyConstraint(Simplex &problem)
{
    vector<float> greedySolution = greedySolution(problem);
    float V_greedy = accumulate(greedySolution.begin(), greedySolution.end(), 0.0f);

    // Crear una nueva restricción que garantiza que el valor total es al menos V_greedy
    vector<float> coefficients = problem.getObjectiveCoefficients();
    vector<float> newConstraint(coefficients.size(), 0);
    for (int i = 0; i < coefficients.size(); ++i)
    {
        newConstraint[i] = coefficients[i];
    }

    problem.addConstraint(newConstraint, V_greedy);
}
