PROJECT(Hello)
cmake_minimum_required(VERSION 3.16)
find_package(OpenCV REQUIRED)

message(STATUS "OpenCV library status:")
message(STATUS "    version:    ${OpenCV_VERSION}")
message(STATUS "    headers:    ${OpenCV_INCLUDE_DIRS}")
message(STATUS "    libraries:  ${OpenCV_LIBS}")

INCLUDE_DIRECTORIES({OpenCV_INCLUDE_DIRS})
AUX_SOURCE_DIRECTORY(. SRC_LIST)

ADD_EXECUTABLE(hello main.cpp)
target_link_libraries(hello ${OpenCV_LIBS})
