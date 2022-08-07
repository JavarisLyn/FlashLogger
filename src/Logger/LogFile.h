/*
 * @Descripttion: 
 * @version: 
 * @Author: justin
 * @Date: 2022-08-06 15:53:24
 * @LastEditTime: 2022-08-07 18:07:39
 * @copyright: Copyright (c) 2022
 */
#ifndef _LOG_FILE_
#define _LOG_FILE_
#include<memory>
#include "FileWriter.h"

class LogFile{
    public:
        LogFile();

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

#endif