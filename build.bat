@echo off
if not exist "build\native" mkdir build\native
cd build\native

cmake -DCMAKE_BUILD_TYPE=Release -A x64 ..\..
cmake --build . --config Release

cd ..\..
copy build\native\Release\enetjni.dll build\native\ 