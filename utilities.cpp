//opencv
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "utilities.h" 
#include "config.h"
#include <iterator>

//**************************************************
// Convert matrix vector<float> to cv:Mat (ROW major) 
// Input: + input matrix 
//			 + nRows of matrix 
//			 + nCols of matrix 
// Output:
//			++ cvMatrix (opencv) 
//*************************************************
cv::Mat vector2mat(std::vector<float>  inputVector, int nRows, int nCols){

   cv::Mat result(nRows,nCols,CV_32FC1);

   for(int i = 0 ; i < nRows ; i++){
      for(int j = 0 ; j < nCols ; j++){
         result.at<float>(i,j) = inputVector[i*nCols + j] ;
      }

   }

   return result ;
}

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
//**************************************************
// Convert a vector (type cv:Mat to std::vector)  
// Input: + input matrix  (row major)  
// Output:
//       ++ cvMatrix (opencv) 
//*************************************************
std::vector<float> mat2vector(const cv::Mat inputMat){

	std::vector<float> array ; 	
   if (inputMat.isContinuous()) {
      array.assign((float*)inputMat.datastart, (float*)inputMat.dataend);
   } else {
      for (int i = 0; i < inputMat.rows; ++i) {
         array.insert(array.end(), (float*)inputMat.ptr<float>(i), (float*)inputMat.ptr<float>(i)+inputMat.cols);
      }
   }
	return array ; 
}

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



// **********************************************
// Normalize matrix to norm L2
// Input : + input matrix (vector<float>) collum-major 
//			  + nRows of matrix 
//			  + nCols of matrix 
//
// **********************************************
std::vector<float> normalize_matrix_norml2(std::vector<float> inputMat, int nRows, int nCols){

	if(inputMat.empty()){
		std::cout << BOLDRED << " Invalid input matrix : " << RESET << std::endl;
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

// **********************************************
// Normalize each col of matrix to range [a,b] 
// Input: + matrix vector<float> 
//			 + nRows of matrix 
//			 + nCols of matrix 
//			 + lower bound value a 
//			 + upper bound value b 		
// **********************************************
std::vector<float> normalize_matrix(std::vector<float> inputMat, int nRows, int nCols,  float a, float b){

	if(a >=b ) {
		std::cout << BOLDRED << "Invalid range : " << RESET << std::endl ;
		exit(1);
	}
	if(inputMat.empty() || inputMat.size() == 1) {
		std::cout << BOLDRED << " Invalid input vector : " << RESET << std::endl;
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



//**********************************************************
// compute variance of a vector 
//*********************************************************
float compute_variance(const std::vector<float> inVector){

  //compute mean of this vector 
  float mean = 0 ; 
  float sum = 0 ; 	

  for(int i = 0 ; i < inVector.size() ; i++){ 
		sum = sum + inVector[i] ; 
  }  
  mean  = sum/ inVector.size(); 

  float temp = 0 ; 
  for(int i = 0; i < inVector.size(); i++){
	temp = temp + pow(inVector[i] - mean,2) ; 
  }
  return temp/(inVector.size()) ; 
} 

//**************************************************************
// Compute l2-norm of vector 
//***********************************************************
float l2_norm(std::vector<float> const& u) {
    float accum = 0;
    for (int i = 0; i < u.size(); ++i) {
        accum += u[i] * u[i];
    }
    return sqrt(accum);
}

//********************************************************
// Compute inner_product of 2 vector 
//********************************************************
float inner_product(std::vector<float> u, std::vector<float> v){

	if(u.size() != v.size()){
		std::cout << "Two vector have different size " << std::endl ; 
		exit(1); 	
	}

	float acc = 0.0 ; 
	for(int i = 0 ; i < u.size() ;i++){ 
		acc = acc + u[i]*v[i]; 
	}

	return acc ; 
}

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
