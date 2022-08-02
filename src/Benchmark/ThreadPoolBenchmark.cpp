/*
 * @Descripttion: 
 * @version: 
 * @Author: justin
 * @Date: 2022-08-02 10:53:21
 * @LastEditTime: 2022-08-02 16:40:44
 * @copyright: Copyright (c) 2022
 */

#include "../ThreadPool/ThreadPool.h"
#include<future>
#include<chrono>
#include<thread>

using namespace std;
using LL = long long;
const int N = 1e8;
const int poolSize = 4;
const int jobNum = 300;
vector<future<LL>> futureVec(jobNum);
vector<promise<LL>> promiseVec(jobNum);
vector<LL> result(jobNum);
/* promise配合future进行线程间通信 */
/* future绑定promise后，线程1用promise传值，线程2处理后，会回传到future中。*/

void oneThread(){
    auto start = chrono::high_resolution_clock::now();

    for(int i=0;i<jobNum;i++){
        LL t_result = 0;
        for(int j = 1;j<N;j++){
            t_result+=j;
        }
        result[i] = t_result;
    }

    auto end = chrono::high_resolution_clock::now();
    LL duration = (end - start).count();

    for(int i=0;i<jobNum;i++){
        cout<<"future"<<i<<"="<<result[i]<<endl;
    }

    cout<<"程序运行时间"<<duration/1000000.0<<"ms";
}

void multiThread(){
    auto start = chrono::high_resolution_clock::now();
    vector<thread> threadVec;
   
    for(int i=0;i<jobNum;i++){
        futureVec[i] = promiseVec[i].get_future();
        thread t([&](promise<LL>& p){
                LL result = 0;
                for(int j = 1;j<N;j++){
                    result+=j;
                }
                p.set_value(result);
                
            }
           ,ref(promiseVec[i])
        );
        threadVec.emplace_back(move(t));
    }
    for(int i=0;i<jobNum;i++){
        threadVec[i].join(); 
    }

    auto end = chrono::high_resolution_clock::now();
    LL duration = (end - start).count();
    
    for(int i=0;i<jobNum;i++){
        cout<<"future"<<i<<"="<<futureVec[i].get()<<endl;
    }

    cout<<"程序运行时间"<<duration/1000000.0<<"ms";
}

void threadPool(){
    auto start = chrono::high_resolution_clock::now();

    ThreadPool threadpool(poolSize);
    
    for(int i=0;i<jobNum;i++){
        futureVec[i] = promiseVec[i].get_future();
        threadpool.addTask([&](promise<LL>& p){
            LL result = 0;
            for(int j = 1;j<N;j++){
                result+=j;
            }
            p.set_value(result);
        },ref(promiseVec[i]));
    }
    threadpool.shutdown();

    auto end = chrono::high_resolution_clock::now();
    LL duration = (end - start).count();

    for(int i=0;i<jobNum;i++){
        cout<<"future"<<i<<"="<<futureVec[i].get()<<endl;
    }

    cout<<"程序运行时间"<<duration/1000000.0<<"ms";
}



int main(){
    // oneThread();
    // multiThread();
    threadPool();
    return 0;
}