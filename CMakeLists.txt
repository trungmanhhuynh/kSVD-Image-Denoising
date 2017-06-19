cmake_minimum_required(VERSION 2.8)

project(main )

include_directories(./)

find_package( OpenCV REQUIRED )

file(GLOB sparsecoding sparsecodingbox/omp.cpp) 

file(GLOB kSVD dictionarylearningbox/kSVD.cpp) 

add_executable(main main.cpp denoise.cpp ${sparsecoding} ${kSVD} utilities.cpp)
       
target_link_libraries(main ${OpenCV_LIBS})

 
