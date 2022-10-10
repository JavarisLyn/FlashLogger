include (../../utils.cmake)

SET(LOGGER_SRC 
    LogConfig.h 
    Buffer.h 
    FileWriter.cpp 
    LogFile.cpp 
    AsynLogger.cpp 
    Logger.cpp
    )

ADD_LIBRARY(Logger STATIC ${LOGGER_SRC})

file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
INSTALL(FILES ${HEADERS} DESTINATION include/lyf/Logger)
INSTALL(TARGETS Logger Logger LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)

SET_TARGET_PROPERTIES(Logger PROPERTIES  OUTPUT_NAME "Logger")
SET_TARGET_PROPERTIES(Logger PROPERTIES CLEAN_DIRECT_OUTPUT 1)

target_link_libraries(Logger ${CMAKE_THREAD_LIBS_INIT})
find_package(Threads REQUIRED)

redefine_file_macro(Logger)

add_executable(FlashLogger ${LOGGER_SRC} ../Benchmark/AsynLoggerBenchmark.cpp)
target_link_libraries(FlashLogger ${CMAKE_THREAD_LIBS_INIT})