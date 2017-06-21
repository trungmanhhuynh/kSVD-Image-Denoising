#include "utilities.h" 


//********************************************************
// Function: Add noise to image 
// Description: Add Gaussian noise with sigma value to image
//	Input: + inputImage: vector<float> 
//			 + imgWidth: width of image 
//			 + imgHeight: height of image
//			 + sigma 	  : variance of noise 
//*******************************************************
std::vector<float> add_noise_to_image(std::vector<float> inputImg,
												  int imgWidth, 
												  int imgHeight, 
												  int sigma){

   cv::Mat noise = cv::Mat(imgWidth,imgHeight, CV_32F);
   cv::randn(noise, 0,sigma);
   std::vector<float> noiseVec = mat2vector_v1(noise);
   std::vector<float> noiseImg(inputImg.size()) ;
   std::transform(inputImg.begin(), inputImg.end(), noiseVec.begin(), noiseImg.begin(),std::plus<float>());

   //Show noised image 
   cv::Mat noiseImg_cvMat = vector2mat_v1(noiseImg,imgWidth,imgHeight) ;
   cv::Mat noiseImg_display ;
   cv::normalize(noiseImg_cvMat,noiseImg_display, 0.0, 1.0, CV_MINMAX, CV_32F);

   imshow("noisy input",noiseImg_display) ;
   cv::waitKey(0);
   return noiseImg ;
}

