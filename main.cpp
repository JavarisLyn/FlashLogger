/*
 * @Descripttion: 
 * @version: 
 * @Author: justin
 * @Date: 2022-07-30 16:23:13
 * @LastEditTime: 2022-08-06 16:46:13
 */
#include<iostream>
#include<thread>
#include<functional>
#include<queue>
#include<cstdio>
#include "src/ThreadPool/ThreadPool.h"
#include "src/Logger/FileWriter.h"
#include "src/Logger/LogFile.h"
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
   
    // ThreadPool* tp = new ThreadPool(5);
   

    // for(int i = 0;i<10;i++){
    //     std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    //     tp->addTask(func1,i);
    // }
    // tp->shutdown();
    // const char* name = "test.txt";
    // FILE* fp_ = fopen(name, "w");
    // if (fp_ == nullptr) {
    //     // perror("NormalFileWriter open file failed: ");
    //     // int err = ferror(fp_);
    //     // fprintf(stderr, "NormalFileWriter open file: %s failed, errno: %s\n",
    //     //         name, strerror(err));
    //     abort();
    // }
    // char buffer_[5];
    // setbuffer(fp_, buffer_, sizeof(buffer_));
    // const char *_msg = "111111\n111111111111111111111111111111111111111111";
    // int written = 0;
    // int remain = 4;
    // size_t n = fwrite_unlocked(_msg + written, 1, remain, fp_);
    // // std::cout<<n;
    // while(true){
        
    // }
    LogFile lgf;
    for(int i=0;i<100000;i++){
        lgf.append("12345678901234567890",11);
    }
    return 0;
}