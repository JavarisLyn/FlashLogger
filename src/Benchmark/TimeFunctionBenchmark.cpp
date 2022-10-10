/*
 * @Version: 
 * @Author: LiYangfan.justin
 * @Date: 2022-08-14 00:14:41
 * @LastEditTime: 2022-10-09 23:22:27
 * @Description: time related functions benchmark
 * Copyright (c) 2022 by Liyangfan.justin, All Rights Reserved. 
 */
#include <iostream>
#include<sys/time.h>//gettimeofday
#include <time.h> //localtime
#include <chrono> //chrono
#include <x86intrin.h> //__rdtsc()
#include "../Utils/tscns.h"
using namespace std;

unsigned long long rdtsc(void)
{
  unsigned hi, lo;
  __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
  return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}

int main(){
    TSCNS tscns;

    tscns.init();

    auto start = chrono::high_resolution_clock::now();
    
    for(int i=0;i<1e8;i++){
        // struct timeval tv;
        // gettimeofday(&tv, NULL);//1970到现在的时间戳
        //1925715009

        // long int tim = 0;
        // time(&tim);
        // struct tm * gmt = NULL;
        // gmt = localtime(&tim);//将tm时间转为本地时间
        //79877716099

        // struct timespec time_now;
		// clock_gettime(CLOCK_MONOTONIC, &time_now);

        // cout<<rdtsc()<<endl; 

        // cout<<__rdtsc()<<endl;    
        //     {
        //    __asm__ __volatile__("" : : : "memory");
        //     uint64_t r =  __rdtsc();
        //     __asm__ __volatile__("" : : : "memory");
        //     cout<<r<<endl;
        //     }

        // int64_t ns = tscns.rdns();
        // int64_t tsc = tscns.rdtsc();
        // cout<<ns<<endl;
        
    }
    auto end = chrono::high_resolution_clock::now();
    long long int duration = (end - start).count();
    cout<<"耗时"<<duration<<endl;
    
}