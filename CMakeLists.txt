cmake_minimum_required(VERSION 2.8)

project(main )

include_directories(./)

find_package( OpenCV REQUIRED )

file(GLOB utilities utilities/*.cpp)

file(GLOB sparsecoding sparsecodingbox/omp.cpp) 

file(GLOB kSVD dictionarylearningbox/kSVD/kSVD.cpp) 

#add -fopenmp flag
SET(GCC_COVERAGE_COMPILE_FLAGS "-fopenmp")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

add_executable(denoise denoise.cpp ${sparsecoding} ${kSVD} ${utilities})
       
target_link_libraries(denoise ${OpenCV_LIBS})

 
