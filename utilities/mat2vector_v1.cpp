#include "utilities.h" 

//**************************************************
// Collumn major
// Convert a vector (type cv:Mat to std::vector)  
// Input: + input matrix   
// Output:
//       ++ cvMatrix (opencv) 
//*************************************************
std::vector<float> mat2vector_v1(const cv::Mat inputMat){

   cv::Mat transposeOfinputMat ;
   cv::transpose(inputMat,transposeOfinputMat);
   std::vector<float> array ;
   if (transposeOfinputMat.isContinuous()) {
      array.assign((float*)transposeOfinputMat.datastart, (float*)transposeOfinputMat.dataend);
   } else {
      for (int i = 0; i < transposeOfinputMat.rows; ++i) {
         array.insert(array.end(), (float*)transposeOfinputMat.ptr<float>(i), (float*)transposeOfinputMat.ptr<float>(i)+ transposeOfinputMat.cols);
      }
   }
   return array ;
}



