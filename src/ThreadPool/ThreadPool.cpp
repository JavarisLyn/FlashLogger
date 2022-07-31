/*
 * @Version: 1.0
 * @Author: LiYangfan.justin
 * @Date: 2022-07-30 23:30:14
 * @LastEditTime: 2022-07-31 16:19:36
 * @Description: 
 * Copyright (c) 2022 by Liyangfan.justin, All Rights Reserved. 
 */

#include "ThreadPool.h"
#include <thread>

using Task = std::function<void()>;

ThreadPool::ThreadPool(size_t maxSize){
    this->maxSize = maxSize;
    this->isRunning = true;
    this->threadRunning = 0;
}

void ThreadPool::addTask(Task newtask){
    if(not this->isRunning){
        throw std::runtime_error("Thread pool add task error: thread pool not running");
    }else{
        if(this->threadQueue.size()<maxSize){
            //创建线程
            //todo 下面的操作需要保证原子性
            this->taskQueue.push_back(newtask);
            std::thread t(
                //lambda表达式，直接推导类型可以省略返回值类型
                [&](){
                    while(this->isRunning){
                        Task task = this->taskQueue.back();

                        if(task){
                            this->taskQueue.pop_back();
                            task();
                        }
                    }
                }
            );
            //这里必须是move
            this->threadQueue.emplace_back(std::move(t));
            this->threadRunning++;
        }else{
            //任务队列添加
            this->taskQueue.emplace_back(newtask);
        }
    }
}

void ThreadPool::shutdown(){
    if(not this->isRunning){
        throw std::runtime_error("Thread pool shutdown error: thread pool alread shutdown");
    }else{
        this->isRunning = false;
        for(auto& i:threadQueue){
            i.join();
        }
    }  
}