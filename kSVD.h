#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "config.h" 

#ifndef KSVD_H 
#define KSVD_H 

//***************************************************************************
// * Updating dictionary using K-SVD Algorithm
//  Input  + inputData  (nInput x featureSize)
//         + sparseCode (nInput x dictionarySize) 
//         + dictioanry (dictionarySize x featureSIze) 
//         +atomUsedbyInput 
//  Output + dictionary (dictionarySIze x featureSize) 
// * ************************************************************************
void update_dictionary(std::vector<float> inputData,std::vector<float> &sparseCode,
      std::vector<float> &dictionary, parameters param) ;
//***************************************************************
// * kSVD algorithm 
// * CPU version  
// * **************************************************************
void kSVD(std::vector<float> sparseCode, std::vector<float> &dictionary, parameters param); 


//************************************************************
// Compute resconstruction error ||Y -DX||
// Input: + InputData Y [featureSize * nInput]
//        + SparseCode X [DicSize * nInput 
//        + Dictionary D [featureSize * DicSize] 
//************************************************************
float compute_rescontruction_error(std::vector<float> Y,std::vector<float> D,std::vector<float> X, parameters param) ;

#endif 
