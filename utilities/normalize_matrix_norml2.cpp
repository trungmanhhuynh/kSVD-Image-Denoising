
#include "utilities.h" 

// **********************************************
// Normalize matrix to norm L2
// Input : + input matrix (vector<float>) collum-major 
//         + nRows of matrix 
//         + nCols of matrix 
//
// **********************************************
std::vector<float> normalize_matrix_norml2(std::vector<float> inputMat, int nRows, int nCols){

   if(inputMat.empty()){
      std::cout <<  " Invalid input matrix : " << std::endl;
      exit(1) ;
   }

   std::vector<float> outMatrix ;
   //normalize each collumn of matrix 
   for(int col = 0 ; col < nCols ; col++){
      std::vector<float> oneCol(inputMat.begin() + col*nRows, inputMat.begin() + (col+1)*nRows);
      float acc = 0 ;
      //get l2norm of input vector 
      for(int i = 0 ; i < oneCol.size() ; i++){
         acc += oneCol[i]*oneCol[i] ;
      }
      float l2norm = sqrt(acc);

      //calculate new value in range [a,b] for each element
      for(int i =0 ; i < oneCol.size(); i++){
         oneCol[i] = oneCol[i]/l2norm ;
      }
      outMatrix.insert(outMatrix.end(),oneCol.begin(), oneCol.end());
   }
   return outMatrix;
}

