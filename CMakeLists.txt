cmake_minimum_required(VERSION 3.2)

project(FlashLogger VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_BUILD_TYPE "Debug")

#指定c++版本
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_COMPILER "g++")

set(CXX_FLAGS
-g
-Wall
-std=c++11
-pthread
)

find_package(Threads REQUIRED)



add_executable(FlashLogger src/ThreadPool/ThreadPool.cpp src/Logger/FileWriter.cpp src/Logger/LogFile.cpp main.cpp)

target_link_libraries(FlashLogger ${CMAKE_THREAD_LIBS_INIT})