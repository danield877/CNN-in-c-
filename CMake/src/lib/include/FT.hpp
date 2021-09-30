#ifndef FT_HPP
#define FT_HPP

#include <cmath>
#include <Matriz.hpp>

/*!
*   \file       FT.hpp
*   \brief      Clase FT.
*   \details    Una clase para hallar la FT de una matriz compleja.
*   \author     Daniel Reyes Barrera
*   \version    1.0
*   \date       2021
*   \copyright  GNU Public License.
*/

//! Clase FT
/*!
Esta clase contiene funciones relacionada con la DFT.
*/
class FT
{
    public:

        //! Calcula la FT de una matriz.
        /*!
        \param [in] mstriz matriz que se desea hallar su DFT.
        */
        Matriz DFT_2d(Matriz);

        //! Calcula la DFT inversa de una matriz compleja.
        /*!
        \param [in] mstriz matriz compleja que se desea hallar su DFT inversa.
        */
        Matriz DFT_2d_inv(Matriz);

        //! Calcula la densidad espectral de una matriz.
        /*!
        \param [in] mstriz matriz que se desea hallar su densidad espectral.
        */
        Matriz SpectralDensity(Matriz);
        
        //! Calcula el espectro de fourier de una matriz.
        /*!
        \param [in] mstriz matriz que se desea hallar su espectro de fourier.
        */
        Matriz Spectrum(Matriz);

        Complex* Calcular_Wn(int log2_N);
        
        void FFT_1d(Complex* array, int N, Complex* Wn);

        Complex* DFT_1d(Complex* array, int N);

        unsigned int bitReverse(unsigned int x, int log2n);

        void OrderBitReverse(Complex* array, int N, int log2n);

        void inv_FFT_1d(Complex* array, int N, Complex* Wn);

        Matriz FFT_2d(Matriz* matriz, int N, Complex* Wn);
        
};

#endif // FT_HPP