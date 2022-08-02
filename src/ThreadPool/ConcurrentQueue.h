/*
 * @Version: 
 * @Author: LiYangfan.justin
 * @Date: 2022-08-01 13:35:14
 * @LastEditTime: 2022-08-02 10:15:48
 * @Description: A concurrent Queue implemented with mutex
 * Copyright (c) 2022 by Liyangfan.justin, All Rights Reserved. 
 */
#ifndef _CONCURRENT_QUEUE_
#define _CONCURRENT_QUEUE_

#include<iostream>
#include<queue>
#include<mutex>
#include<condition_variable>

template<typename T>
class ConcurrentQueue{
    private:
        std::queue<T> crt_queue;
        std::mutex mtx;
        std::condition_variable cdv;

    public:
        T pop(){
            //加锁
            std::unique_lock<std::mutex> lck(mtx);
            //wait:阻塞等待的同时释放锁,解除阻塞后，再次获得锁;这里犹豫判空后是直接null返回还是阻塞
            // cdv.wait(lck,[this](){
            //     return !crt_queue.empty();
            // });
        
            //记得用move，否则T为thread时报错,thread的拷贝构造函数delete
            //pop不调用析构，不释放内存
            T t = std::move(crt_queue.front());
            crt_queue.pop();

            return t;
           
        }

        void push(T t){
            std::unique_lock<std::mutex> lck(mtx);
            crt_queue.emplace(std::move(t));
            // cdv.notify_one();
        }

        size_t size(){
            std::unique_lock<std::mutex> lck(mtx);
            return crt_queue.size();
        }
};
#endif