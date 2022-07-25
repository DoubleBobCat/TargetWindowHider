cmake_minimum_required(VERSION 3.22)
project(TargetWindowHider)

set(CMAKE_CXX_STANDARD 14)

add_executable(TargetWindowHider main.cpp)

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")