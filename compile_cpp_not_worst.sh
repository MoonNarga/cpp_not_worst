#!/bin/sh
cd ./src
rm -rf build
mkdir build
cd build
cmake ../
make