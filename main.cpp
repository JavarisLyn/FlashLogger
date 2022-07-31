/*
 * @Descripttion: 
 * @version: 
 * @Author: justin
 * @Date: 2022-07-30 16:23:13
 * @LastEditTime: 2022-07-31 16:27:12
 */
#include<iostream>
#include<thread>
#include<functional>
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

int func1(int a){
    std::cout<<a<<std::endl;
    return a;
}

int main(){
    ThreadPool tp((size_t)10);
    for(int i = 0;i<15;i++){
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        tp.addTask([&]{
            std::this_thread::sleep_for(std::chrono::milliseconds(10000));
            std::cout<<"i am task "<<i<<std::endl;
        });
    }
    tp.shutdown();
    return 0;
}