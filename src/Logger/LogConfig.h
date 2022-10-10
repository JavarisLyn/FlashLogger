/*
 * @Version: 
 * @Author: LiYangfan.justin
 * @Date: 2022-08-08 01:24:31
 * @LastEditTime: 2022-10-09 22:36:00
 * @Description: 
 * Copyright (c) 2022 by Liyangfan.justin, All Rights Reserved. 
 */
#ifndef _LOG_CONFIG_
#define _LOG_CONFIG_
#include <string>
/* 这里考虑设置成结构体？ */

namespace FlashLogger
{

class LogConfig{
public:
    int flushInterval;
    int initBufferNodeListSize;
    size_t flushFileBufferSize;//这个还没配置
    size_t bufferNodeSize;
    size_t rollFileSize = 256 * 1024 * 1024;
    std::string baseName = "FlashLogFile:";
};

} // namespace FlashLogger
#endif
