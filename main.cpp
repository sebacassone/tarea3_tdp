#include "Branch.h"
#include <chrono> // Para medir el tiempo de ejecución
using namespace std::chrono;

int main()
{
    // hace un menú que 1 sea para elegir archivo y 2 para salir, solo resuelve el branch and bound
    int option;
    while (true)
    {
        cout << "1. Elegir archivo\n2. Salir\n";
        cin >> option;
        if (option == 1)
        {
            string file_name;
            cout << "Ingrese el nombre del archivo: ";
            cin >> file_name;
            Simplex s(file_name);
            Branch solver;
            // mide el tiempo
            auto start = high_resolution_clock::now();
            vector<float> solution = solver.solve(s);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(stop - start);
            if (!solution.empty())
            {
                cout << "Optimal solution found:\n";
                for (float x : solution)
                {
                    cout << x << " ";
                }
                cout << "\n";
            }
            else
            {
                cout << "Solución no encontrada" << endl;
            }
            // Imprime el tamaño del máximo clique
            cout << "Tiempo de ejecución: " << duration.count() << " milisegundos" << endl;
        }
        else if (option == 2)
        {
            break;
        }
        else
        {
            cout << "Opción no válida\n";
        }
    }
}