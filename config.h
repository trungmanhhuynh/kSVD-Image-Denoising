//system libs 
#include <iomanip>
#include <iostream>
#include <vector>             //std::vector 

//opencv libs
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#ifndef CONFIG_H 
#define CONFIG_H

#define GREEN   "\033[32m"      /* Green */
#define RED     "\033[31m"      /* Red */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */

#define RESET   "\033[0m"
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */

//Changes these paramaters to make effect on results 
const	int sigma = 25 ; 
const	int sliding = 2; 
const	int imgWidth = 160 ; 		//image is resized to this value
const	int imgHeight = 160 ; 	   //image is resized to this value 
const int patchWidth = 8 ; 
const	int patchHeight = 8 ; 
const	int featureSize = patchWidth*patchHeight; 
const int numberIterationOfkSVD = 10 ;
const int numberAtomsOfDictionary = 256 ;
const int numberOfCoefficients = 5 ;   
#endif

