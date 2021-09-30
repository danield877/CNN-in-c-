#ifndef NN_HPP
#define NN_HPP

#include <cmath>
#include "Capa.hpp"
#include <fstream>
#include <iostream>

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

float* SoftMax(float* vec, int len)
{
    float* vec_re = new float[len];

    float suma = 0;

    for (int i = 0; i < len; i++)
    {
        suma += exp(vec[i]);
    }
    for (int i = 0; i < len; i++)
    {
        vec_re[i] = exp(vec[i]) / suma;
    }

    return vec_re;
}

float* apl_ReLU(float* vec, int len)
{
    float* vec_re = new float[len];

    
    for (int i = 0; i < len; i++)
    {
        vec_re[i] = fmax(0, vec[i]);       
    }
    
    return vec_re;
}




NN::NN(int input_len, int len, int* topo): topologia(topo), len_layers(len)
{

    Layers = new Capa*[len];
    Layers[0] = new Capa(topo[0], input_len, apl_ReLU);

    for (int i = 0; i < len - 1 ; i++)
    {
        Layers[i+1] = new Capa(topo[i+1], topo[i], apl_ReLU);
    }
    Z = new float*[len];
}

NN::NN(int len): len_layers(len)
{
    Layers = new Capa*[1];

    Z = new float*[len];
}

float* NN::Forward(float* input)
{
    Capa* Layer;

    int len_layers = this->len_layers;

    int N;

    Z[0] = this->Layers[0]->Cal_Z(input);
    float* outputs = this->Layers[0]->ActFuntion(Z[0]);


    for (int layer = 1; layer < len_layers; layer++)
    {
        Layer = this->Layers[layer];
        
        Z[layer] = Layer->Cal_Z(Layer->output);
        outputs = Layer->ActFuntion(Z[layer-1]);
    }

    return this->Layers[len_layers-1]->output;
}

void NN::Backward(float* output, float* target)
{

    int len_layers = this->len_layers;

    this->Layers[len_layers - 1]->delta = this->Layers[len_layers - 1]->error(output, target);

    if (len_layers > 1){
        for (int layer = len_layers - 2; layer >= 0; layer--)
        {
            float* LastDelta = this->Layers[layer + 1]->output;
            float* Delta = this->Layers[layer]->output;
            float** _Wns = this->Layers[layer + 1]->Wns->M;
            
            for (int i = 0; i < this->Layers[layer + 1]->input_shape; i++)
            {
                float suma = 0;
                for (int j = 0; j < this->Layers[layer + 1]->output_shape; j++)
                {
                    suma += _Wns[j][i] * LastDelta[j];
                }

                // Suma = Inlayer->ActFunc_d(Z[layer][i]) 

                Delta[i] = suma;
            }
        }
    }
    
}

void NN::Update(float lr){

    for (int layer = 0; layer < this->len_layers; layer++){

        this->Layers[layer]->Update(lr);
    }
}

float NN::CrossEntropy(float* pn, float* yn)
{
    float suma = 0;
    for (int i = 0; i < 10; i++)
    {
        suma -= yn[i] * log(pn[i]);
    }
    return suma;
}

#endif // NN_HPP