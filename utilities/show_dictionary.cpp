#include "utilities.h"

// **************************************************
// Function: show_dictionary
// Convert dictionary to image for displaying. Each atom
// represent a patch in images.
// Input: + vector<float> dictionary 
//        + featureSize: number of rows of dictionary 
//        + nAtoms: number of cols of dictionary 
// ***********************************************
void show_dictionary(std::vector<float> dictionary, int featureSize, int nAtoms ){

   std::vector<float> dictionary_01 =  normalize_matrix_range(dictionary,featureSize,nAtoms, 0 , 1) ;

   //construct pair of atom and its variance for later sorting
   std::vector<Variances> pairAtomVariance(nAtoms);
   for(int i = 0 ; i < nAtoms ; i++){
      std::vector<float> thisAtom(dictionary_01.begin() + i*featureSize, dictionary_01.begin() +(i+1)*featureSize);
      pairAtomVariance[i].atom = thisAtom ;
      pairAtomVariance[i].variance = compute_variance(thisAtom);
   }

   //sort by variance 
   sort(pairAtomVariance.begin(), pairAtomVariance.end(), compareVariances);

   //copy the atom in pair back to dictionary
   std::vector<float> dictionaryTemp;
   for(int i = 0; i < nAtoms ; i++){
      dictionaryTemp.insert(dictionaryTemp.end(),pairAtomVariance[i].atom.begin(), pairAtomVariance[i].atom.end());
      //std::cout << " variance = " << pairAtomVariance[i].variance << std::endl ; 
   }

	int patchWidth = sqrt(featureSize); 
	int patchHeight = sqrt(featureSize); 	
	int dicImgWidth = sqrt(nAtoms)*patchWidth; 
	int dicImgHeight = sqrt(nAtoms)*patchHeight; 	
	
   std::vector<float> imgWeight(dicImgWidth*dicImgHeight,0);
   std::vector<float> dicImg = patches2img(dictionaryTemp,
														 imgWeight,
														 patchWidth,  //patchWidth
														 patchHeight,  //patchHeight
														 sqrt(nAtoms),			//nPatches_x
														 sqrt(nAtoms),			//nPatches_y
														 patchHeight);	//sliding distance
									
   cv::Mat dicImg_cvMat = vector2mat_v1(dicImg,dicImgWidth,dicImgHeight);

   //Resize image  
   cv::resize(dicImg_cvMat,dicImg_cvMat,cv::Size(320,320));

   //display dictionary 
   imshow("dictionary" , dicImg_cvMat);
   //imwrite("dictionary_0.jpg",grayDicImg); 
   cv::waitKey(0);

}
   
