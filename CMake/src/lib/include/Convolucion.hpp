#ifndef CONCOLUCION_HPP
#define CONVOLUCION_HPP

#include <cmath>
#include <Matriz.hpp>
#include <fstream>
#include <iostream>

class Convolucion
{
    public:


        Matriz* leer_imagenes(std::string, int&, int&);

        int* leer_etiquetas(std::string, int&);

        float* OneHotLabel(int x);

        Matriz Conv2Db(Matriz* input, Matriz* deltas);

        Matriz Conv2D(Matriz*, Matriz*, int s);

        Matriz fullConv2D(Matriz* filtros, Matriz* deltas);

        float Relu(float x);

        Matriz activation(Matriz matriz);

        Matriz pooling(Matriz, int, int); 

        Matriz MaxPooling(Matriz matriz, int p, int f, int s);

        Matriz MaxPooling_inv(Matriz matriz, Matriz* max_index_matriz , int p, int f, int s);

        float* flatten(Matriz matriz, int* N);

        Matriz flatten_inv(float* vec, int len_vec, int f);

        Matriz Mult_dRelu(Matriz* M, Matriz* z);
};

#endif // CONVOLUCION_HPP