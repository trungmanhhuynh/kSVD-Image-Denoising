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

typedef std::vector<std::vector<float> > vector2d_float ;
typedef std::vector<std::vector<int> > vector2d_int ;


struct parameters{
	float sigma; 
	int sliding; 
   int kSVDiteration ;
   int nAtoms ;               //number of atoms of dictionary 
   int featureSize ;    		//number of rows of dictionary 
	int imgWidth ;
	int imgHeight ; 
	int patchWidth ; 
	int patchHeight ; 
	int L ; 							//sparsity level, #number of non-zero element
   int debug ;
};
#endif

