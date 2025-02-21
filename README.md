# qrcode-recognition
Computer Vision demo, use openCV to recognize qrcode

## Basic Technical Tool Requirements
- C++
- OpenCV
- VScode
- CMake

## Key Points
- The testing image is a color 2D barcode.
- The image may be rotated, scaled and slightly distorted.
- the image was simple encoding with 8 colors.
- There are 64 characters that can be encoded: the digits 0-9, the lower case a-z, the upper case A-Z, a full stop '.' and 
space. The encoding color was presented below.

## The encoding table
![image](https://github.com/user-attachments/assets/2b1af9b3-5032-4b92-bfa6-6423daed9818)


## How to run the code
Run the commands below:

`
mkdir build
cd build
cmake -G "MinGW Makefiles" \
-DCMAKE_C_COMPILER=D:/software/mingw64/bin/gcc.exe \
-DCMAKE_CXX_COMPILER=D:/software/mingw64/bin/g++.exe \
-DCMAKE_MAKE_PROGRAM=D:/software/mingw64/bin/make.exe \
..

`
Note: Remember to replace the path variable to your own system path.

And then, run `make` command to create the executable file.
