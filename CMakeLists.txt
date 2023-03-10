cmake_minimum_required(VERSION 3.22)
project(TargetWindowHider)

set(CMAKE_CXX_STANDARD 20)

add_executable(TargetWindowHider main_Old.cpp lib/WatchDog.h lib/Clock.h Module/Bar.cpp lib/Clock.cpp lib/WatchDog.cpp lib/JsonRW.cpp lib/JsonRW.h main.cpp lib/CheckData.cpp lib/CheckData.h Module/WindowSet.cpp)

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")