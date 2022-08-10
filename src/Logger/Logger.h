/*
 * @Descripttion: 
 * @version: 
 * @Author: justin
 * @Date: 2022-08-08 00:55:43
 * @LastEditTime: 2022-08-10 14:38:55
 * @copyright: Copyright (c) 2022
 */
/*
 * @Version: 
 * @Author: LiYangfan.justin
 * @Date: 2022-08-08 00:55:43
 * @LastEditTime: 2022-08-09 17:10:27
 * @Description: 该文件的方法被用户直接调用，下层是AsnyLogger等
 *               AsynLogger只提供了基本的append数据功能,Logger
 *               要提供添加日志级别信息、文件名、行号、日期、变量替换等功能
 * Copyright (c) 2022 by Liyangfan.justin, All Rights Reserved. 
 */
#ifndef _LOGGER_
#define _LOGGER_

#include <functional>
#include "LogConfig.h"
#include "Buffer.h"
#include <mutex>


class Logger{

public:

    /* 日志级别 */
    enum LogLevel{
        TRACE = 0,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL,
        levelNum,
    };

    /* Logger的输出函数，如果是异步日志的话就是asynLogger.append(),也可以说stdout */
    using OutPutFunc = std::function<void(const char*,size_t)>;

    using LineBuffer = Buffer<LineLogBuffer>;

    /* 这里static是因为设置的时候还没有初始化Logger实例,具体打日志的时候才会初始化 */
    static void setOutPutFunc(OutPutFunc);



    static LogLevel getLogLevel();

    static void setLogLevel();

    /* 获取单例对象,必须是单例,不然不能打一次日志就生成一个对象,同时用户无需手动初始化,所以必须是静态单例 */
    static Logger* getInstance(); 

    /* Logger相关参数的设置 */
    static void setConfig(const LogConfig &);

    /* 单条日志的加工处理并输出 */
    void append(const char* data,LogLevel loglevel, ...);


private:
    /* 如果定义了带参构造函数，系统就不会自动生成无参构造了，加default就是要求生成无参构造 */
    /* https://blog.csdn.net/weixin_42108533/article/details/125953755 */
    Logger() = default;
    ~Logger() = default;

    /* 指针可以实现多态，直接用对象不行 */
    /* 对象用的是内存栈，指针用的是内存堆 */
    /* https://blog.csdn.net/u011754972/article/details/118756049 */
    static Logger * logger;

    /* 获取单例对象时加锁 */
    static std::mutex mtx;

    /* 这个变量每个线程必须是互不影响的，所以用thread_local */
    // thread_local LineBuffer lineBuffer;



};
/* 宏定义函数 */
/* do...while(0)的写法是为了保证健壮性 https://www.bilibili.com/read/cv9431572/ */
/* 主要要加有右边的反斜线，宏函数需要写在一行 */
#define LOG_TRACE(data, args...)                                     \
    do{                                                             \
        Logger::getInstance()->append(data, Logger::TRACE, args);     \
    }while(0)    
#define LOG_DEBUG(data, args...)                                     \
    do{                                                             \
        Logger::getInstance()->append(data, Logger::DEBUG, args);     \
    }while(0)  
#define LOG_INFO(data, args...)                                     \
    do{                                                             \
        Logger::getInstance()->append(data, Logger::INFO, args);     \
    }while(0)  
#define LOG_WARN(data, args...)                                     \
    do{                                                             \
        Logger::getInstance()->append(data, Logger::WARN, args);     \
    }while(0)  
#define LOG_ERROR(data, args...)                                     \
    do{                                                             \
        Logger::getInstance()->append(data, Logger::ERROR, args);     \
    }while(0)                                                     
#define LOG_FATAL(data, args...)                                     \
    do{                                                             \
        Logger::getInstance()->append(data, Logger::FATAL, args);     \
    }while(0)  

#endif