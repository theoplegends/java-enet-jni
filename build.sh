#!/bin/bash
mkdir -p build/native
cd build/native

cmake -DCMAKE_BUILD_TYPE=Release ../..
cmake --build .

cd ../..
cp build/native/libenetjni.* build/native/ 