/*
 * @Descripttion: 通用的缓冲区，提供了一系列相关操作
 * @version: 
 * @Author: justin
 * @Date: 2022-08-06 18:31:52
 * @LastEditTime: 2022-08-10 00:20:46
 * @copyright: Copyright (c) 2022
 */
#ifndef _BUFFERE_NODE_
#define _BUFFERE_NODE_
#include <memory>
#include <cstring>
#include <iostream>


/* 单行日志缓冲区，用于处理原始日志的场所，做参数替换、加时间这些操作 */
const size_t LineLogBuffer = 1 * 1024;

/* 环形链表中一个缓冲区的大小 16MB */
const size_t NodeBufferSize = 16 * 1024 * 1024;

template <size_t size>
class Buffer{
    public:
        Buffer():length(0){};

        ~Buffer() = default;

        size_t getLength() const{return length;}

        /* 这里不能返回const */
        char* getCurrent(){
            return data + length;
        }

        void addLen(size_t len){
            length += len;
            return;
        }

        const char* getData() const{
            return data;
        }

        size_t getAvaliable() const{
            // std::cout<<"getAvaliable"<<std::endl;
            return size - length;
        }

        void append(const char* appendData,size_t appendLen){
            if(getAvaliable()>appendLen){
                memcpy(data+length,appendData,appendLen);
                length += appendLen;
            }else{
                /* todo */
            }
        }

        void clear(){
            length = 0;
            /* 还需要memcpy清零吗？ */
        }
        
    private:
        char data[size];
        size_t length;
};



#endif