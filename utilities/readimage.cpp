#include "utilities.h"

//***********************************************
// Function: read_image 
// Description: Read gray scale image and store it to 
// 				 a 1D vector
// Input: + imgName
// Output: + vector<float> img
//**********************************************
std::vector<float> read_image(cv::String imageName){

   cv::Mat img_cvMat = cv::imread(imageName) ;
   if(!img_cvMat.data ){
      std::cout << "Could not open image" << std::endl ;
      exit(1);
   }
   cvtColor(img_cvMat, img_cvMat, CV_BGR2GRAY );

   //show input image
   cv::imshow("Input Image",img_cvMat);
   cv::waitKey(0);

   //convert to float type image 
   img_cvMat.convertTo(img_cvMat,CV_32FC1);

   //Convert image (cv::Mat) to array 1-D 
   std::vector<float> img = mat2vector_v1(img_cvMat);

   //testing
   //std::vector<float> imgtest_vector = patches2img(imgPatches,imgPatches.size()/patchSize);
   //cv::Mat imgtest_cvMat = vector2mat(imgtest_vector,rzHeight,rzWidth) ;
   //imgtest_cvMat.convertTo(imgtest_cvMat,CV_8UC1);
   //copy(imgtest_vector.begin(), imgtest_vector.begin() + 160, std::ostream_iterator<float>(std::cout, " ")); std::cout << std::endl;
   //std::cout << "imgtest_cvMat row 0 =  " <<std::endl << imgtest_cvMat.row(0) << std::endl ;  
   //cv::imshow("test",imgtest_cvMat);    
   //cv::waitKey(0); 

   return img ;

}

