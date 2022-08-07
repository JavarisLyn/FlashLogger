/*
 * @Descripttion: 
 * @version: 
 * @Author: justin
 * @Date: 2022-08-06 15:58:53
 * @LastEditTime: 2022-08-07 15:05:34
 * @copyright: Copyright (c) 2022
 */
#include "LogFile.h"
#include <unistd.h> /* gethostname */
#include <thread>
#include <string>
LogFile::LogFile(){
    /* 单个文件最大256M */
    /* 如果设置的太小，可能在1s内调用日rollsize两次，但是名字却相同，就会出现错误 */
    maxFileSize = 256 * 1024 * 1024;
    baseName = "LogFile:";
    rollFile();
}

void LogFile::flush(){
    if(file!=nullptr){
        file->flush();
    }
}

void LogFile::append(const char* data, long long int length){
    if(file!=nullptr){
        file->append(data,length);
        if(file->getWrittenBytes()>=maxFileSize){
            rollFile();
        }
    }
}

void LogFile::rollFile(){
    std::string newFileName = getLogFileName(baseName);
    file = std::unique_ptr<FileWriter>(new FileWriter(newFileName));
}

std::string LogFile::getLogFileName(std::string &baseName){
    /* 日志文件名：baseName + time + host + pid + threadid*/
    std::string fileName;
    fileName += baseName;

    /* localtime */
    time_t now = 0;
    time(&now);
    struct tm tm_time;
    localtime_r(&now,&tm_time);
    char timeStr[32];
    strftime(timeStr,sizeof(timeStr),"%Y%m%d-%H%M%S",&tm_time);
    fileName += timeStr;

    fileName += "-";

    /* host */
    char hostName[256];
    if(gethostname(hostName, sizeof(hostName)) == 0) {
        hostName[sizeof(hostName) - 1] = '\0';
        fileName += hostName;
    }else{
        fileName += "UnKnownHost";
    }

    fileName += "-";

    /* pid todo*/
    fileName += std::to_string(getpid());
    
    fileName += ".log";
    return fileName;

}