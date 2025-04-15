cmake_minimum_required(VERSION 3.10)
project(java-enet-jni)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find JNI
find_package(JNI REQUIRED)
include_directories(${JNI_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/enet/include)
include_directories(${CMAKE_SOURCE_DIR}/build/generated/jni)

# Add ENet as a subdirectory
add_subdirectory(enet)

# Add our JNI library
file(GLOB SOURCES "src/main/cpp/*.cpp")
add_library(enetjni SHARED ${SOURCES})
target_link_libraries(enetjni enet ${JNI_LIBRARIES})

if(WIN32)
    target_link_libraries(enetjni ws2_32 winmm)
endif() 