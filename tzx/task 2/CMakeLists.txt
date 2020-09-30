cmake_minimum_required(VERSION 3.5)
project(contour)

set(CMAKE_CXX_STANDARD 14)
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}")  #产物目录

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

aux_source_directory(. DIR_SRCS)
add_executable(contour ${DIR_SRCS})

target_link_libraries(contour ${OpenCV_LIBS})
