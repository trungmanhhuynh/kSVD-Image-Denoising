//opencv
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

//c++
#include <iostream>
#include <iomanip>            //std::setprecision

//cublas
#include <cublas_v2.h>
#include <curand.h>

//thrust
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>



#ifndef UTILITIES_H 
#define UTILITIES_H 


//*****************************************************
// This file contains all function prototype
// in side directory utilities. Each function can be served
// for different purposes or applications. Please look inside
// each of the function for more details.
//***************************************************

// Convert patches to image   
std::vector<float> patches2img(std::vector<float> patches,
                               std::vector<float> &imgWeight,
                               int patchWidth,
                               int patchHeight, 
                               int nPatches_x,
                               int nPatches_y,
                               int sliding); 

// Convert image to patches
std::vector<float> img2patches(std::vector<float> img, 
                               int imgWidth, 
                               int imgHeight,
                               int patchWidth, 
                               int patchHeight, 
                               int sliding); 
// Read image 
std::vector<float> read_image(cv::String imageName); 

// Add noise to image
std::vector<float> add_noise_to_image(std::vector<float> inputImg,
                                      int imgWidth, 
                                      int imgHeight, 
                                      int sigma);


//show dictionary 
void show_dictionary(std::vector<float> dictionary, int featureSize, int nAtoms); 
//struct to group patch and variance, used for sorting patches 
struct Variances{
   std::vector<float> atom ;
   float variance ;
};
inline bool compareVariances(Variances p1, Variances p2){
   return  (p1.variance < p2.variance);
}


//********************************************************
// Compute Signal to Noise Ratio 
//*******************************************************
float compute_PSNR(std::vector<float> inputImg, std::vector<float> noiseImg); 


//**************************************************
// Convert matrix vector<float> to cv:Mat (collum major)
//*************************************************
cv::Mat vector2mat_v1(std::vector<float>  inputVector, int nRows, int nCols) ; //collumn-major


//**************************************************
// Convert cv::Mat to vector<float> (collumn major)
//*************************************************
std::vector<float> mat2vector_v1(const cv::Mat inputMat) ; 

// **********************************************
// Normalize each col of matrix to norm L2
// **********************************************
std::vector<float> normalize_matrix_norml2(std::vector<float> inputMat, int nRows, int nCols); 

// **********************************************
// Normalize each col of matrix to range [a,b] 
// **********************************************
std::vector<float> normalize_matrix_range(std::vector<float> inputMat, int nRows, int nCols,  float a, float b);

//**************************************************************
// Compute l2-norm of vector 
//***********************************************************
float l2norm_vec(std::vector<float> const& u);  

//********************************************************
// Compute inner_product of 2 vector 
//********************************************************
float inner_product(std::vector<float> u, std::vector<float> v);

//*************************************************************
// Matrix multiply C = A*B  
//***********************************************************
std::vector<float> matrix_multiply(std::vector<float> A, std::vector<float> B , int nRows_A, int nCols_A, int nCols_B);


//C = alpha*op(A)*op(B) + beta*C ; 
void mmul_cublas(std::vector<float> A, cublasOperation_t transa,  std::vector<float> B, cublasOperation_t transb ,std::vector<float> &C, int Arows, int Acols, int Bcols); 

//matrix-vector multiplication 
//y = α op ( A ) x + β y

void mvmul_cublas(std::vector<float> A, cublasOperation_t transa,  std::vector<float> x, std::vector<float> &y, int Arows, int Acols ); 


//**********************************************************
// compute variance of a vector 
//*********************************************************
float compute_variance(const std::vector<float> inVector);

//Get current time 
double get_seconds() ;

#endif 


