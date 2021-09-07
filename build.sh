#!/bin/bash

set -e
set -x

rm -rf build
mkdir build
pushd build

conan install .. --build=missing #--profile:host ../config/win_to_rpi --profile:build ../config/default_build_conf
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_BUILD_PARALLEL_LEVEL=4 #-DCMAKE_TOOLCHAIN_FILE=../config/toolchain-arm.cmake
cmake --build . -j

cp ./bin/caravel ../
