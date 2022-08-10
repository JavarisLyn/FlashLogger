/*
 * @Version: 
 * @Author: LiYangfan.justin
 * @Date: 2022-08-06 20:48:48
 * @LastEditTime: 2022-08-10 01:05:07
 * @Description: 
 * Copyright (c) 2022 by Liyangfan.justin, All Rights Reserved. 
 */
#ifndef _ASYNLOGGER_
#define _ASYNLOGGER_
#include "Buffer.h"
#include "LogFile.h"
#include "LogConfig.h"
#include <vector>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <thread>
#include <memory>
#include <assert.h>

extern LogConfig logConfig;

class AsynLogger{
    public:
        // AsynLogger();
        /* 这种指定值得话就不能重载构造函数了 */
        AsynLogger(int bufferNodeListSize = logConfig.initBufferNodeListSize,
        int flushInterval = logConfig.flushInterval);
        ~AsynLogger();

        /* 前端调用该接口写日志 */
        void append(const char * data,size_t len);

        /* 启动后台线程写日志 */
        void start();

        /* 停止后台线程 */
        void stop();

    private:

        /* 后台写日志线程 */
        void backgroundFunc();

        using LogBuffer = Buffer<NodeBufferSize>;

        /* 环形链表结点 */
        struct BufferNode {
            std::unique_ptr<LogBuffer> buffer;
            std::shared_ptr<BufferNode> next,prev;//因为是双向链表，小心循环引用问题

            BufferNode() : buffer(nullptr),next(nullptr),prev(nullptr){}
            BufferNode(LogBuffer* data): buffer(data),next(nullptr),prev(nullptr){}
        };
    
        std::shared_ptr<BufferNode> head,tail;
        std::shared_ptr<BufferNode> cur;

        std::shared_ptr<BufferNode> newBufferNode();

        void removeHead();
        void addTail(std::shared_ptr<BufferNode> bufferNodePtr);
        
        std::vector<std::shared_ptr<BufferNode>> toWriteBufferNodeList;

        int bufferNodeListSize;

        /* 后台线程 */
        std::thread backgroundThread;

        /* 负责刷盘 */
        LogFile logFileWriter;

        /* 刷盘间隔 */
        int flushInterval;

        /* 锁相关 */
        std::mutex mtx;
        std::condition_variable cv;
        /* 真的需要这个吗？初始化为什么要放到后台线程而不是start */
        // CountDownLatch cdlatch;

        std::atomic_bool running;

        //用于存储日志时间，后面会移动到logging模块
        char timeStr[32];
};

#endif