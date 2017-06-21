#include "utilities.h" 

//**************************************************
// Convert matrix vector<float> to cv:Mat (Collum major) 
// Input: + input matrix  
//        + nRows of matrix 
//        + nCols of matrix  
// Output:
//       ++ cvMatrix (opencv) 
//*************************************************
cv::Mat vector2mat_v1(std::vector<float>  inputVector, int nRows, int nCols){

   cv::Mat result(nRows,nCols,CV_32FC1);

   for(int j = 0 ; j < nCols ; j++){
      for(int i = 0 ; i < nRows ; i++){
         result.at<float>(i,j) = inputVector[j*nRows + i] ;
      }

   }

   return result ;
}

