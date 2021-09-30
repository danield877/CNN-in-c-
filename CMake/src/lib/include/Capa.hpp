#ifndef CAPA_HPP
#define CAPA_HPP

#include <cmath>
#include <Matriz.hpp>
#include <fstream>
#include <iostream>

class Capa
{
    public:

        int output_shape;
        int input_shape;

        Capa(int input_shape, int output_shape, float* (*func)(float*, int));

        float* Cal_Z(float* input);

        float* (*function)(float*, int);

        float* (*function_d)(float*, int);

        float Relu(float x);

        float* apl_ReLU(float* vec);

        float* SoftMax(float* vec);

        float* WnMultDelta(float* delta);

        void Update(float lr);


        float* error(float* p, float* y);

        float* reul_d(float* vec);

        float* ActFuntion(float* input);


        Matriz *Wns;

        float* delta;

        float* input;

        float* output;
 
};

#endif // NN_HPP