/*
 * @Descripttion: 
 * @version: 
 * @Author: justin
 * @Date: 2022-08-05 15:50:54
 * @LastEditTime: 2022-08-06 16:44:43
 * @copyright: Copyright (c) 2022
 */

#include<cstdio>
#include "FileWriter.h"
#include<cstdio>

FileWriter::FileWriter(std::string &name){
    file = fopen(name.c_str(), "w");
    if(file==nullptr){
        perror("file open failed");
    }
    setbuffer(file,buffer,sizeof(buffer));
}

FileWriter::~FileWriter(){
    flush();
    if(file!=nullptr){
        fclose(file);
    }
}

void FileWriter::append(const char* data,long long int length){
    int unwritten = 0;
    int written = 0;
    while(written!=length){
        unwritten = length - written;
        long long int n = fwrite_unlocked(data + written, 1, unwritten, file);
        if(n!=unwritten){
            /* 错误处理后面完善一下 */
            perror("fwrite_unlocked failed");
        }
        written += n;
    }
    writtenBytes += written;
}

void FileWriter::flush(){
    fflush(file);
}

long long int FileWriter::getWrittenBytes() const{
    return writtenBytes;
}

