#include "utilities.h"

// C = alpha*op(A)*op(B) + beta*C ; 
void mmul_cublas(std::vector<float> A, cublasOperation_t transa,  std::vector<float> B, cublasOperation_t transb ,std::vector<float> &C, int Arows, int Acols, int Bcols){

   int Brows = Acols ;
   int Crows = Arows;
   int Ccols = Bcols ;

   int lda = Arows ;          //leading dimension of A; 
   int ldb = Brows ;          //leading dimenstion of B; 
   int ldc = Crows ;
   const float alf = 1;
   const float bet = 0 ;
   const float *alpha = &alf ;
   const float *beta = &bet ;

   //declare gpu device matrix 
   thrust::device_vector<float> d_A(Arows* Acols), d_B(Brows*Bcols), d_C(Crows*Ccols);

   //convert vector to cublas data type 
   thrust::copy(A.begin(),A.end(),d_A.begin());
   thrust::copy(B.begin(),B.end(),d_B.begin());

   // Create a handle for CUBLAS
   cublasHandle_t handle;
   cublasCreate(&handle);

   // Do the actual multiplication
   cublasSgemm(handle,
         transa,
         transb,
         Arows,
         Bcols,
         Acols,
         alpha,
         thrust::raw_pointer_cast(&d_A[0]),
         lda,
         thrust::raw_pointer_cast(&d_B[0]),
         ldb,
         beta,
         thrust::raw_pointer_cast(&d_C[0]),
         ldc);

   //Copy data back to cpu 
   thrust::copy(d_C.begin(),d_C.end(),C.begin());


   // Destroy the handle
   cublasDestroy(handle);


}

