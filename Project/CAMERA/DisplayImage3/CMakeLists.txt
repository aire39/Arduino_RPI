cmake_minimum_required(VERSION 2.8)
project(DisplayImage)
find_package(OpenCV REQUIRED)
include_directories(/usr/local/include/libpng15)
include_directories(/usr/local/include/opencv)
add_executable(DisplayImage main.c)
target_link_libraries(DisplayImage /usr/local/lib/libpng15.so ${OpenCV_LIBS})

