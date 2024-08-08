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
    vector<float> bestSolution = originalProblem.solve();
    // Se agrega una restricción greedy para obtener una cota superior
    vector<float> costosOriginal = originalProblem.getObjectiveCoefficients();
    vector<float> pesosOriginal = originalProblem.getWeights();
    vector<float> greedySolution = greedy(originalProblem, costosOriginal, pesosOriginal);
    float greedyValue = originalProblem.calculateObjectiveValue(greedySolution);
    if (greedyValue > 0)
    {
        originalProblem.insertComplexConstraint(greedySolution, greedyValue, 2);
        // Agrega el calculo de la función objetivo al inicio del vector
        greedySolution.insert(greedySolution.begin(), greedyValue);
    }
    // Se resuelve el problema original
    vector<float> currentSolution;
    originalProblem.solve();
    // Se agrega el problema original al vector de problemas
    Live.push(originalProblem);

    while (!Live.empty())
    {
        // Extrae el primer elemento de la cola
        Simplex currentProblem = Live.top();
        Live.pop();

        // Se obtiene la solución
        currentSolution = currentProblem.getSolution();

        if (!currentProblem.getFeasible() && currentProblem.calculateObjectiveValue(greedySolution) && bestSolution[0])
            return greedySolution;

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

// Implementación del algoritmo codicioso
vector<float> Branch::greedy(const Simplex &problem, const vector<float> &costos, const vector<float> &pesos)
{
    priority_queue<Element> elements;
    vector<Element> solutionGreedy;

    // Crear una lista de elementos con valores, pesos e índices
    for (int i = 0; i < costos.size(); ++i)
    {
        elements.push(Element(fabs(costos[i]), fabs(pesos[i]), i));
    }

    // Seleccionar elementos hasta que no se pueda agregar más
    float totalWeight = 0;

    while (!elements.empty())
    {
        Element element = elements.top();
        elements.pop();

        // Si el peso total más el peso del elemento es menor o igual al peso máximo
        if (totalWeight + element.weight <= problem.getMaxWeight())
        {
            solutionGreedy.push_back(element);
            totalWeight += element.weight;
        }
    }

    // Construye un vector solución
    vector<float> solutionVector(costos.size(), 0.0);
    for (Element element : solutionGreedy)
    {
        solutionVector[element.indice] = 1.0;
    }

    return solutionVector;
}
