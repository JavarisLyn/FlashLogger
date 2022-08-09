/*
 * @Version: 
 * @Author: LiYangfan.justin
 * @Date: 2022-08-09 17:10:57
 * @LastEditTime: 2022-08-10 00:35:13
 * @Description: 
 * Copyright (c) 2022 by Liyangfan.justin, All Rights Reserved. 
 */
#include "Logger.h"
#include "stdio.h"
#include "stdarg.h"
/* 静态变量要在这里再声明一下？ */
/* 或者不在h文件声明，直接在这里声明 */
Logger * Logger::logger = nullptr;
std::mutex Logger::mtx;
/* 被thread_local修饰后的变量，从属于访问它的线程，线程第一次访问它时创建它且只创建一次
（与被static的修饰的变量是一样的，多实例共享一份），线程结束时系统释放该变量。 */
/* 所以类似于静态变量，也要在这里声明 */
thread_local Logger::LineBuffer lineBuffer;

Logger* Logger::getInstance(){
    /* 单例 双检锁懒汉*/
    if(logger == nullptr){
        std::unique_lock<std::mutex> lock(mtx);
        if(logger == nullptr){
            return new Logger;
        }
         /* RAII 自动释放锁 */
    }
    return logger;  
} 

inline void defaultOutPutFunc(const char* data,size_t length){
        
    size_t n = fwrite(data,1,length,stdout);
    /* 可以忽略编译器的未使用定义变量warn */
    (void)n;
}

Logger::OutPutFunc global_outPutFunc = defaultOutPutFunc;

    /* static只需要在头文件声明，不能再在cpp声明 */
void Logger::setOutPutFunc(Logger::OutPutFunc outPutFunc){
    /* 这里为什么要用右值引用来着?outputfun不是函数指针吗 */
    global_outPutFunc = std::move(outPutFunc);
}


void Logger::append(const char* data,LogLevel loglevel, ...){
    /* 参数替换 */
    /* va_list指针，接收可变参数列表 */
    va_list args;
    /* va_list执行data后面的参数开始 */
    va_start(args,data);
    /* 替换参数并写入缓冲区 */
    int n = vsnprintf(lineBuffer.getCurrent(),lineBuffer.getAvaliable(),data,args);
    lineBuffer.addLen(static_cast<size_t>(n));
    /* 释放指针 */
    va_end(args);
    if(global_outPutFunc!=nullptr){
        global_outPutFunc(lineBuffer.getData(),lineBuffer.getLength());
    }
    lineBuffer.clear();
}