#ifndef COMPLEX_HPP
#define COMPLEX_HPP

/*!
*   \file       Complex.hpp
*   \brief      Clase para almacenar datos tipo complejos.
*   \details    Una clase simple que contine dos variables tipo double.
*   \author     Daniel Reyes Barrera
*   \version    1.0
*   \date       2021
*   \copyright  GNU Public License.
*/

//! Clase Complex
/*!
Esta clase contiene dos variables tipo double para representar un numero complejo.
*/
class Complex
{
    public:
        //! Parte real
        float Re = 0;
        //! Parte imaginaria
        float Im = 0;
};

#endif // COMPLEX_HPP