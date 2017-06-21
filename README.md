# Image Denoising using K-SVD with OMP for Sparse Coding
There is limited number of softwares/tools on C/C++ about kSVD and Sparse Coding available on web. This why I come up with with decision 
implement kSVD algorithm to solve its orginal problems: Image Denosing. 
However, my main goal for implementing this algorithm are:
  - For later optimization on multicore system 
  - Use it for my main research: multi-target tracking.

The algorithm uses Orthogonal Matching Pursuit (OMP) for sparsecoding and kSVD for dictionary learning. 

For Matlab code, < 1s by googling. 

## Getting Started

### Prerequisites

 - OpenCV 3.0 
 - Cmake 2.8 
 
 Tested on Centos 7.0

### Installing
At the current project directory  
mkdir build   
cmake ../    
make     

You can try to modify all parameters in main function. 

## Running the tests

Here I run the test with following parameters 

   //set parameters  
   parameters param;  
   param.sigma = 25 ;  
   param.sliding = 4 ;  
   param.kSVDiteration = 10 ;  
   param.nAtoms = 256 ;  
   param.imgWidth = 160 ;  
   param.imgHeight = 160 ;  
   param.patchWidth = 8 ;  
   param.patchHeight = 8 ;  
   param.featureSize = 8*8 ;  
   param.L = 5 ;  
   
  Results: (It is important to see the reduced resconstruction error, and PNSR)  

[manh@node18 build]$ ./main  
PSNR(inputImg/noiseImg) = 20.2078  
number of patches = 1521  
param.kSVDiteration = 10  
training iteration = 0  
 resconstruction Error = 20.4946  
training iteration = 1  
 resconstruction Error = 19.6167  
training iteration = 2  
 resconstruction Error = 19.2629  
training iteration = 3  
 resconstruction Error = 19.0181  
training iteration = 4  
 resconstruction Error = 18.8327  
training iteration = 5  
 resconstruction Error = 18.7264  
training iteration = 6  
 resconstruction Error = 18.6484  
training iteration = 7  
 resconstruction Error = 18.5715  
training iteration = 8  
 resconstruction Error = 18.5231  
training iteration = 9  
 resconstruction Error = 18.5006  

Denoising image ...  
PSNR(inputImg/recoveredImg) = 23.1781  

Orginal Image
![image](https://user-images.githubusercontent.com/13492723/27363750-8f01c0cc-55f3-11e7-974d-65847997e29a.png)
Noise Image (sigma  =25)
![image](https://user-images.githubusercontent.com/13492723/27363760-9803ebb4-55f3-11e7-8258-e82740163db1.png)

Orginal Dictionary
![image](https://user-images.githubusercontent.com/13492723/27363764-9fe0872a-55f3-11e7-8498-83bd535f61ad.png)

## Versioning

Version 1.0 : First Release  
Version 1.1: 
  - Fixed convert patches to images 
  - Organized files in dictionary learning toolbox and sparse coding toolbox 
Future Improvemenst:   
 - Fix bugs might have (I have not tried all different sliding parameters).   
 - Consider the atoms which have not been used, and similliar atoms.  
 
 ## FAQS
  1. Is program written follow row of column major ? 
     - The code are written follows column major (elements in array scanned in col first). I would think that will be easier way
     to implement dictionary matrix. 
  2. The code run slow, is there any optimazation yet ? 
      - No optimization is applied yet. We may using OpenMP and GPU for fast processing the implemenation. 
 
 ## Authors

 Huynh Manh - PhD student at UC Denver. 



