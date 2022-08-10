/*
 * @Version: 
 * @Author: LiYangfan.justin
 * @Date: 2022-08-08 01:24:31
 * @LastEditTime: 2022-08-10 01:17:04
 * @Description: 
 * Copyright (c) 2022 by Liyangfan.justin, All Rights Reserved. 
 */
#ifndef _LOG_CONFIG_
#define _LOG_CONFIG_
#include <string>
/* 这里考虑设置成结构体？ */
class LogConfig{
public:
    int flushInterval;
    int initBufferNodeListSize;
    size_t flushFileBufferSize;//这个还没配置
    size_t bufferNodeSize;
    size_t rollFileSize;
    std::string baseName = "FlashLogFile:";
};

#endif
