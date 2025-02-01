#!/bin/bash
BUILD_DIR="build"

ls $BUILD_DIR || mkdir $BUILD_DIR
cd $BUILD_DIR

cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . -j6