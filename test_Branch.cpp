#include "Branch.h"

int main()
{
    Simplex s("P2.txt");
    Branch solver;
    std::vector<float> solution = solver.solve(s);

    if (!solution.empty())
    {
        std::cout << "Optimal solution found:\n";
        for (float x : solution)
        {
            std::cout << x << " ";
        }
        std::cout << "\n";
    }
    else
    {
        std::cout << "No solution found\n";
    }
    return 0;
}