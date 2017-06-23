//system libs 
#include <iomanip>
#include <iostream>
#include <vector>             //std::vector 

//opencv libs
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "config.h" 

#ifndef OMP_H
#define OMP_H

struct OMPParameters{
   int nAtoms ;               //number of atoms of dictionary 
   int featureSize ;          //number of rows of dictionary 
   int L ;                    //sparsity level, #number of non-zero element
   int debug ;
};



// ****************************************************************
// BASIC MATCHING PURSUIT ALGORITM FOR SPARSE CODING 
// Input:  + input vector [feature * number of input vector] 
//         + dictionary   [feature * K]         
// Output: + sparse coding of input vector [ K * number of input vector]
//         + atomUsedbyInput [K * vector<int> ] 
//*****************************************************************
std::vector<float> omp(std::vector<float> inputData, std::vector<float> dictionary, OMPParameters param) ; 

std::vector<float> omp_v1(std::vector<float> inputData, std::vector<float> dictionary, OMPParameters param) ;

#endif

