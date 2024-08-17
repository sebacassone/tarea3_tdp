#ifndef ELEMENT_H
#define ELEMENT_H

/*
 * Element estruct:
 * Esta estructura representa un elemento con un valor, un peso y un índice.
 */
struct Element
{
    float value;  // Valor del elemento
    float weight; // Peso del elemento
    int indice;   // Indice del elemento

    Element(float v, float w, int i) : value(v), weight(w), indice(i) {}
    // Definir el operador <
    bool operator<(const Element &other) const
    {
        return this->value < other.value;
    }
};

#endif
