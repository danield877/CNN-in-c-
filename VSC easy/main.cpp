#include "Complex.hpp"
#include "Matriz.hpp"
#include <iostream>
#include <cmath>
#include <fstream>
#include "Convolucion.hpp"
#include "NN.hpp"

using namespace std;


int main(){

    int k = 0;
    Convolucion conv;

    int numero_imagenes;
    int size_imagen;

    Matriz *imagenes = conv.leer_imagenes("train-images.idx3-ubyte" , numero_imagenes, size_imagen);
    int *labels;
    int numero_labels;

    labels = conv.leer_etiquetas("train-labels.idx1-ubyte" , numero_labels);
    
    Matriz f1(4, 5, 5, false);
    f1.random();
    Matriz f2(12, 4, 5, 5, false);
    f2.random();

     

    for(int epoch = 0; epoch < 30; epoch++){

        Matriz *m = imagenes->Matrices[epoch];
        m->normalize(255);


        Matriz con1 = conv.Conv2D(m, &f1, 1);
        Matriz relu1 = conv.activation(con1);
        Matriz max1 = conv.MaxPooling(relu1, 0, 2, 2);


        Matriz con2 = conv.Conv2D(&max1, &f2, 1);
        Matriz relu2 = conv.activation(con2);
        Matriz max2 = conv.MaxPooling(relu2, 0, 2, 2);
        

        int N = 0;
        float* input = conv.flatten(max2, &N);

        NN nn(1);
        nn.Layers[0] = new Capa(10, 192, SoftMax);
        

        float* pn = nn.Forward(input);


        // -----------TEST------------

        float* yn = conv.OneHotLabel(labels[epoch]);

        if (epoch % 1 == 0){
            cout << "epoch = " << epoch << " ,  L = " << nn.CrossEntropy(pn, yn) << endl;

        }

        nn.Backward(pn, yn);

        float* lastDelta = nn.Layers[0]->delta;  

        float *errorflatt_vec = nn.Layers[0]->WnMultDelta(lastDelta);

        Matriz errorMP = conv.flatten_inv(errorflatt_vec, N, 4); //shapè = (12, 4, 4)
        
        Matriz error2 = conv.MaxPooling_inv(errorMP, &max2, 0, 2, 2 ); // shape= (12, 8, 8)



        Matriz delta2 = conv.Mult_dRelu(&error2, &con2);

        Matriz rotf2 = f2.rot180();

        Matriz delta1MP = conv.fullConv2D(&rotf2, &delta2); // filtro(12,4,5,5)  deltas(12, 8, 8)
        Matriz error1 = conv.MaxPooling_inv(delta1MP, &max1, 0, 2, 2 ); 

        Matriz delta1 = conv.Mult_dRelu(&error1, &con1);

        // ------------ UPDATE WEIGHTS--------

        float lr = 0.01;

        nn.Update(lr);

        

        Matriz deltaWn_2 = conv.Conv2Db(&max1, &delta2); //imax(4, 12, 12)  delta2{12, 8, 8}

        f2.add(&deltaWn_2, -lr);

        Matriz deltaWn_1 = conv.Conv2Db(m, &delta1); // m(1, 28, 28) , dekta1(4, 24, 24)

        f1.add(&deltaWn_1, -lr); 

    }



    NN nn(1);
    nn.Layers[0] = new Capa(10, 192, SoftMax);


    for(int epoch = 0; epoch < 12000; epoch++){

        float lr = 0.01;

        int h = epoch + 2000;

        Matriz *m = imagenes->Matrices[h];
        m->normalize(255);


        Matriz con1 = conv.Conv2D(m, &f1, 1);
        Matriz relu1 = conv.activation(con1);
        Matriz max1 = conv.MaxPooling(relu1, 0, 2, 2);


        Matriz con2 = conv.Conv2D(&max1, &f2, 1);
        Matriz relu2 = conv.activation(con2);
        Matriz max2 = conv.MaxPooling(relu2, 0, 2, 2);
        

        int N = 0;
        float* input = conv.flatten(max2, &N);

        float* pn = nn.Forward(input);


        // -----------TEST------------

        float* yn = conv.OneHotLabel(labels[h]);

        if (epoch % 1000 == 0){
            cout << "epoch = " << epoch << " ,  L = " << nn.CrossEntropy(pn, yn) << endl;

        }

        nn.Backward(pn, yn);

        float* lastDelta = nn.Layers[0]->delta;  

        float *errorflatt_vec = nn.Layers[0]->WnMultDelta(lastDelta);

        Matriz errorMP = conv.flatten_inv(errorflatt_vec, N, 4); //shapè = (12, 4, 4)
        
        Matriz error2 = conv.MaxPooling_inv(errorMP, &max2, 0, 2, 2 ); // shape= (12, 8, 8)



        Matriz delta2 = conv.Mult_dRelu(&error2, &con2);

        Matriz rotf2 = f2.rot180();

        Matriz delta1MP = conv.fullConv2D(&rotf2, &delta2); // filtro(12,4,5,5)  deltas(12, 8, 8)
        Matriz error1 = conv.MaxPooling_inv(delta1MP, &max1, 0, 2, 2 ); 

        Matriz delta1 = conv.Mult_dRelu(&error1, &con1);

        // ------------ UPDATE WEIGHTS--------

        

        nn.Update(lr);       

        Matriz deltaWn_2 = conv.Conv2Db(&max1, &delta2); //imax(4, 12, 12)  delta2{12, 8, 8}

        f2.add(&deltaWn_2, -lr);

        Matriz deltaWn_1 = conv.Conv2Db(m, &delta1); // m(1, 28, 28) , dekta1(4, 24, 24)

        f1.add(&deltaWn_1, -lr); 

    }

    cout << "  "  << endl;

    for (int i = 0; i < 10; i++){
        int j = 20000 + i;
        Matriz *test = imagenes->Matrices[j];
        test->normalize(255);

        Matriz con1 = conv.Conv2D(test, &f1, 1);
        Matriz relu1 = conv.activation(con1);
        Matriz max1 = conv.MaxPooling(relu1, 0, 2, 2);

        Matriz con2 = conv.Conv2D(&max1, &f2, 1);
        Matriz relu2 = conv.activation(con2);
        Matriz max2 = conv.MaxPooling(relu2, 0, 2, 2);

        int N = 0;
        float* input = conv.flatten(max2, &N);

        float* pn = nn.Forward(input);

        for (int i = 0; i < 10; i++){
            cout << i << " -> " << pn[i] << endl;
        }
        cout << "True label =  " << labels[j] << endl << endl;

    }        

}