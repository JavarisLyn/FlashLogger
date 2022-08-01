/*
 * @Version: 1.0
 * @Author: LiYangfan.justin
 * @Date: 2022-07-30 23:30:14
 * @LastEditTime: 2022-08-01 20:48:52
 * @Description: 
 * Copyright (c) 2022 by Liyangfan.justin, All Rights Reserved. 
 */

#include "ThreadPool.h"
#include <thread>

using Task = std::function<void()>;

ThreadPool::ThreadPool(size_t maxSize){
    this->maxSize = maxSize;
    this->runState = 1;
    this->threadRunning = 0;
}

ThreadPool::~ThreadPool(){
    if(this->isRunning){
        this->shutdown();
    }
}

bool ThreadPool::isRunning(){
    return this->runState==1;
}

void ThreadPool::shutdown(){
    if(not this->isRunning()){
        throw std::runtime_error("Thread pool shutdown error: thread pool alread shutdown");
    }else{
        this->runState = 0;
        this->cdv.notify_all();
        while(threadQueue.size()>0){
            auto i = std::move(threadQueue.pop());
            i.join();
        }
    }  
}