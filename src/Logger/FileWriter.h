/*
 * @Descripttion: 
 * @version: 
 * @Author: justin
 * @Date: 2022-08-06 14:55:29
 * @LastEditTime: 2022-08-07 14:14:15
 * @copyright: Copyright (c) 2022
 */
#ifndef _FILE_WRITER_
#define _FILE_WRITER_

#include<cstdio>
#include<string>
class FileWriter{
    public:
        /* 禁止单参构造函数自动类型转换 */
        explicit FileWriter(std::string name);

        /* 需要时virtual */
        virtual ~FileWriter();

        void append(const char* data,long long int length);

        void flush();

        long long int getWrittenBytes() const;


    private:
        FILE* file;
        /* 缓冲区大小 4k，即写满4k就会自动刷盘*/
        char buffer[4*1024];

        long long int writtenBytes;
};


#endif