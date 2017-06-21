#include "utilities.h" 


//*************************************************************
// Matrix multiply C = A*B (Collum major)  
// Input : +  A [nRows_A ,nCols_A] 
//         +  B [nCols_A, nCols_B]      
//         + nRows_A : number of rows of matrix A 
//         + nCols_A: number of cols of matrix A 
//         + nCols_B: number of cols of matrix B 
// Output  + Matrix C [nRows_A, nCols_B] 
//***********************************************************
std::vector<float> matrix_multiply(std::vector<float> A, std::vector<float> B , int nRows_A, int nCols_A, int nCols_B){

   std::vector<float> C(nRows_A*nCols_B,0);
   int nRows_B = nCols_A ;
   int nRows_C = nRows_A ;
   int nCols_C = nCols_B ;
   for(int j = 0 ; j < nCols_C; j++){
      for(int i  =0 ; i < nRows_C ;  i++){
         for(int k = 0 ; k < nCols_A ; k++){
            C[j*nRows_C + i] =   C[j*nRows_C + i] + A[i + k*nRows_A]*B[j*nRows_B + k];
         }

      }
   }

   return C ;
}

