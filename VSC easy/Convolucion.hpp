#ifndef CONCOLUCION_HPP
#define CONVOLUCION_HPP

#include <cmath>
#include "Matriz.hpp"
#include <fstream>
#include <iostream>


class Convolucion{
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


Matriz* Convolucion::leer_imagenes(std::string full_path, int& number_of_images, int& image_size) {

    auto reverseInt = [](int i) {

        unsigned char c1, c2, c3, c4;

        c1 = i & 255;
        c2 = (i >> 8) & 255;
        c3 = (i >> 16) & 255;
        c4 = (i >> 24) & 255;

        return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
    };


    std::ifstream file;
    file.open(full_path, std::ios::binary);
    if(file.is_open()) {

        int magic_number = 0;
        int n_rows = 0;
        int n_cols = 0;
        

        file.read((char *)&magic_number, sizeof(magic_number));

        magic_number = reverseInt(magic_number);

        if(magic_number != 2051) throw std::runtime_error("Invalid MNIST image file!");

        file.read((char *)&number_of_images, sizeof(number_of_images));
        number_of_images = reverseInt(number_of_images);

        file.read((char *)&n_rows, sizeof(n_rows));
        n_rows = reverseInt(n_rows);

        file.read((char *)&n_cols, sizeof(n_cols));
        n_cols = reverseInt(n_cols);

        image_size = n_rows * n_cols;
        
        Matriz *imagenes = new Matriz(number_of_images, 28, 28, false);
        
        for(int i = 0; i < number_of_images; i++){
            
            unsigned char p[image_size];
            file.read((char *)p, sizeof(p));

            int aux = 0;
            float temp = 0;
            for (int k = 0; k < 28 ; k++){
                for (int j = 0; j < 28 ; j++){
                    
                    temp = (float) p[aux];

                    imagenes->Matrices[i]->M[k][j] = temp;
                              
                    aux++;          
                }
            }
        }
        return imagenes;
    } 
    else{
        throw std::runtime_error("Cannot open file `" + full_path + "`!");
    }
}

int* Convolucion::leer_etiquetas(std::string full_path, int& number_of_labels){
    auto reverseInt = [](int i){
        unsigned char c1, c2, c3, c4;

        c1 = i & 255;
        c2 = (i >> 8) & 255;
        c3 = (i >> 16) & 255;
        c4 = (i >> 24) & 255;

        return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
    };

    std::ifstream file(full_path, std::ios::binary);

    if(file.is_open()){

        int magic_number = 0;

        file.read((char *)&magic_number, sizeof(magic_number));

        magic_number = reverseInt(magic_number);

        if(magic_number != 2049) throw std::runtime_error("Invalid MNIST label file!");

        file.read((char *)&number_of_labels, sizeof(number_of_labels)), number_of_labels = reverseInt(number_of_labels);

        int* _dataset = new int[number_of_labels];

        
        char* p = new char[number_of_labels];
        file.read((char*)p, number_of_labels);
        for(int i = 0; i < number_of_labels; i++) {
            
            _dataset[i] = (int) p[i];
        }
        return _dataset;
    } 
    else {
        throw std::runtime_error("Unable to open file `" + full_path + "`!");
    }
}

float* Convolucion::OneHotLabel(int x){

    float* vec_re = new float[10];

    for (int i = 0; i < 10; i++){

        vec_re[i] = 0;
    }
    vec_re[x] = 1;
    return vec_re;
}

Matriz Convolucion::Conv2D(Matriz* matrices, Matriz* filtros, int s){

    int Maps_dim_m = (matrices->dim_matriz_n - filtros->dim_matriz_m) / s + 1;
    int Maps_dim_n = (matrices->dim_matriz_m - filtros->dim_matriz_n) / s + 1;

    int maps_chanels = matrices->N_matrices;
    int N_filtros = filtros->N_matrices;

    int N_chanels = N_filtros / maps_chanels;
    int aux;
    

    Matriz *Maps_re = new Matriz(N_chanels, Maps_dim_m, Maps_dim_n, false);

    for (int maps_index = 0; maps_index < N_chanels; maps_index++){
        aux = maps_index * maps_chanels;

        // Recorremos los elementos de la matriz_re.
        for (int conv_v = 0; conv_v < Maps_dim_m; conv_v++){
            for (int conv_h = 0; conv_h < Maps_dim_n; conv_h++){
                float conv_suma = 0;

                for (int index = 0; index < maps_chanels; index++){
                    float** _matriz;
                    float** _filtro;

                   

                    // Declaramos la variable que almacena la suma de cada elemento.
                    if (filtros->N_matrices > 1){
                        _filtro = filtros->Matrices[index+ aux]->M;
                    }
                    else{
                        _filtro = filtros->M;
                    }

                    if (matrices->N_matrices > 1){
                        _matriz = matrices->Matrices[index]->M;
                    }
                    else{
                        _matriz = matrices->M;
                    }
                
                    // Recorremos el delta
                    for (int filtros_m = 0; filtros_m < filtros->dim_matriz_m; filtros_m++){
                        for (int filtros_n = 0; filtros_n < filtros->dim_matriz_n; filtros_n++){
                            conv_suma += _matriz[conv_v * s + filtros_m][conv_h * s + filtros_n] * _filtro[filtros_m][filtros_n];                            
                        }                       
                    }                   
                }
                // Almacenamos la suma.
                Maps_re->Matrices[maps_index]->M[conv_v][conv_h] = conv_suma; 
            }
        }
    }
    return *Maps_re;

}

Matriz Convolucion::Conv2Db(Matriz* input, Matriz* deltas){

    int Maps_dim_m = input->dim_matriz_m - deltas->dim_matriz_m + 1;
    int Maps_dim_n = input->dim_matriz_n - deltas->dim_matriz_n + 1;

    // en teoria matrices->N_matrices = deltas->N_matrice

    int N_matrices = input->N_matrices * deltas->N_matrices;


    Matriz *Maps = new Matriz(N_matrices, Maps_dim_m, Maps_dim_n, false);

    // Declaramos la matriz resultante de dimension (m,n).
    int Maps_idx = 0;
    
    for (int delta_idx = 0; delta_idx < deltas->N_matrices; delta_idx++){

        float **_delta;
        _delta = deltas->Matrices[delta_idx]->M;
       
        for (int matriz_idx = 0; matriz_idx < input->N_matrices; matriz_idx++){

            float** _matriz;

            if (input->N_matrices > 1){
                _matriz = input->Matrices[matriz_idx]->M;
            }
            else{
                _matriz = input->M;
            }           
                                 
            // Recorremos los elementos de la matriz_re.
            for (int conv_v = 0; conv_v < Maps_dim_m; conv_v++){
                for (int conv_h = 0; conv_h < Maps_dim_n; conv_h++){

                    // Declaramos la variable que almacena la suma de cada elemento.
                    float conv_suma = 0;

                    // Recorremos el delta
                    for (int delta_m = 0; delta_m < deltas->dim_matriz_m; delta_m++){
                        for (int delta_n = 0; delta_n < deltas->dim_matriz_n; delta_n++){
                            // Sumamos todos los productos.
                            
                            conv_suma += _matriz[conv_v + delta_m][conv_h + delta_n] * _delta[delta_m][delta_n];
                        }
                    }
                    
                    // Almacenamos la suma.
                    
                    Maps->Matrices[Maps_idx]->M[conv_v][conv_h] = conv_suma;
                }
            }
            
            Maps_idx++;
        }       
    }
    return *Maps;
}

Matriz Convolucion::fullConv2D(Matriz* filtros, Matriz* deltas){

    int Maps_dim_m = filtros->dim_matriz_m + deltas->dim_matriz_m - 1;
    int Maps_dim_n = filtros->dim_matriz_n + deltas->dim_matriz_n - 1;

    // en teoria matrices->N_matrices = deltas->N_matrice
    Matriz *Maps;
    int N_matrices = filtros->N_chanels;

    int N_deltas = deltas->N_matrices;

    Maps = new Matriz(N_matrices, Maps_dim_m, Maps_dim_n, false);

    Matriz deltas_p = deltas->pading(filtros->dim_matriz_m - 1, 0);    

    float** _Maps;
    float** _delta;
    float** _filter;
 
    // Declaramos la matriz resultante de dimension (m,n).
    
    for (int matriz_idx = 0; matriz_idx < N_matrices; matriz_idx++){

        _Maps = Maps->Matrices[matriz_idx]->M;

        for (int conv_v = 0; conv_v < Maps_dim_m; conv_v++){
            for (int conv_h = 0; conv_h < Maps_dim_n; conv_h++){
                float conv_suma = 0;

                for (int delta_idx = 0; delta_idx < N_deltas; delta_idx++){

                    _delta = deltas_p.Matrices[delta_idx]->M;

                    int filter_idx = delta_idx * N_matrices + matriz_idx;

                    _filter = filtros->Matrices[filter_idx]->M;

                    

                    //deltas->Matrices[delta_idx]->show();

                    //filtros->Matrices[delta_idx]->show(); 

                    // Recorremos el filtro
                    for (int filtro_m = 0; filtro_m < filtros->dim_matriz_m; filtro_m++){                 
                        for (int filtro_n = 0; filtro_n < filtros->dim_matriz_n; filtro_n++){  
                                            
                            conv_suma += _delta[conv_v + filtro_m][conv_h + filtro_n] * _filter[filtro_m][filtro_n];
                        }                       
                    }                   
                }
                // Almacenamos la suma.
                Maps->Matrices[matriz_idx]->M[conv_v][conv_h] = conv_suma; 
            }
        }       
    }

    return *Maps;
} 
 
Matriz Convolucion::pooling(Matriz matriz, int f, int s){

    // Hallamos las dimensiones de la matriz resultante.+
    int n = (matriz.dim_matriz_n - f) / s + 1;
    int m = (matriz.dim_matriz_m - f) / s + 1;

    Matriz *matriz_re = new Matriz(n, m, false);

    int aux_n = 0;
    int aux_m = 0;

    for (int i = 0; i < matriz.dim_matriz_n - 1; i = i + s){
        
        aux_m = 0;
        
        for (int k = 0; k < matriz.dim_matriz_m - 1; k = k + s){
            
            float suma = 0;
            for (int ii = 0; ii < f; ii++){
                for (int kk = 0; kk < f; kk++){
                    suma += matriz.M[i + ii][k + kk];
                    
                }
            }
            
            matriz_re->M[aux_n][aux_m] = suma / ( f * f);
            
            aux_m++;
        }
        aux_n++;        
    }
    return *matriz_re;   
}

Matriz Convolucion::MaxPooling(Matriz matrices, int p, int f, int s){

    int matriz_m = matrices.dim_matriz_m;
    int matriz_n = matrices.dim_matriz_n;

    int n = (matrices.dim_matriz_n - f) / s + 1;
    int m = (matrices.dim_matriz_m - f) / s + 1;
    int N_matrices = matrices.N_matrices;

    Matriz* matriz_re = new Matriz(N_matrices, m, n, false );
    Matriz* matriz;


    for (int index = 0; index < N_matrices; index++){
        
        if (N_matrices > 1){
            matriz = matrices.Matrices[index];
        }
        else{
            matriz = &matrices; 
        }
    

        if (p >= 1){
            matriz->pading(p, 0);
        }

        matriz_re->Matrices[index]->addComplex();

        int matriz_m_idx = 0;
        
        
        for (int i = 0; i < matriz_m - 1; i = i + s){
            
            int matriz_n_idx = 0;
            
            for (int k = 0; k < matriz_n - 1; k = k + s){
                int save_max_index_m = 0;
                int save_max_index_n = 0;
                
                float max = 0;
                for (int ii = 0; ii < f; ii++){
                    for (int kk = 0; kk < f; kk++){
                        
                        float value = matriz->M[i + ii][k + kk]; 
                        if (max < value)  {
                            max = value;
                            save_max_index_m = i + ii;
                            save_max_index_n = k + kk;
                        }                
                    }
                }
                
                matriz_re->Matrices[index]->M[matriz_m_idx][matriz_n_idx] = max;
                
                matriz_re->Matrices[index]->MC[matriz_m_idx][matriz_n_idx].Re = save_max_index_m;
                
                matriz_re->Matrices[index]->MC[matriz_m_idx][matriz_n_idx].Im = save_max_index_n;
                
                matriz_n_idx++;
            }           
            matriz_m_idx++;
        }      
    }   
    return *matriz_re;
}

Matriz Convolucion::MaxPooling_inv(Matriz matriz_in, Matriz* max_index_matriz , int p, int f, int s){

    int m = (matriz_in.dim_matriz_m - 1) * s + f;
    int n = (matriz_in.dim_matriz_n - 1) * s + f;
    int N_matrices = matriz_in.N_matrices;

    Matriz* matriz_re = new Matriz(N_matrices, m, n, false);

    float** m_r;
    float** matriz;
    Complex** max_index;

    

    for (int matriz_idx = 0; matriz_idx < N_matrices; matriz_idx++){

        m_r = matriz_re->Matrices[matriz_idx]->M;
        matriz = matriz_in.Matrices[matriz_idx]->M;
        max_index = max_index_matriz->Matrices[matriz_idx]->MC;
        for (int m_idx = 0; m_idx < matriz_in.dim_matriz_m; m_idx++ ){

            for (int n_idx = 0; n_idx < matriz_in.dim_matriz_m; n_idx++ ){

                int max_idx_m = (int)max_index[m_idx][n_idx].Re;
                int max_idx_n = (int)max_index[m_idx][n_idx].Im;

                m_r[max_idx_m][max_idx_n] += matriz[m_idx][n_idx];

            }
        }       
    }   
    return *matriz_re;
}

float Convolucion::Relu(float x){
    if (x > 0){
        return x;
    }
    else{
        return 0;
    }   
}

Matriz Convolucion::activation(Matriz matrices){

    Matriz* matriz_re = new Matriz(matrices.N_matrices, matrices.dim_matriz_m, matrices.dim_matriz_n, false);
    Matriz* matriz;

    for (int index = 0; index < matrices.N_matrices; index++){
        
        if (matrices.N_matrices > 1){
            matriz = matrices.Matrices[index];
        }
        else{
            matriz = &matrices; 
        }
        for (int i = 0; i < matriz->dim_matriz_m; i++){
            for (int j = 0; j < matriz->dim_matriz_n; j++){            
                matriz_re->Matrices[index]->M[i][j] = Relu(matriz->M[i][j]);      
                   
            }
        }
    }
    return *matriz_re;   
}

float* Convolucion::flatten(Matriz matrices, int* N){
    int dim_m = matrices.dim_matriz_m;
    int dim_n = matrices.dim_matriz_n;

    *N = matrices.N_matrices * dim_m * dim_n;

    float* r_vector = new float[*N];

    int vector_index = 0;

    for (int index = 0; index < matrices.N_matrices; index++){
        for (int m = 0; m < dim_m; m++){
            for (int n = 0; n < dim_n; n++){
                r_vector[vector_index] = matrices.Matrices[index]->M[m][n];
                vector_index++;
            }
        }
    }
    return r_vector;
}

Matriz Convolucion::flatten_inv(float* vec, int len_vec, int f){

    int dim_m = f;
    int dim_n = f;
    int N_matrices = len_vec / (f * f);

    Matriz *Maps_re = new Matriz(N_matrices, dim_m, dim_n, false);

    int vec_idx = 0;
    
    for (int matriz_idx = 0; matriz_idx < N_matrices; matriz_idx++){
        for (int m = 0; m < dim_m; m++){
            for (int n = 0; n < dim_n; n++){
                Maps_re->Matrices[matriz_idx]->M[m][n] = vec[vec_idx];
                vec_idx++;
            }
        }
    }
    return *Maps_re;
}

Matriz Convolucion::Mult_dRelu(Matriz* M, Matriz* z){

    Matriz* matriz_re = new Matriz(M->N_matrices, M->dim_matriz_m, M->dim_matriz_n, false);

    for (int matriz_idx = 0; matriz_idx < M->N_matrices; matriz_idx++){

        float** _Mre = matriz_re->Matrices[matriz_idx]->M;
        float** _M = M->Matrices[matriz_idx]->M;
        float** _z = z->Matrices[matriz_idx]->M;

        for (int i = 0; i < M->dim_matriz_m; i++){
            for (int j = 0; j < M->dim_matriz_n; j++){

                if ( _z[i][j] >= 0 ){

                    _Mre[i][j] = _M[i][j];
                }
            }
        }
    }
    return *matriz_re;        
}

#endif // CONVOLUCION_HPP