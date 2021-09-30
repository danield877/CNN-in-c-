#ifndef MATRIZ_HPP
#define MATRIZ_HPP

#include <iostream>
#include <iomanip>
#include <cmath>
#include <Complex.hpp>


/*!
*   \file       Matriz.hpp
*   \brief      Clase para definir matrices.
*   \details    Una clase define y opera con matrices complejas.
*   \author     Daniel Reyes Barrera
*   \version    1.0
*   \date       2021
*   \copyright  GNU Public License.
*/

//! Clase Matriz
/*!
Clase que define matrices y sus operaciones
*/
class Matriz
{

    public:

        //! Default constructor
        /*!
        \param [in] m dimension m de la matriz.
        \param [in] n dimension n de la matriz.
        \param [in] complex corresponda si los coeficientes de la matriz son complejos o no. False pot defecto.
        */
        Matriz(){};

        Matriz(int dim);

        Matriz(int m, int n, bool complex = false );

        Matriz(int matrices, int m, int n, bool complex);

        Matriz(int matrices, int chanels, int m, int n, bool complex);

        //! Inicializa los coeficientes de la matriz en valores aleatorios.
        void random();

        void addComplex();

        //! Muestra los coeficientes de la matriz.
        /*!
        \param [in] x valor que representa los coeficientes a mostrar (reales o imaginarios). 0 por defectro que corresponde a valores reales.
        */
        void show(int x = 0);

        //! Muestra los coeficientes de la matriz.
        /*!
        \param [in] x valor que representa los coeficientes a mostrar (reales o imaginarios). 0 por defectro que corresponde a valores reales.
        */
        

        Matriz pading(int, int x);

        void normalize(float n);

        Matriz copy();

        Matriz dot(Matriz filtro);

        void matmult(Matriz matriz);

        void add(Matriz* matriz, float lr);

        Matriz rot180();

        //! Variables que almacenan las dimensiones de la matriz
        int dim_matriz_m;
        int dim_matriz_n;

        int N_matrices = 1;
        int N_vectores = 0;

        //! Puntero que apunta a una matriz con coeficientes reales.
        float ** M;

        //! Puntero que apunta a una matriz con coeficientes complejos.
        Complex ** MC;

        Complex * vector;

        Matriz ** Matrices;

        int N_chanels = 1;

        //! Booleano que alamacena si la matriz es compleja o no.
        bool complx = false;

};

#endif // MATRIZ_HPP
