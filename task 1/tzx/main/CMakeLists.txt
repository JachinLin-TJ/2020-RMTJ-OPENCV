cmake_minimum_required(VERSION 3.5)
project(hello)
set(CMAKE_CXX_STANDARD 14)
add_definitions(-std=c++11)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(hello main.cpp)

target_link_libraries(hello ${OpenCV_LIBS})
