#include "denoise.h" 
#include "config.h" 
#include "utilities.h" 
#include <math.h>       /* log10 */

//***********************************************
// Read gray-scale image and store in 1D vector
//**********************************************
std::vector<float> read_image(cv::String imageName,parameters param){

	cv::Mat img_cvMat = cv::imread(imageName) ;
	if(!img_cvMat.data ){
		std::cout << BOLDRED << "Could not open image" << RESET << std::endl ;
		exit(1);
	}
	cvtColor(img_cvMat, img_cvMat, CV_BGR2GRAY );

	//Resize image to 160x160     
	//cv::resize(img_cvMat,img_cvMat,cv::Size(param.imgWidth,param.imgHeight));

	//show input image
	cv::imshow("Input Image",img_cvMat);
	cv::waitKey(0);

	//convert to float type image 
	//remember to convert back to CV_8U for displaying
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
//********************************************************
// Add noise to image 
//
//*******************************************************

std::vector<float> add_noise_to_image(const std::vector<float> inputImg, parameters param){

	cv::Mat noise = cv::Mat(param.imgWidth,param.imgHeight, CV_32F);
	cv::randn(noise, 0, param.sigma);
	std::vector<float> noiseVec = mat2vector(noise);
	std::vector<float> noiseImg(inputImg.size()) ;
	std::transform(inputImg.begin(), inputImg.end(), noiseVec.begin(), noiseImg.begin(),std::plus<float>());

	//Show noised image 
	cv::Mat noiseImg_cvMat = vector2mat_v1(noiseImg,param.imgWidth,param.imgHeight) ;
	cv::Mat noiseImg_display ;
	cv::normalize(noiseImg_cvMat,noiseImg_display, 0.0, 1.0, CV_MINMAX, CV_32F);


	imshow("noisy input",noiseImg_display) ;
	cv::waitKey(0);
	return noiseImg ;
}


//*********************************************************
// INIT DICTIONARY MATRIX
// We initialize dictionary by select randomly 441 patches [8x8] 
// from the training patches
// Input: 
//        + Training patches
// Output:
//       + D [featureDim x nInputs] = [64 x 500] 
//       + D is stored in collum vector (1D)    
// *****************************************************
std::vector<float> initialize_dictionary(const std::vector<float> inputData,parameters param){

	std::vector<float> dictionary ;

	//Get patches dictionary to form D 
	for(int i = 0 ; i < param.nAtoms ; i++){
		dictionary.insert(dictionary.end(),inputData.begin() + i*param.featureSize, inputData.begin() + (i + 1)*param.featureSize);
	}

	std::vector<float> normalizedDictionary_l2 =  normalize_matrix_norml2(dictionary, param.featureSize, param.nAtoms);

	return normalizedDictionary_l2 ;
}

//*********************************************************************
// Divide grayscale image into blocks 
// Input: + Grayscale image (vector<float> 1 dimension) - row major
//        + width of blocks
//        + height of blocks 
// Output: + imgPatches (1D) and row-major for each patch
//**********************************************************************
std::vector<float> img2patches(const std::vector<float> img, parameters param){

	int nPatch = param.imgWidth*param.imgHeight/param.featureSize ;          //number of non-overlapped patches
	std::vector<float> imgPatch ;

	for(int j = 0 ; j <= param.imgHeight - param.patchHeight ; j = j + param.sliding){ 
		for(int i = 0; i <= param.imgWidth - param.patchWidth; i = i + param.sliding){ 

			//std::cout << " i= " << i << "j =" << j << std::endl ;
			std::vector<float> thisPatch;
			int startPatchIdx = j*param.imgHeight + i ;

			//collect each row of patch   
			for(int k = startPatchIdx ; k < startPatchIdx + param.patchHeight*param.imgHeight ; k  = k + param.imgHeight){
				thisPatch.insert(thisPatch.end(),img.begin() + k, img.begin() + k + param.patchHeight);
			}

			//concatenate this patch to image patch 
			imgPatch.insert(imgPatch.end(),thisPatch.begin(), thisPatch.end());
			//copy(thisPatch.begin(), thisPatch.end(), std::ostream_iterator<float>(std::cout, " "));
		}
	}
	return imgPatch ;
}


//*********************************************************************
// Convert patches to image (std::vector<float> )  
// Input:  + patches : consist all non-overallped patches of images (row-major) 
// Output: + grayImage (vector<float>) 
//**********************************************************************
std::vector<float> patches2img(const std::vector<float> patches, std::vector<float> &imgWeight,  parameters param){

	int nPatches = patches.size()/param.featureSize; 
	int nPatchesPerDimension = sqrt(nPatches);
	int imgWidth = param.patchWidth + param.sliding*(nPatchesPerDimension - 1) ; 
	int imgHeight = param.patchHeight + param.sliding*(nPatchesPerDimension - 1) ;
	std::vector<float> img(imgWidth*imgHeight,0);        //initilize img vector
	std::vector<float> blockWeight(param.patchWidth*param.patchHeight,1); 
	std::cout << " imgWidth = " << imgWidth << std::endl ; 
	std::cout << " imgHeight = " << imgHeight << std::endl ;

	int patchIdx = 0 ; 	
	for(int i = 0 ; i <= imgWidth - param.patchWidth ; i = i + param.sliding){
		for(int j = 0; j <= imgHeight - param.patchHeight; j = j + param.sliding){

			//std::cout << " i= " << i << "j =" << j << std::endl ;
			int startPatchIdx_inImg = i*imgWidth + j ;
			std::vector<float> thisPatch(patches.begin() + patchIdx*param.featureSize,patches.begin() + (patchIdx + 1)*param.featureSize);
			//std::cout << "thispatch = " << std::endl ; 
			//copy(thisPatch.begin(), thisPatch.end(), std::ostream_iterator<float>(std::cout, " ")); std::cout << std::endl;

			//copy each row of patch to image
			int rowIdx = 0  ;
			for(int k = startPatchIdx_inImg ; k < startPatchIdx_inImg + param.patchHeight*imgWidth ; k  = k + imgWidth){
				std::transform(thisPatch.begin() + rowIdx*param.patchWidth, thisPatch.begin() + (rowIdx +1)*param.patchWidth, 
									img.begin() + k, img.begin() + k, std::plus<float>());
            std::transform(blockWeight.begin() + rowIdx*param.patchWidth ,blockWeight.begin() + (rowIdx + 1)*param.patchWidth,
							      imgWeight.begin() + k , imgWeight.begin() + k, std::plus<float>());

				rowIdx = rowIdx + 1 ;
			}

			patchIdx = patchIdx + 1; 
		}
	}

	return img ;
}

// **************************************************
// Convert dictionary to image for displaying 
// each atom will create one patch (8x8) of image. 
// Input: + vector<float> dictionary 
//        + nRows of dictionary 
//        + nCols of dictionary 
// ***********************************************
void dictionary2image(const std::vector<float> dictionary, parameters param){

	std::vector<float> dictionary_01 =  normalize_matrix(dictionary, param.featureSize, param.nAtoms, 0 , 1) ;

	//construc pair of atom and its variance for later sorting
	std::vector<Variances> pairAtomVariance(param.nAtoms);
	for(int i = 0 ; i < param.nAtoms ; i++){
		std::vector<float> thisAtom(dictionary_01.begin() + i*param.featureSize, dictionary_01.begin() +(i+1)*param.featureSize);
		pairAtomVariance[i].atom = thisAtom ;
		pairAtomVariance[i].variance = compute_variance(thisAtom);
	}

	//sort by variance 
	sort(pairAtomVariance.begin(), pairAtomVariance.end(), compareVariances);

	//copy the atom in pair back to dictionary
	std::vector<float> dictionaryTemp;
	for(int i = 0; i < param.nAtoms ; i++){
		dictionaryTemp.insert(dictionaryTemp.end(),pairAtomVariance[i].atom.begin(), pairAtomVariance[i].atom.end());
		//std::cout << " variance = " << pairAtomVariance[i].variance << std::endl ; 
	}

	param.sliding = 8 ;
   std::vector<float> imgWeight(sqrt(param.nAtoms)*param.patchWidth*sqrt(param.nAtoms)*param.patchHeight,0);
	std::vector<float> dicImg = patches2img(dictionaryTemp,imgWeight,param);
	cv::Mat dicImg_cvMat = vector2mat(dicImg,sqrt(param.nAtoms)*param.patchWidth,sqrt(param.nAtoms)*param.patchHeight);
	
	//Resize image to 160x160     
	cv::resize(dicImg_cvMat,dicImg_cvMat,cv::Size(320,320));

	//display dictionary 
	imshow("dictionary" , dicImg_cvMat);
	//imwrite("dictionary_0.jpg",grayDicImg); 
	cv::waitKey(0);

}
//********************************************************
// Compute Signal to Noise Ratio 
//*******************************************************
float compute_PSNR(std::vector<float> inputImg, std::vector<float> noiseImg){

	float accErr = 0 ; 
	for(int i = 0 ; i < inputImg.size() ; i++){ 
		accErr = accErr + pow(inputImg[i] - noiseImg[i],2);  
	}
	accErr = accErr/inputImg.size(); 
	float PSNR = 20*log10(255/sqrt(accErr)); 	

	return PSNR ;

}




