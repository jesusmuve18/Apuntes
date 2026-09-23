#!/bin/bash

cd code
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
echo "--------------------------------------------------------------------------------------------------------------------------"
./main