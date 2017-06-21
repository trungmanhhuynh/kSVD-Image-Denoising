#include <iostream>
#include <iomanip>
#include <iterator> 
//opecv libs
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

//local libs
#include "dictionarylearningbox/kSVD/kSVD.h"
#include "sparsecodingbox/omp.h"
#include "config.h" 
#include "utilities/utilities.h"


int main(int argc, char** argv){

	//read image
	std::vector<float> inputImg = read_image("barbara160.png");  

	std::vector<float> noiseImg = add_noise_to_image(inputImg, imgWidth,imgHeight,sigma); 

	std::cout << "PSNR(inputImg/noiseImg) = " << compute_PSNR(inputImg,noiseImg) << std::endl; ; 

	std::vector<float> noisePatches = img2patches(noiseImg,imgWidth,imgHeight,patchWidth,patchHeight,sliding); 

	std::cout <<"number of patches = " << noisePatches.size()/featureSize << std::endl ;

	//Set parameter for kSVD 
	kSVDParameters kSVDParam;
	kSVDParam.kSVDiteration = numberIterationOfkSVD ;
	kSVDParam.nAtoms = numberAtomsOfDictionary ;
	kSVDParam.featureSize = featureSize;

	//Intilize dictionary 
	std::vector<float> dictionary = kSVD_initialize_dictionary(noisePatches,kSVDParam) ; 

	//Write dictionary to image 
	show_dictionary(dictionary, kSVDParam.featureSize, kSVDParam.nAtoms); 

	//Start K-SVD dictionary learning 
	kSVD(noisePatches,dictionary,kSVDParam); 		

	show_dictionary(dictionary, kSVDParam.featureSize, kSVDParam.nAtoms);

	std::cout <<"Denoising image ..." << std::endl ; 

	//sparse coding  
	//Set parameter for sparse coding OMP 
	OMPParameters  OMPParam ; 
	OMPParam.nAtoms = kSVDParam.nAtoms; 
	OMPParam.L = numberOfCoefficients ; 
	OMPParam.featureSize = featureSize ; 
	std::vector<float> sparseCode = omp(noisePatches, dictionary ,OMPParam);

	//Resconstruct noisy image
	std::vector<float> recoverImgPatches = matrix_multiply(dictionary,sparseCode,featureSize,OMPParam.nAtoms,noisePatches.size()/OMPParam.featureSize) ;		

	//convert to image
	std::vector<float> imgWeight(imgWidth*imgHeight,0);
	std::vector<float> recoverImgVec = patches2img(recoverImgPatches,
			imgWeight,
			patchWidth, 
			patchHeight, 
			sqrt(recoverImgPatches.size()/featureSize) , // nPatches_x
			sqrt(recoverImgPatches.size()/featureSize) , // nPatches_y			
			sliding
			); 

	//Optimize ||Y - X|| 
	for(int i = 0 ; i < recoverImgVec.size(); i++){ 
		recoverImgVec[i] = (noiseImg[i] + 0.034*sigma*recoverImgVec[i])/(1 + 0.034*sigma*imgWeight[i]); 
	}

	std::cout << "PSNR(inputImg/recoveredImg) = " << compute_PSNR(inputImg,recoverImgVec) << std::endl; ;

	//convert imagePatches to grayscale img
	cv::Mat recoverImg = vector2mat_v1(recoverImgVec,160,160);
	normalize(recoverImg, recoverImg, 0.0, 1.0, CV_MINMAX, CV_32F);

	imshow("recover image",recoverImg); 
	cv::waitKey(0);

	return 0 ;
}

