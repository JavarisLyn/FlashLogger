SET(LOGGER_SRC 
    src/Logger/LogConfig.h 
    src/Logger/Buffer.h 
    src/Logger/FileWriter.cpp 
    src/Logger/LogFile.cpp 
    src/Logger/AsynLogger.cpp 
    src/Logger/Logger.cpp
    )
    
ADD_LIBRARY(FlashLogger STATIC ${LOGGER_SRC})