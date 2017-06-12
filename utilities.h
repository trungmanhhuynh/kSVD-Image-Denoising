//opencv
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

//c++
#include <iostream>
#include <iomanip>            //std::setprecision

#ifndef UTILITIES_H 
#define UTILITIES_H 

//**************************************************
// Convert matrix vector<float> to cv:Mat  
// Input: + input matrix 
//        + nRows of matrix 
//        + nCols of matrix 
// Output:
//       ++ cvMatrix (opencv) 
//*************************************************
cv::Mat vector2mat(std::vector<float>  inputVector, int nRows, int nCols);  //row-major

cv::Mat vector2mat_v1(std::vector<float>  inputVector, int nRows, int nCols) ; //collumn-major

//**************************************************
// Convert a vector (type cv:Mat to std::vector)  
// Input: + input matrix 
// Output:
//       ++ cvMatrix (opencv) 
//*************************************************
std::vector<float> mat2vector(const cv::Mat inputMat) ;

//**************************************************
// Collumn major
// Convert a vector (type cv:Mat to std::vector)  
// Input: + input matrix   
// Output:
//       ++ cvMatrix (opencv) 
//*************************************************
std::vector<float> mat2vector_v1(const cv::Mat inputMat) ; 


// **********************************************
// Normalize matrix to norm L2
// Input : + input matrix (vector<float>) collum-major 
//         + nRows of matrix 
//         + nCols of matrix 
//
// **********************************************
std::vector<float> normalize_matrix_norml2(std::vector<float> inputMat, int nRows, int nCols); 

// **********************************************
// Normalize each col of matrix to range [a,b] 
// Input: + matrix vector<float> 
//        + nRows of matrix 
//        + nCols of matrix 
//        + lower bound value a 
//        + upper bound value b     
// **********************************************
std::vector<float> normalize_matrix(std::vector<float> inputMat, int nRows, int nCols,  float a, float b);

//**************************************************************
// Compute l2-norm of vector 
//***********************************************************
float l2_norm(std::vector<float> const& u);  

//********************************************************
// Compute inner_product of 2 vector 
//********************************************************
float inner_product(std::vector<float> u, std::vector<float> v);

//*************************************************************
// Matrix multiply C = A*B (Row major )  
// Input : +  A [nRows_A ,nCols_A] 
//         +  B [nCols_A, nCols_B]      
//         + nRows_A : number of rows of matrix A 
//         + nCols_A: number of cols of matrix A 
//         + nCols_B: number of cols of matrix B 
// Output  + Matrix C [nRows_A, nCols_B] 
//***********************************************************
std::vector<float> matrix_multiply(std::vector<float> A, std::vector<float> B , int nRows_A, int nCols_A, int nCols_B);


//**********************************************************
// compute variance of a vector 
//*********************************************************
float compute_variance(const std::vector<float> inVector);
#endif 


