cmake_minimum_required(VERSION 2.8)

project(main )

include_directories(./)

find_package( OpenCV REQUIRED )

add_executable(main main.cpp denoise.cpp omp.cpp bmp.cpp kSVD.cpp utilities.cpp)
       
target_link_libraries(main ${OpenCV_LIBS})

 
