#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "config.h" 

#ifndef KSVD_H 
#define KSVD_H 



struct kSVDParameters{
   int kSVDiteration ;
   int nAtoms ;               //number of atoms of dictionary 
   int featureSize ;          //number of rows of dictionary 
   int debug ;
};

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
std::vector<float> kSVD_initialize_dictionary(const std::vector<float> trainPatches, kSVDParameters param);



//***************************************************************************
// * Updating dictionary using K-SVD Algorithm
//  Input  + inputData  (nInput x featureSize)
//         + sparseCode (nInput x dictionarySize) 
//         + dictioanry (dictionarySize x featureSIze) 
//         +atomUsedbyInput 
//  Output + dictionary (dictionarySIze x featureSize) 
// * ************************************************************************
void kSVD_update_dictionary(std::vector<float> inputData,std::vector<float> &sparseCode,
      std::vector<float> &dictionary, kSVDParameters param) ;
//***************************************************************
// * kSVD algorithm 
// * CPU version  
// * **************************************************************
void kSVD(std::vector<float> sparseCode, std::vector<float> &dictionary, kSVDParameters param); 


//************************************************************
// Compute resconstruction error ||Y -DX||
// Input: + InputData Y [featureSize * nInput]
//        + SparseCode X [DicSize * nInput 
//        + Dictionary D [featureSize * DicSize] 
//************************************************************
float kSVD_compute_rescontruction_error(std::vector<float> Y,std::vector<float> D,std::vector<float> X, kSVDParameters param) ;

#endif 
