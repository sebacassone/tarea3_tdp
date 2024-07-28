#include <iostream>
#include "Simplex.h"

using namespace std;

int main()
{
    Simplex s("P2.txt");
    vector<float> sol = s.solve();
    for (auto x : sol)
    {
        cout << x << " ";
    }
    cout << endl;
    s.solve();
}