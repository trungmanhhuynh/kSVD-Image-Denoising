//system libs 
#include <iomanip>
#include <iostream>
#include <vector>             //std::vector 

//opencv libs
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "config.h" 

#ifndef BMP_H
#define BMP_H


// ****************************************************************
// BASIC MATCHING PURSUIT ALGORITM FOR SPARSE CODING 
// Input:  + input vector [feature * number of input vector] 
//         + dictionary   [feature * K]         
// Output: + sparse coding of input vector [ K * number of input vector]
//         + atomUsedbyInput [K * vector<int> ] 
//*****************************************************************
std::vector<float> bmp(std::vector<float> inputData, std::vector<float> dictionary,
      std::vector<std::vector<int> >  &atomUsedByInput, parameters param) ; 

#endif

