/*
 * @Descripttion: 
 * @version: 
 * @Author: justin
 * @Date: 2022-08-07 14:14:57
 * @LastEditTime: 2022-08-07 16:14:24
 * @copyright: Copyright (c) 2022
 */
#include <iostream>
#include <vector>
#include <thread>
#include "../Logger/AsynLogger.h"
using namespace std;
using LL = long long;
AsynLogger asynLogger(3,3);
void func(){
    for(int i = 0;i<20000000;i++){
        // const char* a = "This is AsynLogger by LiYangfan.justin\n";
        asynLogger.append("This is AsynLogger by LiYangfan.justin\n",39);
    }
}

int main(){
    asynLogger.start();
    vector<thread> vec(1);
    auto start = chrono::high_resolution_clock::now();
    for(auto& t: vec){
        t = thread(func);
        t.join();
    }
    auto end = chrono::high_resolution_clock::now();
    LL duration = (end - start).count();
    cout<<"耗时"<<duration<<endl;
    // for(int i=0;i<1000;i++){
    //     /* 后面考虑改成不需要自己加\n */
    //     asynLogger.append("This is AsynLogger by LiYangfan.justin\n",5);
    // }
    
    // asynLogger.stop();
}