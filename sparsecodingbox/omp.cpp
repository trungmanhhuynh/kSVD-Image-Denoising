#include "omp.h"				//orthorgonal matching pursuit 
#include "utilities/utilities.h" 
#include <iterator> 

// ****************************************************************
// MATCHING PURSUIT ALGORITM (OMP) FOR SPARSE CODING 
// Input:  + input vector [feature * number of input vector] 
//         + dictionary   [feature * K]         
// Output: + sparse coding of input vector [ K * number of input vector]
//         + atomUsedbyInput [K * vector<int> ] 
//*****************************************************************
std::vector<float> omp(std::vector<float> inputData, std::vector<float> dictionary,OMPParameters param){

   int inputSize = inputData.size()/param.featureSize;
	int inputIdx ; 
   std::vector<float> sparseCodeRes(inputSize*param.nAtoms) ;

   float weight =0  ;
   int  chosenAtomIdx  ;

   //1. For each input patch
   for(inputIdx = 0 ; inputIdx < inputSize ; inputIdx++){

      std::cout <<BOLDRED << "inputIdx = " << inputIdx << RESET <<  std::endl ;
      //Assign residual vector = input patch 
      std::vector<float> thisInput(inputData.begin() + inputIdx*param.featureSize, inputData.begin() + (inputIdx+1)*param.featureSize);
		std::vector<float> residualVec(thisInput.begin(),thisInput.end()); 	

      //Initialize sparseCode for this input vector 
      std::vector<float> thisSparseCode(param.nAtoms,0) ;
		
		std::vector<int> chosenAtomIdxList ; 		
		std::vector<float> chosenAtomList ; 
      //start basic pursuit for each input 
      int iter = 0 ;
      while(iter < param.L){

         float maxProduct = -pow(2,32) ;
         std::vector<float> chosenAtom(param.featureSize);
         //find the best match atom  
         for(int atomIdx = 0 ; atomIdx < param.nAtoms; atomIdx++){
            std::vector<float> thisAtom(dictionary.begin() + atomIdx*param.featureSize, dictionary.begin() + (atomIdx+1)*param.featureSize);
            float product = inner_product(thisAtom,residualVec);
            if(std::abs(product) > maxProduct){
               maxProduct = std::abs(product) ;
               chosenAtomIdx = atomIdx ;
               weight = product ;
               std::copy(thisAtom.begin(),thisAtom.end(),chosenAtom.begin());  ;
               // std::cout << "select atom idx = " << chosenAtomIdx <<  std::endl ;
               // std::cout << "max product =" << maxProduct <<  std::endl ;
					
            }
         }

        // std::cout << "final select atom idx = " << chosenAtomIdx <<  std::endl ;
        // std::cout << "final max product =" << maxProduct <<  std::endl ;
        // std::cout <<"weight = " << weight << std::endl ;
		//	std::cin.get() ;
			//find weights a = pinv(D)*residual ; 
		   //std::cout <<"chosenAtom_vec = " <<  std::endl ;
          //copy(chosenAtom.begin(), chosenAtom.end(), std::ostream_iterator<float>(std::cout, " ")); std::cout << std::endl;

		 
			chosenAtomIdxList.push_back(chosenAtomIdx); 			
			chosenAtomList.insert(chosenAtomList.end(),chosenAtom.begin(),chosenAtom.end()) ; 		


				
			cv::Mat chosenAtomMat = vector2mat_v1(chosenAtomList,param.featureSize,chosenAtomIdxList.size()); 
			cv::Mat thisInputMat = vector2mat_v1(thisInput,param.featureSize,1); 
			cv::Mat chosenAtomMat_pinvert ; 
			cv::invert(chosenAtomMat, chosenAtomMat_pinvert,cv::DECOMP_SVD); 
			cv::Mat weightList ; 
			weightList = chosenAtomMat_pinvert*thisInputMat; 
			
         //store coefficient 
			for(int i = 0 ; i < chosenAtomIdxList.size() ; i++){
		    		 int thisAtomIdx = chosenAtomIdxList[i] ; 
				    thisSparseCode[thisAtomIdx] = weightList.at<float>(i)  ;
			}

         //calcualte residual R = R - chosenAtomList*weighList ; 
         cv::Mat tempMat = chosenAtomMat*weightList ; 
			std::vector<float> tempVec = mat2vector_v1(tempMat.clone());
			std::transform(thisInput.begin(),thisInput.end(), tempVec.begin(),residualVec.begin(),std::minus<float>());
		
			 
			//std::cout <<"choose atom idx  = " << chosenAtomIdx << std::endl ;  
			//std::cout <<"weightList = " << weightList << std::endl ;
		   //std::cout <<"D = " <<  chosenAtomMat << std::endl ;
			//std::cout <<"D_vec = " <<  std::endl ;
         //copy(chosenAtomList.begin(), chosenAtomList.end(), std::ostream_iterator<float>(std::cout, " ")); std::cout << std::endl;


			//std::cout <<"D_invt = " << chosenAtomMat_pinvert << std::endl ;
         //std::cout <<"residual = " <<  std::endl ;
			//copy(residualVec.begin(), residualVec.end(), std::ostream_iterator<float>(std::cout, " ")); std::cout << std::endl;	
	      //std::cin.get();  
	
			//std::cout << BOLDGREEN << "l2 norm (residual ) = " << l2_norm(residualVec) << RESET << std::endl ; 
			if(l2norm_vec(residualVec) < 0.001) break ; 
		
         //increase iteration
         iter = iter + 1;
      }

		 //copy(thisSparseCode.begin(), thisSparseCode.begin() + 64, std::ostream_iterator<float>(std::cout, " ")); std::cout << std::endl;	
      sparseCodeRes.insert(sparseCodeRes.begin() + inputIdx*param.nAtoms,thisSparseCode.begin(),thisSparseCode.end());
   }
	
   return sparseCodeRes ;

}
