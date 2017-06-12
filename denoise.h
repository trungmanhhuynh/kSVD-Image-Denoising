//system libs 
#include <iomanip>
#include <iostream>
#include <vector>             //std::vector 

//opencv libs
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "config.h" 
#ifndef DENOISE_H 
#define DENOISE_H




//***********************************************
// Read gray-scale image and store in 1D vector
//**********************************************
std::vector<float> read_image(cv::String imageName , parameters param) ;

//********************************************************
// Add noise to image 
//
//*******************************************************
std::vector<float> add_noise_to_image(const std::vector<float> inputImg, parameters param) ;


//*********************************************************
// INIT DICTIONARY MATRIX
// We initialize dictionary by select randomly 441 patches [8x8] 
// from the training patches
// Input: 
//        + Training patches
// Output:
//       + D [featureDim x nInputs] = [64 x 500] 
//       + D is stored in collum vector (1D)    
// *****************************************************
std::vector<float> initialize_dictionary(const std::vector<float> trainPatches, parameters param);

//*********************************************************************
// Divide grayscale image into blocks 
// Input: + Grayscale image (vector<float> 1 dimension) - row major
//        + width of blocks
//        + height of blocks 
// Output: + imgPatches (1D) and row-major for each patch
//**********************************************************************
std::vector<float> img2patches(const std::vector<float> img, parameters param);


//*********************************************************************
// Convert patches to image (std::vector<float> )  
// Input:  + patches : consist all non-overallped patches of images (row-major) 
// Output: + grayImage (vector<float>) 
//**********************************************************************
std::vector<float> patches2img(const std::vector<float> patches,  std::vector<float> &imgWeight, parameters param) ;

// **************************************************
// Convert dictionary to image for displaying 
// each atom will create one patche (8x8) of image. 
// Input: + vector<float> dictionary 
//        + nRows of dictionary 
//        + nCols of dictionary 
// ***********************************************
void dictionary2image(const std::vector<float> dictionary, parameters param);
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





#endif


