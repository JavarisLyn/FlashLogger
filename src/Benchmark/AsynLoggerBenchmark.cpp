/*
 * @Descripttion: 
 * @version: 
 * @Author: justin
 * @Date: 2022-08-07 14:14:57
 * @LastEditTime: 2022-08-13 14:55:27
 * @copyright: Copyright (c) 2022
 */
#include <iostream>
#include <vector>
#include <thread>
#include <cstring>
#include <memory>
#include "../Logger/AsynLogger.h"
#include "../Logger/Logger.h"
#include "../Logger/LogConfig.h"
#include "stdio.h"
using namespace std;
using LL = long long;
// AsynLogger asynLogger(3,3);
void func(){
    for(int i = 0;i<1000000;i++){
        // const char* a = "AsynLoggerAsynLoggerAsynLoggerAsynLoggerAsynLoggerAsynLoggerAsynLoggerAsynLoggerAsynLoggerAsynLoggerAsynLoggerAsynLoggerAsynLoggerAsynLoggerAsynLoggerAsynLoggerAsynLoggerAsynLoggerAsynLoggerAsynLogge\n";
        //  asynLogger.append(a,100);
        //  LOG_TRACE("This is AsynLogger by %s\n","liyangfan");
        LOG_TRACE("AsynLoggerAsynLoggerAsynLoggerAsynLoggerAsynLoggerAsynLoggerAsynLoggerAsynLoggerAsynLoggerAsynLoggerAsynLoggerAsynLoggerAsynLoggerAsynLoggerAsynLoggerAsynLoggerAsynLoggerAsynLoggerAsynLogge%s\n","AsynLogger");
        // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

std::unique_ptr<AsynLogger> asynLogger;

void output(const char* data, size_t len){
    asynLogger->append(data,len);
}

int main(){
    LogConfig logConfig;
    logConfig.flushInterval = 10;
    logConfig.initBufferNodeListSize = 3;
    logConfig.rollFileSize = 256 * 1024 * 1024;
    Logger::setConfig(logConfig);
 
    Logger::setOutPutFunc(output);
    asynLogger = std::unique_ptr<AsynLogger>(new AsynLogger());
    asynLogger->start();
    // std::unique_ptr<AsynLogger> asynLogger(new AsynLogger());
    // Logger::setOutPutFunc([&](const char* data ,size_t len){
    //     asynLogger->append(data,len);
    // });
    // asynLogger->start();

    vector<thread> vec(1);
    auto start = chrono::high_resolution_clock::now();
    for(auto& t: vec){
        t = thread(func);
        t.join();
    }
    auto end = chrono::high_resolution_clock::now();
    LL duration = (end - start).count();
    cout<<"耗时"<<duration<<endl;
    // fwrite("123456",1,6,stdout);


    
    // asynLogger.stop();
}