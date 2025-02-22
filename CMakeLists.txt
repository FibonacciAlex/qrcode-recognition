cmake_minimum_required(VERSION 3.10)

#########################################################
# Find OpenCV
#########################################################

find_package(OpenCV REQUIRED)


message(STATUS "OpenCV library status:")
message(STATUS "    config: ${OpenCV_DIR}")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

message(STATUS "    include PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
# set C++ standard, please replace to your system path
set(CMAKE_C_COMPILER "D:/software/mingw64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "D:/software/mingw64/bin/g++.exe")
set(CMAKE_MAKE_PROGRAM "D:/software/mingw64/bin/make.exe")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(main)

# add source file
add_executable(main src/main.cpp)

# Link with OpenCV Library
target_link_libraries(main PRIVATE ${OpenCV_LIBS})
