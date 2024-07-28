#include "Branch.h"

vector<float> Branch::solve(Simplex &originalProblem)
{
    Live.push(originalProblem);
    float bestObjectiveValue = numeric_limits<float>::infinity();
    vector<float> bestSolution;

    while (!Live.empty())
    {
        Simplex currentProblem = Live.front();
        Live.pop();

        vector<float> currentSolution = currentProblem.solve();

        if (!currentProblem.getFeasible())
            continue;

        if (currentProblem.getOptimal() && currentProblem.getObjectiveValue() < bestObjectiveValue)
        {
            bestObjectiveValue = currentProblem.getObjectiveValue();
            bestSolution = currentSolution;
        }

        if (currentProblem.getFeasible() && !currentProblem.getOptimal())
        {
            int worstVariableIndex = -1;
            float worstValue = 0.0;

            for (int i = 1; i < currentSolution.size(); ++i)
            {
                if (fabs(currentSolution[i] - round(currentSolution[i])) > worstValue)
                {
                    worstVariableIndex = i;
                    worstValue = fabs(currentSolution[i] - round(currentSolution[i]));
                }
            }

            if (worstVariableIndex != -1)
            {
                addBranchProblems(currentProblem, worstVariableIndex, currentSolution[worstVariableIndex]);
            }
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
