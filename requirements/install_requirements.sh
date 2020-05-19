#!/bin/sh

# install JSONCPP
git clone https://github.com/open-source-parsers/jsoncpp.git
cd jsoncpp
git checkout 1.8.4
cd ../

# update CMake with new install location
patch ./jsoncpp/CMakeLists.txt < cmake.patch

# make build directory 
cd jsoncpp
mkdir -p build/release && cd build/release

# generate make files
cmake ../../

# install library 
make
make install