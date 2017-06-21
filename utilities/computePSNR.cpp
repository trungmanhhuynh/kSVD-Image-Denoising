#include "utilities.h"

//********************************************************
// Compute Signal to Noise Ratio 
//*******************************************************
float compute_PSNR(std::vector<float> inputImg, std::vector<float> noiseImg){

   float accErr = 0 ;
   for(int i = 0 ; i < inputImg.size() ; i++){
      accErr = accErr + pow(inputImg[i] - noiseImg[i],2);
   }
   accErr = accErr/inputImg.size();
   float PSNR = 20*log10(255/sqrt(accErr));

   return PSNR ;

}



