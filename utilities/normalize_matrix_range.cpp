#include "utilities.h" 

// **********************************************
// Normalize each col of matrix to range [a,b] 
// Input: + matrix vector<float> 
//        + nRows of matrix 
//        + nCols of matrix 
//        + lower bound value a 
//        + upper bound value b     
// **********************************************
std::vector<float> normalize_matrix_range(std::vector<float> inputMat, int nRows, int nCols,  float a, float b){

   if(a >=b ) {
      std::cout <<  "Invalid range : " <<  std::endl ;
      exit(1);
   }
   if(inputMat.empty() || inputMat.size() == 1) {
      std::cout  << " Invalid input vector : " << std::endl;
      exit(1) ;
   }

   std::vector<float> outMatrix ;
   //normalize each collumn of matrix 
   for(int col = 0 ; col < nCols ; col++){

      std::vector<float> oneCol(inputMat.begin() + col*nRows, inputMat.begin() + (col+1)*nRows);
      std::vector<float> result(oneCol.size());

      //find min and max value   
      float min = 9999 ;
      float max = -9999 ;

      for(int i = 0 ; i < oneCol.size() ; i++){
         if(oneCol[i] > max){
            max = oneCol[i];
         }
         if(oneCol[i] < min){
            min = oneCol[i] ;
         }
      }

      //calculate new value in range [a,b] for each element
      for(int i =0 ; i < oneCol.size(); i++){
         result[i] = (oneCol[i] - min)*(b-a )/(max - min) + a ;
      }

      outMatrix.insert(outMatrix.end(),result.begin(),result.end());

   }
   return outMatrix;
}



