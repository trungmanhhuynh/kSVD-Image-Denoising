cmake_minimum_required(VERSION 2.8)

project(main )

include_directories(./)

find_package( OpenCV REQUIRED )

find_package( CUDA REQUIRED )

set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};-gencode arch=compute_60,code=sm_60)

file(GLOB utilities utilities/*.cpp utilities/*.cu)

file(GLOB sparsecoding sparsecodingbox/omp.cpp
							  sparsecodingbox/omp_v1.cpp) 

file(GLOB kSVD dictionarylearningbox/kSVD/kSVD.cpp) 

#add -fopenmp flag
SET(GCC_COVERAGE_COMPILE_FLAGS "-fopenmp")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

cuda_add_executable(denoise denoise.cpp ${sparsecoding} ${kSVD} ${utilities})
       
target_link_libraries(denoise ${OpenCV_LIBS} cublas cusolver)

 
