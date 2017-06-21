#include "utilities.h" 

// *****************************************************************
// Function: IMG2PATCHES 
// Description: grayscale image into patches (blocks) 
// Input: + Grayscale image (vector<float> 1 dimension) - row major
//			 + imgWidth  = width of image
//			 + imgHeight = height of image 
//        + patchWidth = width of patches
//        + patchHeight = height of patches
//			 + sliding: sliding distance between patches
// Output: + imgPatches (1D) and col-major for each patch
//**********************************************************************
std::vector<float> img2patches(std::vector<float> img,
										 int imgWidth, 
										 int imgHeight,
										 int patchWidth, 
										 int patchHeight, 
										 int sliding){

   std::vector<float> imgPatch ;

    for(int i = 0 ; i + patchWidth <= imgWidth  ; i = i + sliding){
      for(int j = 0; j + patchHeight <= imgHeight ; j = j + sliding){

         //std::cout << " i= " << i << "j =" << j << std::endl ;
         std::vector<float> thisPatch;
         int startPatchIdx = i*imgHeight + j ;

         //collect each row of patch   
         for(int k = startPatchIdx ; k < startPatchIdx + patchWidth*imgHeight ; k  = k + imgHeight){
            thisPatch.insert(thisPatch.end(),img.begin() + k, img.begin() + k + patchHeight);
         }

         //concatenate this patch to image patch 
         imgPatch.insert(imgPatch.end(),thisPatch.begin(), thisPatch.end());
         //copy(thisPatch.begin(), thisPatch.end(), std::ostream_iterator<float>(std::cout, " "));
      }
   }
   return imgPatch ;
}



