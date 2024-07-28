#include "Simplex.h"

/*
    Metodo: Constructor
    Descripcion: este metodo permite iniciar un Simplex, recibiendo
        el nombre de un archivo, el cual contendra los datos necesarios
        para la creacion de el Simplex.
    Parametros:
        -filename: nombre del archivo que contendra los datos.
    Retorno: El objeto creado.
*/
Simplex::Simplex(std::string fileName)
{
    std::string line;
    std::fstream txtStream(fileName);
    std::stringstream charStream;
    std::string stringNumber;

    if (!txtStream.is_open())
    {
        throw std::invalid_argument("El archivo ingresado no existe.");
    }

    // Se lee la primera linea del archivo, la cual contiene los valores de m1, m2 y m3
    getline(txtStream, line); // m1 es la cantidad de restricciones de tipo 1 (<=), m2 es la cantidad de restricciones de tipo 2 (>=) y m3 es la cantidad de restricciones de tipo 3 (==)
    charStream << line;
    getline(charStream, stringNumber, ' '); // Se obtiene el valor de m1
    this->m1 = stoi(stringNumber);
    getline(charStream, stringNumber, ' '); // Se obtiene el valor de m2
    this->m2 = stoi(stringNumber);
    getline(charStream, stringNumber, ' '); // Se obtiene el valor de m3
    this->m3 = stoi(stringNumber);

    // Se lee el resto del archivo, el cual contiene los valores de la matriz
    while (getline(txtStream, line))
    {
        // Se obtiene una fila de la matriz
        charStream.clear();
        charStream << line;          // Se convierte la linea a un stringstream
        std::vector<float> function; // Se crea un vector para guardar la fila
        while (getline(charStream, stringNumber, ' '))
        {
            function.push_back(stof(stringNumber)); // Se guarda el valor en el vector
        }
        this->a.push_back(function); // Se guarda la fila en la matriz
    }
    this->n = a[0].size() - 1; // Se obtiene el numero de variables
    this->m = m1 + m2 + m3;
    if (m != (int)a.size() - 1)
    {
        throw std::invalid_argument("El numero de restricciones ingresados no coincide con la matriz dada.");
    }

    for (int i = 1; i <= m; i++)
    {
        if (a[i][0] < 0.0 || (int)a[i].size() != n + 1)
        {
            // Error al crear el Simplex, ya que no se permiten
            // constantes negativas para las restricciones, o el
            // numero de columnas no coincide para todas las filas.
            throw std::invalid_argument("La matriz ingresada no es valida.");
        }
    }

    std::vector<float> function;
    for (int i = 0; i <= n; i++)
    {
        function.push_back(0.0);
    }

    this->a.push_back(function);
    this->initialA = a;
    this->isSolved = false;
    this->isFeasible = false;
    this->isOptimal = false;
    txtStream.close();
}

/*
    Metodo: Constructor
    Descripcion: este metodo permite iniciar un Simplex, recibiendo
        los datos necesarios para la creacion de este.
    Parametros:
        -a: Matriz con los valores que acompanian a la funcion a maximizar
            y a las restricciones del problema a resolver.
        -m1: numero de restricciones de tipo 1 (<=)
        -m2: numero de restricciones de tipo 2 (>=)
        -m3: numero de restricciones de tipo 3 (==)
    Retorno: El objeto creado.
*/
Simplex::Simplex(std::vector<std::vector<float>> a, int m1, int m2, int m3)
{
    int m = m1 + m2 + m3;
    for (int i = 1; i <= m; i++)
    {
        if (a[i][0] < 0.0)
        {
            // Error al crear el Simplex, ya que no se permiten
            // constantes negativas para las restricciones.
            throw std::invalid_argument("La matriz ingresada no es valida.");
        }
    }

    this->initialA = a;
    this->a = a;
    this->m1 = m1;
    this->m2 = m2;
    this->m3 = m3;
    this->m = m;
    this->n = a[0].size() - 1;
    this->isSolved = false;
    this->isFeasible = false;
    this->isOptimal = false;
}

/*
    Descripcion: este metodo permite eliminar el objeto.
    Parametros: no posee.
    Retorno: no posee.
*/
Simplex::~Simplex()
{
}

/*
    Descripcion: este metodo permite resolver el problema de programacion
        lineal invocando al metodo de simplex.
    Parametros: No posee.
    Retorno:
       -Si encuntra solucion: Arreglo con el valor maximizado y los
        valores de los parametros de la funcion a maximizar.
       -Si NO encuntra solucion: Arreglo vacio.

*/
std::vector<float> Simplex::solve()
{
    // Si ya se resolvio el problema, se retorna la solucion
    icase = simplx(); //
    isSolved = true;
    // Si es la solución optima, cambiar el parámetro
    if (icase != 0)
    {
        std::vector<float> emptyVector;
        isFeasible = false;
        return emptyVector; // No hay solucion
    }
    isFeasible = true;

    std::vector<float> parameters(n + 1, 0.0);
    parameters[0] = a[0][0];
    for (int i = 0; i < m; i++)
    {
        if (iposv[i] < n)
        {
            parameters[iposv[i] + 1] = a[i + 1][0];
        }
    }
    solution = parameters;

    return parameters;
}

/*
    Descripcion: este metodo resuelve el problema de maximizacion
        dado la funcion y las restricciones dadas en la creacion
        del objeto, todo mediante el metodo simplex.
    Parametros: No posee.
    Retorno:
        - 0: Fue posible resolver el problema.
        - 1: No existe un limite para la funcion a maximizar.
        - -1: No existe solucion que cumpla con las restricciones dadas.
*/
int Simplex::simplx()
{
    int i, ip, is, k, kh, kp, nl1;
    float q1, bmax;
    std::vector<int> l1;
    std::vector<int> l3;
    nl1 = n;
    izrov.clear();
    iposv.clear();
    // Se inicializan los vectores izrov e iposv y se llenan con los valores iniciales
    for (k = 0; k < n; k++)
    {
        l1.push_back(k);
        izrov.push_back(k);
    }
    // Se llena el vector l1 con los valores de las restricciones
    for (i = 0; i < m; i++)
    {
        iposv.push_back(n + i);
    }
    // Si no hay restricciones de tipo 3, se salta este paso
    if (m2 + m3)
    {
        // Se llena el vector l3 con los valores de las restricciones de tipo 3
        for (i = 0; i < m2; i++)
        {
            l3.push_back(1);
        }
        // Se llena el vector l3 con los valores de las restricciones de tipo 2
        for (k = 0; k < n + 1; k++)
        {
            // Se busca si el parametro k esta en las restricciones de tipo 2
            q1 = 0.0;
            for (i = m1; i < m; i++)
            {
                q1 += a[i + 1][k];
            }
            a[m + 1][k] = -q1;
        }
        // Se busca el valor maximo de la fila 0
        for (;;)
        {
            // Se busca el valor maximo de la fila 0
            maxValue(m + 1, l1, nl1, 0, &kp, &bmax);
            if (bmax <= EPS && a[m + 1][0] < -EPS)
            {
                return -1; // icase -1
            }
            else if (bmax <= EPS && a[m + 1][0] <= EPS)
            {
                for (ip = m1 + m2; ip < m; ip++)
                {
                    if (iposv[ip] == ip + n)
                    {
                        maxValue(ip, l1, nl1, 1, &kp, &bmax);
                        if (bmax > EPS)
                        {
                            goto one;
                        }
                    }
                }
                for (i = m1; i < m1 + m2; i++)
                {
                    if (l3[i - m1] == 1)
                    {
                        for (k = 0; k < n + 1; k++)
                        {
                            a[i + 1][k] = -a[i + 1][k];
                        }
                    }
                }
                break;
            }
            // Se busca el elemento pivote
            locatePivot(&ip, kp);
            if (ip == -1)
            {
                return -1; // icase -1
            }
        one: // Lo que hace esta parte con one es que si se cumple la condicion de que bmax <= EPS y a[m + 1][0] < -EPS
            exchangeParameters(m + 1, n, ip, kp);
            if (iposv[ip] >= (n + m1 + m2))
            {
                for (k = 0; k < nl1; k++)
                {
                    if (l1[k] == kp)
                    {
                        break;
                    }
                }
                --nl1;
                for (is = k; is < nl1; is++)
                {
                    l1[is] = l1[is + 1];
                }
            }
            // Se busca el elemento pivote
            else
            {
                kh = iposv[ip] - m1 - n;
                if (kh >= 0 && l3[kh])
                {
                    l3[kh] = 0;
                    ++a[m + 1][kp + 1];
                    for (i = 0; i < m + 2; i++)
                    {
                        a[i][kp + 1] = -a[i][kp + 1];
                    }
                }
            }
            is = izrov[kp];
            izrov[kp] = iposv[ip];
            iposv[ip] = is;
        }
    }
    // Este ciclo for se encarga de encontrar la solucion
    for (;;)
    {
        // Se busca el elemento con mayor valor en la fila 0
        maxValue(0, l1, nl1, 0, &kp, &bmax);
        // Si el valor es menor o igual a EPS, se ha encontrado la solucion
        if (bmax <= EPS)
        {
            return 0; // icase = 0
        }
        // Se busca el elemento pivote
        locatePivot(&ip, kp);
        if (ip == -1)
        {
            return 1; // icase  = 1
        }
        // Se intercambian los parametros
        exchangeParameters(m, n, ip, kp);
        is = izrov[kp];
        izrov[kp] = iposv[ip];
        iposv[ip] = is;
    }
}

/*
    Descripcion: este metodo calcula el valor mayor de los elementos
        de la fila mm, cuyo indice esta en el vector ll dado (corrido en 1 posicion
        ya que el primer elemento corresponde a una constante y los valores se
        estan guardando en base a los parametros x1, x2 ...), guardando el valor
        en bmax y el indice en kp, esto considerando el valor absoluto del valor
        si se entrega iabf distinto de 0, en otro caso se toma el valor tal cual.
    Parametros:
        -mm: fila a revisar de la matriz.
        -ll: vector con las posciones de la columna a revisar en la matriz.
        -nll: largo del vector ll
        -iabf: valor que indica si se desea calcular con el valor absoluto o no.
        -kp: direccion donde se guardara la posicion del elemento con mayor valor.
        -bmax: direccion donde se guardara el valor maximo encontrado.
    Retorno: No posee.
*/
void Simplex::maxValue(int mm, std::vector<int> ll, int nll, int iabf, int *kp, float *bmax)
{
    int k;
    float test;

    if (nll <= 0)
    {
        *bmax = 0.0;
    }
    else
    {
        *kp = ll[0];
        *bmax = a[mm][*kp + 1];
        for (k = 1; k < nll; k++)
        {
            if (iabf == 0)
            {
                test = a[mm][ll[k] + 1] - (*bmax);
            }
            else
            {
                test = fabs(a[mm][ll[k] + 1]) - fabs(*bmax);
            }

            if (test > 0.0)
            {
                *bmax = a[mm][ll[k] + 1];
                *kp = ll[k];
            }
        }
    }
}

/*
    Descripcion: este metodo encuentra la fila en donde se
        encuentra el elemento que sirve como pivote.
    Parametros:
        -ip: direccion donde se guardara la fila del elemento pivote.
        -kp: numero de la columna en donde encontrar el pivote.
    Retorno: No posee.
*/
void Simplex::locatePivot(int *ip, int kp)
{

    int k, i;
    float qp, q0, q, q1;

    kp = kp + 1;
    *ip = -1;
    for (i = 0; i < m; i++)
    {
        if (a[i + 1][kp] < -EPS)
        {
            break;
        }
    }

    if (i + 1 > m)
    {
        return;
    }

    q1 = -a[i + 1][0] / a[i + 1][kp];
    *ip = i;
    for (i = *ip + 1; i < m; i++)
    {
        if (a[i + 1][kp] < -EPS)
        {
            q = -a[i + 1][0] / a[i + 1][kp];
            if (q < q1)
            {
                *ip = i;
                q1 = q;
            }
            else if (q == q1)
            {
                qp = -1;
                q0 = -1;
                for (k = 0; k < n; k++)
                {
                    qp = -a[*ip + 1][k + 1] / a[*ip + 1][kp];
                    q0 = -a[i + 1][k + 1] / a[i + 1][kp];
                    if (q0 != qp)
                    {
                        break;
                    }
                }
                if (q0 < qp)
                {
                    *ip = i;
                }
            }
        }
    }
}

/*
    Descripcion: este metodo realiza operaciones matriciales, para
        intercambiar un parametro perteneciente a la restricciones m3
        con los parametros restantes.
    Parametros:
        -i1: numero de filas a explorar.
        -k1: numero de columnas a explorar.
        -ip: numero de la fila del pivote.
        -kp: numero de la columna del pivote.
    Retorno: No posee.
*/
void Simplex::exchangeParameters(int i1, int k1, int ip, int kp)
{
    int kk, ii;
    float piv;

    piv = 1.0 / a[ip + 1][kp + 1];
    for (ii = 0; ii < i1 + 1; ii++)
    {
        if (ii - 1 != ip)
        {
            a[ii][kp + 1] *= piv;
            for (kk = 0; kk < k1 + 1; kk++)
            {
                if (kk - 1 != kp)
                {
                    a[ii][kk] -= a[ip + 1][kk] * a[ii][kp + 1];
                }
            }
        }
    }

    for (kk = 0; kk < k1 + 1; kk++)
    {
        if (kk - 1 != kp)
        {
            a[ip + 1][kk] *= -piv;
        }
    }

    a[ip + 1][kp + 1] = piv;
}

/*
    Descripcion: este metodo calcula el valor absoluto de un
        valor dado.
    Parametros:
        -x: valor a calcular el valor absoluto
    Retorno: valor absoluto de x.
*/
float Simplex::fabs(float x)
{
    if (x < 0.0)
    {
        return x * -1.0;
    }

    return x;
}

/*
    Descripcion: este metodo permite insertar una restriccion
        a la matriz que representa el problema.
    Parametros:
        -b: valor constante de la restriccion.
        -var: variable/parametro a la que se agrega la restriccion
        -type: tipo de la restriccion, este puede ser:
            .1: var <= b
            .2: var >= b
            .3: var == b
    Retorno: No posee.
*/
void Simplex::insertConstraint(float b, int var, int type)
{
    if (var == 0 || var > n || b < 0.0)
    {
        return;
    }

    std::vector<float> constraint(n + 1, 0.0);
    constraint[0] = b;
    constraint[var] = -1.0;

    switch (type)
    {
    case 1: // var <= b
        this->initialA.insert(this->initialA.begin() + m1 + 1, constraint);
        m1++;
        break;
    case 2: // var >= b
        this->initialA.insert(this->initialA.begin() + m1 + m2 + 1, constraint);
        m2++;
        break;
    case 3: // var == b
        this->initialA.insert(this->initialA.begin() + m + 1, constraint);
        m3++;
        break;
    default:
        return;
        break;
    }
    m++;
    isSolved = false;
    a = initialA;
    solution.clear();
}

/*
    Descripcion: este metodo permite copiar el objeto con sus
        respectivos valores.
    Parametros: No tiene.
    Retorno: El objeto creado con los valores
        del objeto copiado.
*/
Simplex Simplex::copy()
{
    Simplex s = Simplex(initialA, m1, m2, m3);
    s.izrov = this->izrov;
    s.iposv = this->iposv;
    s.isSolved = this->isSolved;
    s.icase = this->icase;
    s.a = this->a;
    s.solution = this->solution;
    return s;
}

std::vector<float> Simplex::getSolution()
{
    if (isSolved && icase == 0)
    {
        return solution;
    }
    std::vector<float> empty;
    return empty;
}

/*
    Descripcion: este metodo permite imprimir la matriz
        que representa al problema.
    Parametros: No tiene.
    Retorno: No tiene.
*/
void Simplex::printProblemMatrix()
{
    for (std::size_t i = 0; i < initialA.size(); i++)
    {
        for (std::size_t j = 0; j < initialA[0].size(); j++)
        {
            std::string aij = std::to_string(initialA[i][j]);
            aij = aij.substr(0, aij.find(".") + 3);
            aij.insert(aij.begin(), 10 - aij.length(), ' ');
            std::cout << aij;
        }
        std::cout << std::endl;
    }
}

/*
    Descripcion: este metodo permite imprimir la matriz solucion,
        esto si es que fue resuelto en problema.
    Parametros: No tiene.
    Retorno: No tiene.
*/
void Simplex::printSolution()
{
    if (isSolved)
    {
        int nm1m2;
        if (icase == 1)
        {
            std::cout << "No existe limite para la funcion a maximizar." << std::endl;
        }
        else if (icase == -1)
        {
            std::cout << "No hay solucion que cumpla con las restricciones." << std::endl;
        }
        else
        {
            nm1m2 = n + m1 + m2;
            std::string txt[nm1m2];
            for (int i = 0; i < n; i++)
            {
                txt[i] = "x" + std::to_string(i + 1);
            }
            for (int i = n; i < nm1m2; i++)
            {
                txt[i] = "y" + std::to_string(i + 1 - n);
            }

            std::cout << std::string(11, ' ');
            for (int i = 0; i < n; i++)
            {
                if (izrov[i] < nm1m2)
                {
                    txt[izrov[i]].insert(txt[izrov[i]].begin(), 10 - txt[izrov[i]].length(), ' ');
                    std::cout << txt[izrov[i]];
                }
            }

            std::cout << std::endl;
            for (int i = 0; i < m + 1; i++)
            {
                if (i == 0 || iposv[i - 1] < nm1m2)
                {
                    if (i > 0)
                    {
                        std::cout << txt[iposv[i - 1]];
                    }
                    else
                    {
                        std::cout << "  ";
                    }

                    std::string ai0 = std::to_string(a[i][0]);
                    ai0 = ai0.substr(0, ai0.find(".") + 3);
                    ai0.insert(ai0.begin(), 10 - ai0.length(), ' ');
                    std::cout << ai0;
                    for (int j = 1; j < n + 1; j++)
                    {
                        if (izrov[j - 1] < nm1m2)
                        {
                            std::string aij = std::to_string(a[i][j]);
                            aij = aij.substr(0, aij.find(".") + 3);
                            aij.insert(aij.begin(), 10 - aij.length(), ' ');
                            std::cout << aij;
                        }
                    }
                    std::cout << std::endl;
                }
            }
        }
    }
    else
    {
        std::cout << "No esta resuelto." << std::endl;
    }
}