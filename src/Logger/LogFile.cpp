/*
 * @Descripttion: 
 * @version: 
 * @Author: justin
 * @Date: 2022-08-06 15:58:53
 * @LastEditTime: 2022-10-02 17:24:23
 * @copyright: Copyright (c) 2022
 */
#include "LogFile.h"
#include <unistd.h> /* gethostname */
#include <thread>
#include <string>
#include <iostream>

using namespace FlashLogger;

LogFile::LogFile(std::string baseName,size_t rollFileSize){
    /* 单个文件最大256M */
    // maxFileSize = 256 * 1024 * 1024;
    // baseName = "LogFile:";
    this->maxFileSize = rollFileSize;
    this->baseName = baseName;
    fileIdx = 0;
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
            // std::cout<<"rollFile:"<<file->getWrittenBytes()<<std::endl;
            rollFile();
        }
    }
}

void LogFile::rollFile(){
    if(file!=nullptr){
        std::cout<<file->getWrittenBytes()<<"rollFile"<<std::endl;
    }else{
        std::cout<<"null"<<std::endl;
    }
    
    std::string newFileName = getLogFileName(baseName);
    file = std::unique_ptr<FileWriter>(new FileWriter(newFileName));
    fileIdx++;
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

    /* 文件序号，防止文件名重复 */
    fileName += std::to_string(fileIdx);
    
    fileName += ".log";
    return fileName;

}