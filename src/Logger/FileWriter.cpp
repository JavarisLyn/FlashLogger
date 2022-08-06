/*
 * @Descripttion: 
 * @version: 
 * @Author: justin
 * @Date: 2022-08-05 15:50:54
 * @LastEditTime: 2022-08-06 15:32:19
 * @copyright: Copyright (c) 2022
 */

#include<cstdio>
#include "FileWriter.h"
#include<string>
#include<cstdio>

FileWriter::FileWriter(const char * name){
    file = fopen(name, "w");
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

void FileWriter::append(const char* data,int length){
    int unwritten = 0;
    int written = 0;
    while(written!=length){
        unwritten = length - written;
        long long int n = fwrite_unlocked(data + written, 1, unwritten, file);
        if(n!=unwritten){
            perror("fwrite_unlocked failed");
        }
        written += n;
    }
}

void FileWriter::flush(){
    fflush(file);
}

