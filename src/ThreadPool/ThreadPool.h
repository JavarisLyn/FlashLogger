/*
 * @version: 1.0
 * @Author: justin
 * @Date: 2022-07-30 17:37:06
 * @LastEditTime: 2022-07-31 16:18:39
 * @Descripttion: 
 * Copyright (c) 2022 by Liyangfan.justin, All Rights Reserved. 
 */

#pragma once
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include<queue>
#include<thread>
#include<functional>
#include<atomic>

class ThreadPool{
    public:
        /* using是c11新特性，代替typedef */
        /* function<void()>是一个函数函数包装器，void()表示接收的函数没有参数,返回值void */
        using Task = std::function<void()>;

        ThreadPool(size_t maxSize);

        void addTask(Task Task);

        /* 后面需要实现根据不同的参数进行shutdown：立即终止所有线程、等待现有线程结束等，现在的方式是等待线程结束 */
        void shutdown();

    private:
        //线程池最大空间
        size_t maxSize;
    
        //已经创建的线程数,需要用std::atomic_int保证线程安全,这里声明为static会报错
        std::atomic<int> threadRunning;
        
        //线程池状态
        bool isRunning;

        //任务队列,这里不用queue是因为queue不好迭代遍历
        std::vector<Task> taskQueue;

        //线程队列
        std::vector<std::thread> threadQueue;
                
};
#endif