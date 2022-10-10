/*
 * @Descripttion: 
 * @version: 
 * @Author: justin
 * @Date: 2022-08-06 15:53:24
 * @LastEditTime: 2022-10-09 22:36:10
 * @copyright: Copyright (c) 2022
 */
#ifndef _LOG_FILE_
#define _LOG_FILE_
#include<memory>
#include "FileWriter.h"
#include "LogConfig.h"

namespace FlashLogger
{

extern LogConfig logConfig;

class LogFile{
    public:
        LogFile(std::string baseName = logConfig.baseName,size_t rollFileSize = logConfig.rollFileSize);

        std::string getLogFileName(std::string &baseName);
        /* 一个文件写满了，创建新的文件 */
        void rollFile();
        void flush();
        void append(const char* data,long long int length);

    private:
        /* 头文件memroy */
        std::unique_ptr<FileWriter> file;
        long long int maxFileSize;
        std::string baseName;
        /* 日志文件序号，防止同一秒内rollfile导致文件名重复 */
        int fileIdx;
};

} // namespace FlashLogger

#endif