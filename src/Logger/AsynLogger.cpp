/*
 * @Version: 
 * @Author: LiYangfan.justin
 * @Date: 2022-08-06 21:17:11
 * @LastEditTime: 2022-10-02 17:24:18
 * @Description: 
 * Copyright (c) 2022 by Liyangfan.justin, All Rights Reserved. 
 */
#include "AsynLogger.h"
#include "FileWriter.h"
#include <chrono>
#include <iostream>

using namespace FlashLogger;

AsynLogger::AsynLogger(int bufferNodeListSize,int flushInterval):write_mtx(ATOMIC_FLAG_INIT){
    head = std::shared_ptr<BufferNode>(new BufferNode);
    tail = std::shared_ptr<BufferNode>(new BufferNode);
    head->next = tail;
    tail->prev = head;
    this->bufferNodeListSize = bufferNodeListSize;
    this->flushInterval = flushInterval;
    running = false;
}

AsynLogger::~AsynLogger(){
    // std::cout<<"destructor"<<std::endl;
    if(running){
        stop();
    }
    /* 小心内存泄漏，双链表的循环引用问题，后面改成weak指针 */
    cur = head;
    head = nullptr;
    while(cur!=nullptr){
        std::shared_ptr<BufferNode> next = cur->next;
        if(next != nullptr){
            next->prev = nullptr;
        }
        // cur = nullptr;
        cur = next;
    }
    tail = nullptr;
    
}

void AsynLogger::start(){
    if(running == true){
        return;
    }
    for(int i=0;i<bufferNodeListSize;i++){
        auto node = AsynLogger::newBufferNode();
        addTail(node);
    }
    cur = head->next;

    running = true;
    std::cout<<"start"<<&running<<running<<std::endl;
    /* class function,remember to use `&` + this pointer */
    backgroundThread = std::thread(&AsynLogger::backgroundFunc,this);

}

void AsynLogger::stop(){
    assert(running = true);
    running = false;
    cv.notify_one();
    backgroundThread.join();
}

std::shared_ptr<AsynLogger::BufferNode> AsynLogger::newBufferNode(){
    std::shared_ptr<BufferNode> bufferNodePtr = std::shared_ptr<BufferNode>(new BufferNode());
    bufferNodePtr->buffer = std::unique_ptr<LogBuffer>(new LogBuffer());
    return bufferNodePtr;
}

void AsynLogger::append(const char * data,size_t len){

    /* 加互斥锁 */
    //std::unique_lock<std::mutex> lock(mtx);
    /* 加自旋锁 */
    while(write_mtx.test_and_set(std::memory_order_acquire)){}

    if(len < cur->buffer->getAvaliable()){
        /* 加入日志时间，后面会统一到loggin中 */
        // time_t now = 0;
        // time(&now);
        // struct tm tm_time;
        // localtime_r(&now,&tm_time);
        // strftime(timeStr,sizeof(timeStr),"%Y-%m-%d-%H-%M-%S:",&tm_time);
        // cur->buffer->append(timeStr,20);
        cur->buffer->append(data,len);
        write_mtx.clear(std::memory_order_release); //释放锁
        return;
    }
    
    /* 该BufferNode空间不够，需要添加到待写队列并新建空的BufferNode */
    // std::cout<<"添加到落盘队列"<<std::endl;
    toWriteBufferNodeList.push_back(cur);
    removeHead();
    bufferNodeListSize-=1;
    if(bufferNodeListSize<=0){
        std::shared_ptr<BufferNode> bufferNode = AsynLogger::newBufferNode();
        addTail(bufferNode);
        bufferNodeListSize += 1;
    }

    cur = head->next;
    /* 同上 */
    // time_t now = 0;
    // time(&now);
    // struct tm tm_time;
    // localtime_r(&now,&tm_time);
    // strftime(timeStr,sizeof(timeStr),"%Y-%m-%d-%H-%M-%S:",&tm_time);
    // cur->buffer->append(timeStr,20);

    cur->buffer->append(data,len);

    /* 唤醒刷盘线程 */
    cv.notify_one();

    write_mtx.clear(std::memory_order_release); //释放自旋锁
}

/* 后台刷盘线程 */
void AsynLogger::backgroundFunc(){
    std::cout<<"background"<<&running<<running<<std::endl;
    assert(running == true);
    assert(bufferNodeListSize > 0);


    while(running){
        /* 定期检查刷盘队列 */
        {
            std::unique_lock<std::mutex> lock(flush_mtx);
            if(toWriteBufferNodeList.empty()){
                /* 阻塞flushInterval后刷盘 */
                cv.wait_for(lock, std::chrono::seconds(flushInterval));
            }
            if(toWriteBufferNodeList.empty() && cur->buffer->getLength()==0){
                continue;
            }

            //争夺自旋锁
            while(write_mtx.test_and_set(std::memory_order_acquire)){}
            /* 每次刷盘是也顺便把当前buffer刷了，也可以考虑不刷当前节点 */
            // if(cur->buffer->getLength()!=0){
            //     toWriteBufferNodeList.push_back(cur);
            //     removeHead();
            //     bufferNodeListSize-=1;
            // }
            if(bufferNodeListSize<=0){
                std::shared_ptr<BufferNode> bufferNode = AsynLogger::newBufferNode();
                addTail(bufferNode);
                bufferNodeListSize+=1;
            }
            cur = head->next;
            write_mtx.clear(std::memory_order_release); //释放锁
        }
        // std::cout<<"刷盘:"<<toWriteBufferNodeList.size()<<std::endl;
        /* 刷盘 */
        for(int i=0;i<toWriteBufferNodeList.size();i++){
            logFileWriter.append(toWriteBufferNodeList[i]->buffer->getData(),toWriteBufferNodeList[i]->buffer->getLength());
        }

        logFileWriter.flush();
        {
            /* 这里其实可以两个队列用两把锁 */
            std::unique_lock<std::mutex> lock(flush_mtx);
            //争夺自旋锁
            while(write_mtx.test_and_set(std::memory_order_acquire)){}
            for(auto& bufferNode :toWriteBufferNodeList){
                bufferNode->buffer->clear();
                addTail(bufferNode);
                bufferNodeListSize += 1;
            }
            toWriteBufferNodeList.clear();
            write_mtx.clear(std::memory_order_release); //释放锁
        }
    }
    /* 最后退出前也要刷盘,需要把缓冲区中的也刷掉 */
    logFileWriter.append(cur->buffer->getData(),cur->buffer->getLength());
    logFileWriter.flush();
}

void AsynLogger::removeHead(){
    head->next->next->prev = head;
    head->next = head->next->next;
}

void AsynLogger::addTail(std::shared_ptr<BufferNode> bufferNodePtr){
    bufferNodePtr->next = tail;
    tail->prev->next = bufferNodePtr;
    bufferNodePtr->prev = tail->prev;
    tail->prev = bufferNodePtr;

}