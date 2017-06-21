#include "utilities.h" 

//*********************************************************************
// Function: patches2img
// Description: Given a set of patches, we arrange patches into 
// 				grayscale images in collumn order					
// Input:  + patches: 1 dimension of patchWidth * patchHeight 
// 		  + patchWidth : width of each patch 
// 		  + patchHeight: height of each patch
// 		  + nPatches_x : number of patches in x directions (width) 
// 		  + nPatches_y: number of patches in y direction (height) 
// 		  + sliding: define how patches overlapped   
//			
// Output: + img: 1D grayscale image 
// 		  + imgWeight: weight of each pixel (defines how many patches 
// 		  	 overlapped on it)
// **********************************************************************
std::vector<float> patches2img(std::vector<float> patches, 
									    std::vector<float> &imgWeight,
										 int patchWidth, 
										 int patchHeight, 
										 int nPatches_x, 
										 int nPatches_y,
										 int sliding){

   int imgWidth = patchWidth + sliding*(nPatches_x - 1) ;
   int imgHeight = patchHeight + sliding*(nPatches_y - 1) ;
	int patchSize = patchWidth*patchHeight ; 
   std::vector<float> img(imgWidth*imgHeight,0);        //initilize img vector
   std::vector<float> blockWeight(patchWidth*patchHeight,1);
   std::cout << " imgWidth = " << imgWidth << std::endl ;
   std::cout << " imgHeight = " << imgHeight << std::endl ;

   int patchIdx = 0 ;
   for(int i = 0 ; i + patchWidth <= imgWidth ; i = i + sliding){
      for(int j = 0; j + patchHeight <= imgHeight ; j = j + sliding){

         //std::cout << " i= " << i << "j =" << j << std::endl ;
         int startPatchIdx = i*imgHeight + j ;
         std::vector<float> thisPatch(patches.begin() + patchIdx*patchSize,patches.begin() + (patchIdx + 1)*patchSize);
         //std::cout << "thispatch = " << std::endl ; 
         //copy(thisPatch.begin(), thisPatch.end(), std::ostream_iterator<float>(std::cout, " ")); std::cout << std::endl;

         //copy each collumn of patch to image
         int colIdx = 0  ;
         for(int k = startPatchIdx ; k < startPatchIdx + patchWidth*imgHeight ; k  = k + imgHeight){
            std::transform(thisPatch.begin() + colIdx*patchHeight, thisPatch.begin() + (colIdx +1)*patchHeight,
                           img.begin() + k, img.begin() + k, std::plus<float>());
            std::transform(blockWeight.begin() + colIdx*patchHeight ,blockWeight.begin() + (colIdx + 1)*patchHeight,
                           imgWeight.begin() + k , imgWeight.begin() + k, std::plus<float>());

            colIdx = colIdx + 1 ;
         }

         patchIdx = patchIdx + 1;
      }
   }

   return img ;
}


