/*
 * @Version: 
 * @Author: LiYangfan.justin
 * @Date: 2022-08-09 17:10:57
 * @LastEditTime: 2022-08-19 17:02:12
 * @Description: 
 * Copyright (c) 2022 by Liyangfan.justin, All Rights Reserved. 
 */
#include "Logger.h"
#include "stdio.h"
#include "stdarg.h"
#include <sys/syscall.h>
#include <unistd.h>
#define gettid() syscall(SYS_gettid)

/* 全局变量 */
LogConfig logConfig;

/* 静态变量要在这里再声明一下？ */
/* 或者不在h文件声明，直接在这里声明 */
Logger * Logger::logger = nullptr;
TSCNS Logger::tscns;
std::mutex Logger::mtx;
/* 被thread_local修饰后的变量，从属于访问它的线程，线程第一次访问它时创建它且只创建一次
（与被static的修饰的变量是一样的，多实例共享一份），线程结束时系统释放该变量。 */
/* 所以类似于静态变量，也要在这里声明 */
thread_local Logger::LineBuffer lineBuffer;
thread_local int currentTid;
thread_local int64_t preSecond;

/* 字节对齐？ */
thread_local char timeStr[64];

const char* loggerLevels[Logger::levelNum] = {"TRACE","DEGUB","INFO","WARN","ERROR","FATAL"};

Logger* Logger::getInstance(){
    /* 单例 双检锁懒汉*/
    if(logger == nullptr){
        std::unique_lock<std::mutex> lock(mtx);
        if(logger == nullptr){
            logger =  new Logger();
            tscns.init();
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

void Logger::setConfig(const LogConfig& logConfig_){
    logConfig = logConfig_;
}


void Logger::append(const char* data,LogLevel loglevel,const char * File,const char* Line, ...){
    /* 添加日志时间 */
    
    int64_t currentSecond = tscns.rdns()/1000000000;
    if(currentSecond!=preSecond){
        preSecond = currentSecond;
        struct tm tm_time;
        localtime_r(&preSecond,&tm_time);
        snprintf(timeStr, sizeof(timeStr), "%4d-%02d-%02d %02d:%02d:%02d ",
             tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
             tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
    }
    lineBuffer.append(timeStr,20);

    // time_t curSecond = 0;
    // time(&curSecond);
    // struct tm tm_time;
    // localtime_r(&curSecond,&tm_time);
    // /* strftime性能不如这个 %02d会补0到2位*/
    // snprintf(timeStr,sizeof(timeStr),"%4d-%02d-%02d %02d:%02d:%02d ",tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
    //          tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
    // lineBuffer.append(timeStr,20);

    /* 添加线程号 用tls存起来*/
    lineBuffer.append("tid:",4);
    if(currentTid == 0){
        /* 系统调用，需要define */
        currentTid = gettid();
    }
    int n = snprintf(lineBuffer.getCurrent(), lineBuffer.getAvaliable(), "%d ", currentTid);
    lineBuffer.addLen(static_cast<size_t>(n));

    /* 添加所在的文件名和行数 */
    while(*File!='\0'){
        lineBuffer.append(File,1);
        File++;
    }
    lineBuffer.append(" ",1);
    lineBuffer.append("line:",5);
    while(*Line!='\0'){
        lineBuffer.append(Line,1);
        Line++;
    }
    lineBuffer.append(" ",1);


    /* 添加日志级别 */
    lineBuffer.append(loggerLevels[loglevel],5);
    lineBuffer.append(": ",2);

    /* 参数替换 */
    /* va_list指针，接收可变参数列表 */
    va_list args;
    /* va_list执行Line后面的参数开始 */
    va_start(args,data);
    /* 替换参数并写入缓冲区 */
    n = vsnprintf(lineBuffer.getCurrent(),lineBuffer.getAvaliable(),data,args);
    lineBuffer.addLen(static_cast<size_t>(n));
    /* 释放指针 */
    va_end(args);

    
    if(global_outPutFunc!=nullptr){
        global_outPutFunc(lineBuffer.getData(),lineBuffer.getLength());
    }
    lineBuffer.clear();
}