#!/bin/sh

mkdir -p build; cd build && cmake -DCMAKE_BUILD_TYPE=$1 .. && cmake --build . -- -j 4
