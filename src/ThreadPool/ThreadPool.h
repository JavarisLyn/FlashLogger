/*
 * @version: 1.0
 * @Author: justin
 * @Date: 2022-07-30 17:37:06
 * @LastEditTime: 2022-08-01 20:48:16
 * @Descripttion: 
 * Copyright (c) 2022 by Liyangfan.justin, All Rights Reserved. 
 */

#pragma once
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include<iostream>
#include<queue>
#include<thread>
#include<functional>
#include<atomic>
#include "ConcurrentQueue.h"

class ThreadPool{
    public:
        /* using是c11新特性，代替typedef */
        /* function<void()>是一个函数函数包装器，void()表示接收的函数没有参数,返回值void */
        using Task = std::function<void()>;

        ThreadPool(size_t maxSize);

        ~ThreadPool();

        bool isRunning();

        //模板函数的实现，此种情况下需要放在头文件中
        template<typename _Callable, typename... _Args>
        void addTask(_Callable&& __f, _Args&&... __args){
            if(not this->isRunning()){
                throw std::runtime_error("Thread pool add task error: thread pool not running");
            }else{
                Task newtask = std::bind(std::forward<_Callable>(__f),std::forward<_Args>(__args)...);
                std::unique_lock<std::mutex> lck(mtx);
                if(this->threadQueue.size()<maxSize){
                    //创建线程
                    //todo 下面的操作需要保证原子性
                    
                    this->taskQueue.push(newtask);
                    std::cout<<this->taskQueue.size()<<std::endl;
                    std::cout<<"new task pushed"<<std::endl;
                    std::thread t(
                        //lambda表达式，直接推导类型可以省略返回值类型
                        [&](){  
                            std::cout<<std::this_thread::get_id()<<"thread created"<<std::endl;
                            while(true){
                                Task task;                                                     
                                {
                                    //从任务队列过去任务的逻辑要加锁
                                    std::unique_lock<std::mutex> lck(mtx);
                                    while(this->isRunning() && this->taskQueue.size()==0){
                                        cdv.wait(lck,[this](){
                                            //这里可能会发生死锁？如果触发wait返回但是锁被其他线程持有
                                            return not this->isRunning();
                                        });
                                    }
                                    if(!this->isRunning() && this->taskQueue.size()==0){
                                        std::cout<<std::this_thread::get_id()<<"thread exit"<<std::endl;
                                        return;
                                    }
                                    task = this->taskQueue.pop();
                                    std::cout<<this->taskQueue.size()<<std::endl;
                                    // if(task){
                                    //     task();
                                    // }
                                    // task();
                                    // if(task){
                                    //     this->taskQueue.pop_back();
                                    //     task();
                                    // }
                                    
                                }
                                task();
                            }
                        }
                    );
                    //这里必须是move
                    this->threadQueue.push(std::move(t));
                    std::cout<<"new thread pushed"<<std::endl;
                    this->threadRunning++;
                }else{
                    
                    //任务队列添加
                    this->taskQueue.push(newtask);
                    std::cout<<this->taskQueue.size()<<std::endl;
                    cdv.notify_one();
                }
            }
        }

        /* 后面需要实现根据不同的参数进行shutdown：立即终止所有线程、等待现有线程结束等，现在的方式是等待线程结束 */
        void shutdown();

    private:
        //线程池最大空间
        size_t maxSize;
    
        //已经创建的线程数,需要用std::atomic_int保证线程安全,这里声明为static会报错
        std::atomic<int> threadRunning;
        
        //线程池状态
        std::atomic<int> runState;

        //任务队列,这里不用queue是因为queue不好迭代遍历
        ConcurrentQueue<Task> taskQueue;

        //线程队列
        ConcurrentQueue<std::thread> threadQueue;

        //锁
        std::mutex mtx;

        //
        std::condition_variable cdv;
                
};
#endif