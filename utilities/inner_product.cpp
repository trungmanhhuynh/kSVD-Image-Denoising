#include "utilities.h" 

//********************************************************
// Compute inner_product of 2 vector 
//********************************************************
float inner_product(std::vector<float> u, std::vector<float> v){

   if(u.size() != v.size()){
      std::cout << "Two vector have different size " << std::endl ;
      exit(1);
   }

   float acc = 0.0 ;
   for(int i = 0 ; i < u.size() ;i++){
      acc = acc + u[i]*v[i];
   }

   return acc ;
}

