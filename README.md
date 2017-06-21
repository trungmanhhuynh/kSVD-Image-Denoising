

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

//Changes these paramaters to make effect on results 
const int sigma = 25 ;  
const int sliding = 2;
const int imgWidth = 160 ;       //image is resized to this value  
const int imgHeight = 160 ;      //image is resized to this value   
const int patchWidth = 8 ;  
const int patchHeight = 8 ;  
const int featureSize = patchWidth*patchHeight;  
const int numberIterationOfkSVD = 10 ;   
const int numberAtomsOfDictionary = 256 ;  
const int numberOfCoefficients = 5 ;  

   
  Results: (It is important to see the reduced resconstruction error, and PNSR)    

[manh@node18 build]$ ./denoise   
PSNR(inputImg/noiseImg) = 20.2078  
number of patches = 5929  
param.kSVDiteration = 10  
training iteration = 0  
Finding sparse code...   
 Updating dictionary   
 resconstruction Error = 23.0418     
training iteration = 1  
Finding sparse code...   
 Updating dictionary   
 resconstruction Error = 22.4011  
training iteration = 2  
Finding sparse code...   
 Updating dictionary   
 resconstruction Error = 22.0507  
training iteration = 3  
Finding sparse code...   
 Updating dictionary   
 resconstruction Error = 21.8134  
training iteration = 4  
Finding sparse code...   
 Updating dictionary   
 resconstruction Error = 21.6354  
training iteration = 5  
Finding sparse code...   
 Updating dictionary   
 resconstruction Error = 21.4912  
training iteration = 6  
Finding sparse code...   
 Updating dictionary   
 resconstruction Error = 21.3799  
training iteration = 7  
Finding sparse code...   
 Updating dictionary   
 resconstruction Error = 21.2703  
training iteration = 8  
Finding sparse code...   
 Updating dictionary   
 resconstruction Error = 21.1875  
training iteration = 9  
Finding sparse code...   
 Updating dictionary   
 resconstruction Error = 21.1174  

Denoising image ...  
 imgWidth = 160    
 imgHeight = 160  

| Original Image | Noise Image (sigma  =25) PSNR = 20.2078| Original Dictionary |
| ------------- | ------------- |-------------|
| ![image](https://user-images.githubusercontent.com/13492723/27363750-8f01c0cc-55f3-11e7-974d-65847997e29a.png) | ![image](https://user-images.githubusercontent.com/13492723/27363760-9803ebb4-55f3-11e7-8258-e82740163db1.png) | ![image](https://user-images.githubusercontent.com/13492723/27363764-9fe0872a-55f3-11e7-8498-83bd535f61ad.png) |

 | Original Image | Recovered Image PSNR = 25.4675| 10th Dictionary |
| ------------- | ------------- |-------------|
| ![image](https://user-images.githubusercontent.com/13492723/27363750-8f01c0cc-55f3-11e7-974d-65847997e29a.png) | ![image](https://user-images.githubusercontent.com/13492723/27363944-b9d9cbd6-55f4-11e7-8dd1-ae62f14d064a.png) | ![image](https://user-images.githubusercontent.com/13492723/27363953-c464229a-55f4-11e7-9691-44bfe77baf37.png) |


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
      
  3. Can we make a better results ?
    - Definitely. It is obvious that in this code, we trained dictionary on noised image only (not using DCT or global dictionary).
    Thus, the results can not be comparable with using pre-trained DCT or global one. 
  
 
 ## Authors

 Huynh Manh - PhD student at UC Denver. 



