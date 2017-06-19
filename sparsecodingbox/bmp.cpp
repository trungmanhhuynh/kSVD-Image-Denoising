#include "bmp.h" 
#include "utilities.h" 
#include "iterator" 

// ****************************************************************
// MATCHING PURSUIT ALGORITM (OMP) FOR SPARSE CODING 
// Input:  + input vector [feature * number of input vector] 
//         + dictionary   [feature * K]         
// Output: + sparse coding of input vector [ K * number of input vector]
//         + atomUsedbyInput [K * vector<int> ] 
//*****************************************************************
std::vector<float> bmp(std::vector<float> inputData, std::vector<float> dictionary,
      std::vector<std::vector<int> >  &atomUsedByInput, parameters param){

   std::vector<float> sparseCode ;
   int inputSize = inputData.size()/param.featureSize;

   float weight =0  ;
   int  chosenAtomIdx  ;

   //1. For each input patch
   for(int inputIdx = 0 ; inputIdx < inputSize ; inputIdx++){

      //std::cout <<BOLDRED << "inputIdx = " << inputIdx << RESET <<  std::endl ;
      //Assign residual vector = input patch 
      std::vector<float> residualVec(inputData.begin() + inputIdx*param.featureSize, inputData.begin() + (inputIdx+1)*param.featureSize);

      //Initialize sparseCode for this input vector 
      std::vector<float> thisSparseCode(param.nAtoms,0) ;

      //start basic pursuit for each input 
      int iter = 0 ;
      while(iter < param.L){

         float maxProduct = -pow(2,32) ;
         std::vector<float> chosenAtom(param.nAtoms);
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
                //std::cout << "max product =" << maxProduct <<  std::endl ;

            }
         }

         //std::cout << "final select atom idx = " << chosenAtomIdx <<  std::endl ;
         //std::cout << "final max product =" << maxProduct <<  std::endl ;
         //std::cout <<"weight = " << weight << std::endl ;

         // update the dictionary buffer
         atomUsedByInput[chosenAtomIdx].push_back(inputIdx) ;

		
         //store coefficient 
         thisSparseCode[chosenAtomIdx] = weight ;

         //calcualte residual R = R - weight*chosenAtom ; 
         std::vector<float> tempVec(chosenAtom.size())  ;
         //std::cout <<"residual = " << std::endl; 
         //copy(residualVec.begin(), residualVec.begin() + 64, std::ostream_iterator<float>(std::cout, " ")); std::cout << std::endl; 
         //std::cout <<"chosenAtom = " << std::endl;
         //copy(chosenAtom.begin(), chosenAtom.begin() + 64, std::ostream_iterator<float>(std::cout, " ")); std::cout << std::endl;

         std::transform(chosenAtom.begin(), chosenAtom.end(), tempVec.begin(), std::bind1st(std::multiplies<float>(),weight));
         std::transform(residualVec.begin(), residualVec.end(), tempVec.begin(),residualVec.begin(),std::minus<float>());
			
			//std::cout << BOLDGREEN << "l2 norm (residual ) = " << l2_norm(residualVec) << RESET << std::endl ; 
			if(l2_norm(residualVec) < 0.001) break ; 
		
         //increase iteration
         iter = iter + 1;
      }

		 //copy(thisSparseCode.begin(), thisSparseCode.begin() + 64, std::ostream_iterator<float>(std::cout, " ")); std::cout << std::endl;	
      sparseCode.insert(sparseCode.end(),thisSparseCode.begin(),thisSparseCode.end());
   }

   return sparseCode ;

}
