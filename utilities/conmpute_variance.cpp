#include "utilities.h" 


//**********************************************************
// compute variance of a vector 
//*********************************************************
float compute_variance(const std::vector<float> inVector){

  //compute mean of this vector 
  float mean = 0 ;
  float sum = 0 ;

  for(int i = 0 ; i < inVector.size() ; i++){
      sum = sum + inVector[i] ;
  }
  mean  = sum/ inVector.size();

  float temp = 0 ;
  for(int i = 0; i < inVector.size(); i++){
   temp = temp + pow(inVector[i] - mean,2) ;
  }
  return temp/(inVector.size()) ;
}

