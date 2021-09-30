//==========================================================================================================================================================
//
// Matriz class
//
//==========================================================================================================================================================

#include <iomanip>
#include <Complex.hpp>
#include <Matriz.hpp>


/* PUBLIC METHODS */

Matriz::Matriz(int m, int n, bool complex):dim_matriz_m(m), dim_matriz_n(n), complx(complex)
{
    // Inicializamos la matriz compleja con coeficientes = 0.
    if (complex == true)
    {
        MC = new Complex*[dim_matriz_m];
        for(int i = 0; i < dim_matriz_m; i++)
        {
            MC[i] = new Complex[dim_matriz_n];
        }
              
                  
    }

    // Inicializamos la matriz con coeficientes = 0.
    else
    {
        M = new float*[dim_matriz_m];
        for(int i = 0; i < dim_matriz_m; i++)
        {
            M[i] = new float[dim_matriz_n];
            for (int j = 0; j < dim_matriz_n; j++){
                M[i][j] = 0;
            } 
        }
                      
    }   
}

Matriz::Matriz(int dim):N_vectores(dim)
{
    vector = new Complex[dim];
}

Matriz::Matriz(int matrices, int m, int n, bool complex):N_matrices(matrices), dim_matriz_m(m), dim_matriz_n(n), complx(complex)
{

    // Inicializamos la matriz compleja con coeficientes = 0.
    if (complex == true)
    {
        Matrices = new Matriz*[N_matrices];
        for(int i = 0; i < N_matrices; i++)
        {
            Matrices[i] = new Matriz(m, n, complx);
        }
              
                  
    }

    // Inicializamos la matriz con coeficientes = 0.
    else
    {
        Matrices = new Matriz*[N_matrices];
        for(int i = 0; i < N_matrices; i++)
        {
            Matrices[i] = new Matriz(m, n, complx);
        }
                      
    }   
}

Matriz::Matriz(int matrices, int chanels, int m, int n, bool complex):N_matrices(matrices * chanels), N_chanels(chanels), dim_matriz_m(m), dim_matriz_n(n), complx(complex)
{
    // Inicializamos la matriz compleja con coeficientes = 0.
    if (complex == true)
    {
        Matrices = new Matriz*[N_matrices * chanels];
        for(int i = 0; i < N_matrices * chanels; i++)
        {
            Matrices[i] = new Matriz(m, n, complx);
        }
              
                  
    }

    // Inicializamos la matriz con coeficientes = 0.
    else
    {
        Matrices = new Matriz*[(N_matrices * chanels)];
        for(int i = 0; i < (N_matrices * chanels); i++)
        {
            Matrices[i] = new Matriz(m, n, false);
        }
                      
    }   
}

void Matriz::random()
{
    Matriz* m;

    for (int index = 0; index < this->N_matrices; index++)
    {
        
        if (this->N_matrices > 1)
        {
            m = this->Matrices[index];
        }
        else
        {
            m = this; 
        }

        if (m->complx == true)
        {
            for (int i = 0; i < dim_matriz_m; i++)
            {
                for (int j = 0; j < dim_matriz_n; j++)
                {
                    
                    m->MC[i][j].Re = ((float) rand() / (RAND_MAX)) * 2 - 1;
                    m->MC[i][j].Im = ((float) rand() / (RAND_MAX)) * 2 - 1;
                    
                    
                }
            }
        }
        else
        {
            for (int i = 0; i < dim_matriz_m; i++)
            {
                for (int j = 0; j < dim_matriz_n; j++)
                {
                    //m->M[i][j] = rand() % 10;
                    m->M[i][j] = ((float) rand() / (RAND_MAX)) * 2 - 1;
                }
            }
        }

    }

        

     
    // convolucion test
    /* MC[5][5].Re = 255;
    MC[5][6].Re = 255;
    MC[6][5].Re = 255;
    MC[6][6].Re = 255; */
    
    
  
    
   
   
}

void Matriz::addComplex()
{
    this->MC = new Complex*[this->dim_matriz_m];
    for(int i = 0; i < this->dim_matriz_m; i++)
    {
        MC[i] = new Complex[this->dim_matriz_n];
    }
}

void Matriz::show(int x)
{
    if (this->complx == true)
    {
        if (x == 1)
        {
            for (int i = 0; i < dim_matriz_m; i++)
            {
                std::cout << "|";
                for (int j = 0; j < dim_matriz_n; j++)
                {
                    std::cout << " " <<  this->MC[i][j].Im<< " " ;
                }
                std::cout << "|" << std::endl;
            }
            std::cout << std::endl;
        }

        else
        {
            for (int i = 0; i < dim_matriz_m; i++)
            {
                std::cout << "|";
                for (int j = 0; j < dim_matriz_n; j++)
                {
                    std::cout << " "<< this->MC[i][j].Re << " " ;
                }
                std::cout << "|" << std::endl;
            }
            std::cout << std::endl;
        }
        
    }
    else
    {
        for (int i = 0; i < dim_matriz_m; i++)
        {
            std::cout << "|";
            for (int j = 0; j < dim_matriz_n; j++)
            {
                std::cout << " " << this->M[i][j] << " " ;
            }
            std::cout << "|" << std::endl;
        }
        std::cout << std::endl;
    }
    
}

Matriz Matriz::pading(int p, int x = 0)
{
    int New_M = this->dim_matriz_m + 2 * p;
    int New_N = this->dim_matriz_n + 2 * p;

    int M = this->dim_matriz_m;
    int N = this->dim_matriz_n;

    Matriz* matriz_re;

    if (this->N_matrices > 1){

        matriz_re = new Matriz(this->N_matrices, New_M, New_N, false);
    }
    else{
        matriz_re = new Matriz(New_M, New_N, false);
    }

    // Inicializamos la matriz compleja con coeficientes = 0.
    for (int matriz_idx = 0; matriz_idx < this->N_matrices; matriz_idx++){

        float** _M;
        float** _Mre;

        if (this->N_matrices > 1){

            _M = this->Matrices[matriz_idx]->M;
            _Mre = matriz_re->Matrices[matriz_idx]->M;

        }
        else{
            _M = this->M;
            _Mre = matriz_re->M;
        }
        
        
            

        for(int i = p; i < M + p; i++){
            for(int j = p; j < N + p; j++){

                _Mre[i][j] = _M[i - p][j - p];
            }               
        }
    }
    return *matriz_re;
}   

void Matriz::normalize(float n)
{
    if (this->complx == true)
    {
        for (int i = 0; i < this->dim_matriz_m; i++)
        {
            for (int j = 0; j < this->dim_matriz_n; j++)
            {
                this->MC[i][j].Re /= n; 
                this->MC[i][j].Im /= n;
            }
        }
    }
    else
    {
        for (int i = 0; i < this->dim_matriz_m; i++)
        {
            for (int j = 0; j < this->dim_matriz_n; j++)
            {
                this->M[i][j] /= n;
            }
        }
    }
    
}

Matriz Matriz::copy()
{
    Matriz *m_copy = 0;
    if (this->complx == true)
    {
        m_copy = new Matriz(this->dim_matriz_m, this->dim_matriz_n, true);
        for (int i = 0; i < this->dim_matriz_m; i++)
        {
            for (int j = 0; j < this->dim_matriz_n; j++)
            {
                m_copy->MC[i][j].Re = this->MC[i][j].Re; 
                m_copy->MC[i][j].Im = this->MC[i][j].Im;
            }
        }
    }
    else
    {
        m_copy = new Matriz(this->dim_matriz_m, this->dim_matriz_n, true);
        for (int i = 0; i < this->dim_matriz_m; i++)
        {
            for (int j = 0; j < this->dim_matriz_n; j++)
            {
                m_copy->M[i][j] = this->M[i][j]; 
            }
        }
    }
    return *m_copy;
}

Matriz Matriz::dot(Matriz matriz)
{
    int m = this->dim_matriz_m;
    int n = this->dim_matriz_n;


    Matriz* matriz_re = new Matriz(m, n, true);
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            float real = this->MC[i][j].Re * matriz.MC[i][j].Re - this->MC[i][j].Im * matriz.MC[i][j].Im;
            float imag = this->MC[i][j].Im * matriz.MC[i][j].Re + this->MC[i][j].Re * matriz.MC[i][j].Im;

            matriz_re->MC[i][j].Re = real;
            matriz_re->MC[i][j].Im = imag;
        }
    }

    return *matriz_re;
}

Matriz Matriz::rot180(){


    Matriz* matriz_re;

    if ( this->N_matrices > 1){
        matriz_re = new Matriz(this->N_matrices, this->dim_matriz_m, this->dim_matriz_n, false);
    }
    else{
        matriz_re = new Matriz(this->dim_matriz_m, this->dim_matriz_n, false);
    }

    float** _matriz;
    float** _matrizrot;

    for (int matriz_idx = 0; matriz_idx < this->N_matrices; matriz_idx++){



        if (this->N_matrices > 1){

            _matriz = Matrices[matriz_idx]->M;
            _matrizrot = matriz_re->Matrices[matriz_idx]->M;
            
        }
        else{

            _matriz = this->M;
            _matrizrot = matriz_re->M;
        }
        
        
        for (int i = 0; i < this->dim_matriz_m; i++){
            for (int j = 0; j < this->dim_matriz_n; j++){

                int m = this->dim_matriz_m - i - 1;
                int n = this->dim_matriz_n - j - 1;

                _matrizrot[m][n] = _matriz[i][j];

            }
        }


    }
    matriz_re->N_chanels = this->N_chanels;
    return *matriz_re;

}

void Matriz::add(Matriz* matriz, float lr){

    float** _M;
    float** _matriz;

    for (int matriz_idx = 0; matriz_idx < this->N_matrices; matriz_idx++){

        if (this->N_matrices > 1){
            _M = this->Matrices[matriz_idx]->M;
            _matriz = matriz->Matrices[matriz_idx]->M;
        }
        else{
            _M = this->M;
            _matriz = matriz->M;
        }

        for (int i = 0; i < this->dim_matriz_m; i++){
            for (int j = 0; j < this->dim_matriz_n; j++){

                _M[i][j] += lr *_matriz[i][j];
            }
        }
    }       
}