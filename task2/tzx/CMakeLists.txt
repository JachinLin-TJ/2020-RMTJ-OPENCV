cmake_minimum_required(VERSION 3.5)
project(contour)
set(CMAKE_CXX_STANDARD 14)
add_definitions(-std=c++11)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(contour main.cpp)

target_link_libraries(contour ${OpenCV_LIBS})
