#include <numeric>      // std::inner_product
#include <algorithm> // for std::transform
#include <functional> 
#include <ctime>
#include <iterator>

#include "kSVD.h"
#include "config.h" 
#include "utilities/utilities.h" 
#include "sparsecodingbox/omp.h" 

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

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
std::vector<float> kSVD_initialize_dictionary(const std::vector<float> inputData, kSVDParameters param){

   std::vector<float> dictionary ;

   //Get patches dictionary to form D 
   for(int i = 0 ; i < param.nAtoms ; i++){
      dictionary.insert(dictionary.end(),inputData.begin() + i*param.featureSize, inputData.begin() + (i + 1)*param.featureSize);
   }

   std::vector<float> normalizedDictionary_l2 =  normalize_matrix_norml2(dictionary, param.featureSize, param.nAtoms);

   return normalizedDictionary_l2 ;
}

//***************************************************************************
// * Updating dictionary using K-SVD Algorithm
//  Input  + inputData  (nInput x featureSize)
//  		  + sparseCode (nInput x dictionarySize) 
//  		  + dictioanry (dictionarySize x featureSIze) 
//  		  +atomUsedbyInput 
//  Output + dictionary (dictionarySIze x featureSize) 
// * ************************************************************************
void kSVD_update_dictionary(std::vector<float> inputData, std::vector<float> &sparseCode,
		std::vector<float> &dictionary, kSVDParameters param){

	for(int atomIdx = 0 ; atomIdx < param.nAtoms ; atomIdx++){ 

		std::vector<int> relevantDataIndices ; 
		for(int i = 0 ; i < sparseCode.size()/param.nAtoms ;i++){
			if(sparseCode[i*param.nAtoms + atomIdx] != 0 ) 
				relevantDataIndices.push_back(i); 
		}


		// Only update atom that has been used in sparse coding steps  
		// and that atom is shared by more than 1 input data 
		if(relevantDataIndices.size() >= 1) {

			//std::cout <<"Updating atomIdx = " << atomIdx << std::endl ; 
			//collect all inputs that used this atom 
			std::vector<float> selectInput ;
			std::vector<float> selectSparseCode ;  
			//std::cout <<"relevant data size = " << relevantDataIndices.size() << std::endl ;
         // std::cout <<"select inputIdx = " << std::endl;

			for(int i = 0 ; i < relevantDataIndices.size() ; i++ ) {

				int inputIdx = relevantDataIndices[i];
				//std::cout << inputIdx << " ";
				selectInput.insert(selectInput.end(),inputData.begin() + inputIdx*param.featureSize, inputData.begin() + (inputIdx+1)*param.featureSize); 
				selectSparseCode.insert(selectSparseCode.end(),sparseCode.begin() + inputIdx*param.nAtoms, sparseCode.begin() + (inputIdx + 1)*param.nAtoms); 
			}
			// In theory, atom at atomIdx should be remove from dictionary 
			// its corresponding coef in sparseCode should also be remove 
			// Here, I just set the coef at row atomIdx = 0, that means we 
			// eliminate the contribution of current atom 
			int nSelectInput = selectInput.size()/param.featureSize ; 
			for(int i = 0 ; i < nSelectInput ; i++){
				selectSparseCode[i*param.nAtoms + atomIdx]  = 0 ; 
			}

			//E =  SelectCol-  dictionaryMatrix*SelectAlpha ;
			std::vector<float> errorMatrix(selectInput.size()) ; 
			std::vector<float> DX = matrix_multiply(dictionary,selectSparseCode,param.featureSize,param.nAtoms,nSelectInput);
			std::transform(selectInput.begin(), selectInput.end(), DX.begin(), errorMatrix.begin(),std::minus<float>());

			//Convert errorMatrix to opencv Mat to use SVD function  
			cv::Mat E = vector2mat_v1(errorMatrix, param.featureSize, nSelectInput ) ;
			//std::cout << "nSelectInput = " << nSelectInput << std::endl; 
			//std::cout << "featureSize = " << param.featureSize  << std::endl ; 
			//std::cout << "Error Matrix cols = " << E.cols << std::endl ; 
			//std::cout << "Error matrix rows = " << E.rows << std::endl ;
			
			//E = (cv::Mat_<float>(3,4) << 0, 1 , -1 , 2, 1 ,2 ,4 ,1, -1, 3 ,4, 1); 
			// Compute SVD 
			cv::Mat S,U,Vt ; 
			cv::SVD::compute(E,S,U,Vt) ;       
			cv::Mat V = Vt.t() ;
			
 			cv::Mat betterAtom ; 
			betterAtom = (-1)*U.col(0).clone() ; 			
			cv::Mat betterCoef = (-1)*S.at<float>(0,0)*V.col(0); 
			
				
			//std::cout <<"X col 1 = " << std::endl ; 
			//copy(selectSparseCode.begin() + 1*param.nAtoms, selectSparseCode.begin() + 2*param.nAtoms, std::ostream_iterator<float>(std::cout, " ")); std::cout << std::endl;

			//std::cout <<"beterAtom = " << betterAtom  <<std::endl ; 
         //std::cout <<"betterCoef = " << betterCoef  <<std::endl ;
			//std::cin.get() ; 	
			// get the first col in U eigen vector matrix;  
			std::vector<float> firstCol =  mat2vector_v1(betterAtom) ;
			//copy(firstCol.begin(), firstCol.end(), std::ostream_iterator<float>(std::cout, " ")); std::cout << std::endl;

			//replace select atom 
			std::copy(firstCol.begin(),firstCol.end(),dictionary.begin() + atomIdx*param.featureSize);
				
			//update better coefficients 
		   for(int i = 0 ; i < relevantDataIndices.size() ; i++ ) {
            int inputIdx = relevantDataIndices[i];
            //std::cout <<"select inputIdx = " << inputIdx << std::endl ;
            sparseCode[inputIdx*param.nAtoms + atomIdx] = betterCoef.at<float>(i) ; 
         }

			//std::cin.get();  
		}	  
	}

}

//***************************************************************
// * kSVD algorithm 
// * CPU version  
// * **************************************************************
void kSVD(std::vector<float> &sparseCode, std::vector<float> &dictionary, std::vector<float> inputData, kSVDParameters param){

	std::cout << "param.kSVDiteration = " << param.kSVDiteration << std::endl ;
	/*
	std::cout << "input data col 8 = " << std::endl ;
	copy(inputData.begin() + 7*param.featureSize, inputData.begin() + 8*param.featureSize, std::ostream_iterator<float>(std::cout, " ")); std::cout << std::endl;
	std::cin.get() ;
	
	for(int i =0 ; i < 10 ; i++){	
 		std::cout << "dictionary col  = " << i + 1<< std::endl ;
  		copy(dictionary.begin() + i*param.featureSize, dictionary.begin() + (i+1)*param.featureSize, std::ostream_iterator<float>(std::cout, " ")); std::cout << std::endl;
  	   std::cin.get() ;
	}
	*/	
	//Start K-SVD dictionary learning 
	
	OMPParameters OMPParam; 
	OMPParam.nAtoms = param.nAtoms; 
	OMPParam.featureSize = param.featureSize; 
	OMPParam.L = param.sparsityThres ;     

	for(int iter = 0 ; iter < param.kSVDiteration ; iter++){
    
      std::cout <<"kSVD training iteration = " << iter << ": " ;
		//find sparse codes  
      //std::cout << "Finding sparse code... " << std::endl ;
      double omp_timeStart = get_seconds(); 
      sparseCode = omp(inputData, dictionary,OMPParam);
		double omp_timeEnd = get_seconds(); 

		//update dictionary 
      //std::cout << " Updating dictionary " << std::endl ;
      double updateDic_timeStart = get_seconds(); 
      kSVD_update_dictionary(inputData,sparseCode,dictionary,param);
		double updateDic_timeEnd = get_seconds(); 
		
      //compute rescontruction error ||Y - DX|
      float resError = kSVD_compute_rescontruction_error(inputData,dictionary,sparseCode,param);
      std::cout << "total error = " << resError << std::endl ;
		std::cout << "OMP: " << (omp_timeEnd - omp_timeStart)*1e3 << " ms" << std::endl ; 
		std::cout << "Update Dictionary: " << (updateDic_timeEnd - updateDic_timeStart)*1e3 << " ms" << std::endl ;
     // std::cin.get();  
	}

}

//************************************************************
// Compute resconstruction error ||Y -DX||
// Input: + InputData Y [featureSize * nInput]
//        + SparseCode X [DicSize * nInput 
//        + Dictionary D [featureSize * DicSize] 
//************************************************************
float kSVD_compute_rescontruction_error(std::vector<float> Y, std::vector<float> D, std::vector<float> X, kSVDParameters param){

   int nCols_X = X.size()/param.nAtoms ;
   //std::vector<float> Dtest(9,1); 
   //std::vector<float> Xtest(9,2);    
   std::vector<float> DX = matrix_multiply(D,X,param.featureSize,param.nAtoms,nCols_X);
   //std::vector<float> DX = matrix_multiply(Dtest,Xtest,3,3,3); 
   //Y-DX 
   std::vector<float> errorMatrix(Y.size()) ;
   std::transform(Y.begin(), Y.end(), DX.begin(), errorMatrix.begin(),std::minus<float>());

   ///compute ||Y-DX|| which is l2_norm of each col in errorMatrix   
   float accError = 0 ;
   for(int i = 0 ; i < errorMatrix.size() ; i++){
      accError = accError + pow(errorMatrix[i],2);
   }

   float resError = sqrt(accError/errorMatrix.size()) ;
   return resError ;
}


