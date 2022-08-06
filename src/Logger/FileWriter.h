/*
 * @Descripttion: 
 * @version: 
 * @Author: justin
 * @Date: 2022-08-06 14:55:29
 * @LastEditTime: 2022-08-06 15:34:32
 * @copyright: Copyright (c) 2022
 */
#ifndef _FILE_WRITER_
#define _FILE_WRITER_

#include<cstdio>
class FileWriter{
    public:
        /* 禁止单参构造函数自动类型转换 */
        explicit FileWriter(const char * name);

        /* 需要时virtual */
        virtual ~FileWriter();

        void append(const char* data,int length);

        void flush();


    private:
        FILE* file;
        /* 缓冲区4k */
        char buffer[4*1024];
};


#endif