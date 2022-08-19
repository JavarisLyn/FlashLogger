/*
 * @Descripttion: 
 * @version: 
 * @Author: justin
 * @Date: 2022-08-08 00:55:43
 * @LastEditTime: 2022-08-12 11:37:10
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
#include "../Utils/tscns.h"

/* 编译期接收字符数组并求长度 */
// class str_util{

// private:
//     const char* t;
//     size_t length;
// public:
//     template<size_t LEN>
//     constexpr str_util(const char(&a)[LEN]){
//         t = a;
//         length = LEN;
//     }
// }

/* 编译期将变量转为字符串 */
//这一步将__LINE__转为了对应的值，第一个x是__LINE__，第二个x是值
#define strify(x) val_(x)
//这一步给值将上了双引号
#define val_(x) #x

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
    void append(const char* data,LogLevel loglevel,const char * File,const char * Line, ...);


private:
    /* 如果定义了带参构造函数，系统就不会自动生成无参构造了，加default就是要求生成无参构造 */
    /* https://blog.csdn.net/weixin_42108533/article/details/125953755 */
    Logger() = default;
    ~Logger() = default;

    /* 指针可以实现多态，直接用对象不行 */
    /* 对象用的是内存栈，指针用的是内存堆 */
    /* https://blog.csdn.net/u011754972/article/details/118756049 */
    static Logger * logger;

    static TSCNS tscns;

    /* 获取单例对象时加锁 */
    static std::mutex mtx;

};
/* 宏定义函数 */
/* do...while(0)的写法是为了保证健壮性 */
/* FILE和LINE可以放在编译器就得到长度这些东西 https://blog.csdn.net/u011718663/article/details/118163962 */
/* https://www.thinbug.com/q/52852503 */
/* 使用模板参数在编译器获得数组长度 */
/* 数组指针 https://blog.csdn.net/weixin_41938578/article/details/107443454?spm=1001.2101.3001.6650.2&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-2-107443454-blog-117064724.pc_relevant_default&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-2-107443454-blog-117064724.pc_relevant_default&utm_relevant_index=3 */
#define LOG_TRACE(data, args...)                                     \
    do{                                                             \
        Logger::getInstance()->append(data, Logger::TRACE,__FILE__,strify(__LINE__), args);     \
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