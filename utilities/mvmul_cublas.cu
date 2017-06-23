#include "utilities.h"

//matrix-vector multiplication 
//y = α op ( A ) x + β y
void mvmul_cublas(std::vector<float> A, cublasOperation_t transa,  std::vector<float> x, std::vector<float> &y, int Arows, int Acols ){

   int lda = Arows ;          //leading dimension of A; 
   const float alf = 1;
   const float bet = 0 ;
   const float *alpha = &alf ;
   const float *beta = &bet ;

   //declare gpu device matrix 
   thrust::device_vector<float> d_A(Arows* Acols), d_x(x.size()), d_y(Acols);

   //convert vector to cublas data type 
   thrust::copy(A.begin(),A.end(),d_A.begin());
   thrust::copy(x.begin(),x.end(),d_x.begin());

   // Create a handle for CUBLAS
   cublasHandle_t handle;
   cublasCreate(&handle);

   // Do the actual multiplication
   cublasSgemv(handle,
         transa,
         Arows,
         Acols,
         alpha,
         thrust::raw_pointer_cast(&d_A[0]),
         lda,
         thrust::raw_pointer_cast(&d_x[0]),
         1,		//incx
         beta,
         thrust::raw_pointer_cast(&d_y[0]),
         1 ); //incry

   //Copy data back to cpu 
   thrust::copy(d_y.begin(),d_y.end(),y.begin());


   // Destroy the handle
   cublasDestroy(handle);

}

