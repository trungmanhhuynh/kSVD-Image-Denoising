#include <iostream>
#include <iomanip>
#include <iterator> 
//opecv libs
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

//local libs
#include "denoise.h" 
#include "dictionarylearningbox/kSVD.h"
#include "sparsecodingbox/omp.h"
#include "config.h" 
#include "utilities.h"

int main(int argc, char** argv){

	//set parameters 
   parameters param;
   param.sigma = 25 ;
	param.sliding = 2 ; 
	param.kSVDiteration = 1 ;
   param.nAtoms = 256 ;
	param.imgWidth = 160 ; 
	param.imgHeight = 160 ; 
	param.patchWidth = 8 ; 
	param.patchHeight = 8 ; 
   param.featureSize = 8*8 ;
	param.L = 5 ; 

	//read image
	std::vector<float> inputImg = read_image("barbara160.png",param);  
	
	//std::vector<float> noiseImg = add_noise_to_image(inputImg, param); 
	std::vector<float> noiseImg = read_image("noisebarbara160.png",param); 

	std::cout << "PSNR(inputImg/noiseImg) = " << compute_PSNR(inputImg,noiseImg) << std::endl; ; 
	
	std::vector<float> noisePatches = img2patches(noiseImg,param); 

	std::cout <<"number of patches = " << noisePatches.size()/param.featureSize << std::endl ;

	//Intilize dictionary 
	std::vector<float> dictionary = initialize_dictionary(noisePatches,param) ; 

	//Write dictionary to image 
	dictionary2image(dictionary, param); 

	//Start K-SVD dictionary learning 
 	 kSVD(noisePatches,dictionary,param); 		

   dictionary2image(dictionary, param);

	std::cout <<"Denoising image ..." << std::endl ; 

	//sparse coding  
	std::vector<float> sparseCode = omp(noisePatches, dictionary ,param);
	
	//Resconstruct noisy image
	std::vector<float> recoverImgPatches = matrix_multiply(dictionary,sparseCode,param.featureSize,param.nAtoms,noisePatches.size()/param.featureSize) ;		

	//convert to image
   std::vector<float> imgWeight(param.imgWidth*param.imgHeight,0);
	std::vector<float> recoverImgVec = patches2img(recoverImgPatches, imgWeight , param); 

	  std::cout << "Weight COl 1   = " << std::endl ;
      copy(imgWeight.begin() + 0*param.imgHeight, imgWeight.begin() + 1*param.imgHeight, std::ostream_iterator<float>(std::cout, " ")); std::cout << std::endl;
      std::cin.get() ; 

	//Optimize ||Y - X|| 
	for(int i = 0 ; i < recoverImgVec.size(); i++){ 
		recoverImgVec[i] = (noiseImg[i] + 0.034*param.sigma*recoverImgVec[i])/(1 + 0.034*param.sigma*imgWeight[i]); 
	}

   std::cout << "PSNR(inputImg/recoveredImg) = " << compute_PSNR(inputImg,recoverImgVec) << std::endl; ;
	
	//convert imagePatches to grayscale img
	cv::Mat recoverImg = vector2mat_v1(recoverImgVec,160,160);
	normalize(recoverImg, recoverImg, 0.0, 1.0, CV_MINMAX, CV_32F);

	imshow("recover image",recoverImg); 
	cv::waitKey(0);
	
	return 0 ;
}

