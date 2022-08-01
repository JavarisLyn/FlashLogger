/*
 * @Descripttion: 
 * @version: 
 * @Author: justin
 * @Date: 2022-07-30 16:23:13
 * @LastEditTime: 2022-08-01 19:07:35
 */
#include<iostream>
#include<thread>
#include<functional>
#include<queue>
#include "src/ThreadPool/ThreadPool.h"

void printTime(){
    for(int i=0;i<10;i++){
        // auto t = std::chrono::high_resolution_clock::now();
        //这里记得加上endl，不然等到缓冲区满才输出
        std::cout<<i<<std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}


// template<typename Callable>
// void print(Callable callable){
//     callable(1);
// }

void func1(int i){
    std::cout<<"i am task "<<i<<" start"<<std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    std::cout<<"i am task "<<i<<" end"<<std::endl;
}

int main(){
    //
    // ThreadPool tp((size_t)10);
    ThreadPool* tp = new ThreadPool(5);
    // for(int i = 0;i<15;i++){
    //     std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    //     tp.addTask([](){
    //         std::cout<<"i am task "<<i<<" start"<<std::endl;
    //         std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    //         std::cout<<"i am task "<<i<<" end"<<std::endl;
    //     },i);
    // }



    for(int i = 0;i<10;i++){
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        tp->addTask(func1,i);
    }
    tp->shutdown();
    return 0;
}