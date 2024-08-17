#ifndef COMPARE_H
#define COMPARE_H
#include "Simplex.h"

/*
 * Compare estruct:
 * Esta estructura compara dos Simplex por su valor de la función objetivo y su cota inferior.
 */
struct Compare
{
    bool operator()(const Simplex &lhs, const Simplex &rhs) const
    {
        // Primero compara los valores de la solución
        if (lhs.getObjectiveValue() != rhs.getObjectiveValue())
        {
            return lhs.getObjectiveValue() < rhs.getObjectiveValue(); // Mayor valor al frente
        }
        return lhs.getLowerBound() < rhs.getLowerBound(); // Mayor cota inferior al frente
    }
};
#endif