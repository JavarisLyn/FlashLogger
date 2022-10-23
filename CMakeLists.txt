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
-c
)

find_package(Threads REQUIRED)

add_subdirectory(src/ThreadPool)
add_subdirectory(src/Logger) 
add_subdirectory(src/Utils)

# SET(LOGGER_SRC src/Logger/LogConfig.h src/Logger/Buffer.h src/Utils/tscns.h src/ThreadPool/ThreadPool.cpp src/Logger/FileWriter.cpp src/Logger/LogFile.cpp src/Logger/AsynLogger.cpp src/Logger/Logger.cpp src/Benchmark/AsynLoggerBenchmark.cpp)
# ADD_LIBRARY(FlashLogger STATIC ${LOGGER_SRC})
# SET_TARGET_PROPERTIES(FlashLogger PROPERTIES  OUTPUT_NAME "FlashLogger")

# file(GLOB HEADERS "*.h")
# file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

# SET_TARGET_PROPERTIES(FlashLogger PROPERTIES CLEAN_DIRECT_OUTPUT 1)
# INSTALL(FILES ${HEADERS} DESTINATION include/lyf)
# INSTALL(FILES src/Logger/LogConfig.h src/Logger/Buffer.h src/Utils/tscns.h src/Logger/Logger.h DESTINATION include/lyf)
# INSTALL(TARGETS FlashLogger FlashLogger LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)

# add_executable(FlashLogger src/ThreadPool/ThreadPool.cpp src/Logger/FileWriter.cpp src/Logger/LogFile.cpp src/Logger/AsynLogger.cpp src/Logger/Logger.cpp src/Benchmark/AsynLoggerBenchmark.cpp)
# add_executable(FlashLogger src/test.cpp)
include_directories(${PROJECT_SOURCE_DIR})
# target_link_libraries(FlashLogger ${CMAKE_THREAD_LIBS_INIT})

# redefine_file_macro(FlashLogger)