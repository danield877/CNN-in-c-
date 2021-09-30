#ifndef CAPA_HPP
#define CAPA_HPP


#include <cmath>
#include <fstream>
#include <iostream>
#include <Capa.hpp>
#include <Matriz.hpp>


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



Capa::Capa(int output_shape, int input_shape, float* (*func)(float*, int)):output_shape(output_shape), input_shape(input_shape)
{
    Wns = new Matriz(output_shape, input_shape + 1, false);
    Wns->random();
    Wns->dim_matriz_n = input_shape;
    function = func;
}

float* Capa::Cal_Z(float* vec_input)
{
    this->input = vec_input;

    int N = this->output_shape;
    float* vec_re = new float[N];

    float** _Wns = this->Wns->M;


    for (int neurona = 0; neurona < this->output_shape; neurona++)
    {
        
        float suma = 0;

        for (int peso = 0; peso < this->input_shape ; peso++)
        {
            
            suma += vec_input[peso] * _Wns[neurona][peso];

        }

        vec_re[neurona] = suma + _Wns[neurona][this->input_shape];
    }
    return vec_re;
}

float* Capa::apl_ReLU(float* vec)
{
    float* vec_re = new float[this->output_shape];

    
    for (int i = 0; i < this->output_shape; i++)
    {
        vec_re[i] = fmax(0, vec[i]);       
    }
    return vec_re;
}

float* Capa::SoftMax(float* vec)
{
    float* vec_re = new float[this->output_shape];

    float suma = 0;

    for (int i = 0; i < this->output_shape; i++)
    {
        suma += exp(vec[i]);
    }
    for (int i = 0; i < this->output_shape; i++)
    {
        vec_re[i] = exp(vec[i]) / suma;
    }

    return vec_re;
}



float* Capa::error(float* p, float* y)
{
    float* vec_re = new float[this->output_shape];

    for (int i = 0; i < this->output_shape; i++)
    {
        vec_re[i] =  p[i] - y[i];
    }
    return vec_re;

}

float* Capa::reul_d(float* vec)
{
    float* vec_re = new float[this->output_shape];

    for (int i = 0; i < this->output_shape; i++)
    {
        if (vec[i] > 0)
        {
            vec_re[i] = 1;
        }
        else
        {
            vec_re[i] = 0;
        }
        
        
    }
    return vec_re;
}

float* Capa::ActFuntion(float* input)
{
    this->output = function(input, this->output_shape);
    return this->output;
}

void Capa::Update(float lr){

    int m = this->Wns->dim_matriz_m;
    int n = this->Wns->dim_matriz_n;

    float** _Wns = this->Wns->M;
    float* _delta = this->delta;
    float* _input = this->input;


    for (int i = 0; i < m; i++){
        for (int j = 0; j < n; j++){

            _Wns[i][j] -= lr * delta[i] * input[j];
        
        
        }
        _Wns[i][n] -= lr * delta[i];
    }
}


float* Capa::WnMultDelta(float* delta){

    int M = this->Wns->dim_matriz_m;
    int N = this->Wns->dim_matriz_n;

    float* vec_re = new float[N];

    float** _Wns = this->Wns->M;


    for (int i = 0; i < N; i++){

        float sum = 0;

        for (int j = 0; j < M; j++){


            sum += _Wns[j][i] * delta[j];
            

        }

        vec_re[i] = sum;
    }
    return vec_re;

}


#endif // NN_HPP