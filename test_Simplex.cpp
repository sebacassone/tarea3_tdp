#include <iostream>
#include "Simplex.h"

using namespace std;

int main()
{
    Simplex s("P2.txt");
    vector<float> sol = s.solve();
    if (sol.empty())
    {
        cout << "No solution found" << endl;
        return 0;
    }
    for (auto x : sol)
    {
        cout << x << " ";
    }
    cout << endl;
}