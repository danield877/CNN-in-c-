#ifndef NN_HPP
#define NN_HPP

#include <cmath>
#include <fstream>
#include <iostream>
#include <Capa.hpp>


class NN
{
    public:

        NN(int input, int len, int* topo);

        NN(int len);

        int* topologia;

        int len_layers;

        Capa** Layers;

        float* Forward(float* input);

        void Backward(float* output, float* target);

        void Update( float lr);

        float CrossEntropy(float* pn, float* yn);

        float** Z;

};

#endif // NN_HPP