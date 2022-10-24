![GitHub](https://img.shields.io/github/license/JavarisLyn/FlashLogger)
# Description
FlashLogger is a C++ 11 based log library. Support both synchronous and asynchronous logging. Asynchronous strategy performence can reach 7,460,000 log items per second.

# Features
- Use buffer to delay flush
- Implement a thread-safe thread pool
- Record timestamp in memory to avoid frequently systemcall
- Use rdtsc to Reduce timestamp acqusition delay by rdtsc
- Use #define\inline function\conexpr to further reduce delay
- Use smart pointer&valgrind to manage object&check memory leak

# Get started
- include headers(static linking)
```
#include <lyf/Logger/AsynLogger.h>
#include <lyf/Logger/Logger.h>
#include <lyf/Logger/LogConfig.h>
```

- init in your project
```
FlashLogger::LogConfig logConfig;
logConfig.flushInterval = 10;
logConfig.initBufferNodeListSize = 3;
logConfig.rollFileSize = 256 * 1024 * 1024;
FlashLogger::Logger::setConfig(logConfig);

FlashLogger::Logger::setOutPutFunc(output);
asynLogger = std::unique_ptr<FlashLogger::AsynLogger>(new FlashLogger::AsynLogger());
asynLogger->start();

LOG_TRACE("server start: %s","flash web server");
```

# Development environment
+ 4vCPUs | 8GiB | c7.xlarge.2 CentOS 8.2 64bit | SSD 40 GB
+ g++ (GCC) 8.5.0
+ cmake version 3.3.2

# Experimental result
## disk read efficiency
```
[root@ecs-7073 ~]# hdparm -Tt /dev/sda

/dev/sda:
 Timing cached reads:   23824 MB in  2.00 seconds = 11930.43 MB/sec
 Timing buffered disk reads: 480 MB in  3.29 seconds = 145.82 MB/sec
```
```
[root@ecs-7073 log]# time dd if=/dev/zero of=/test.dbf bs=8k count=300000
300000+0 records in
300000+0 records out
2457600000 bytes (2.5 GB, 2.3 GiB) copied, 6.18518 s, 397 MB/s

real	0m6.186s
user	0m0.040s
sys	0m1.171s
```

## 线程池效率测试
### 日志处理阶段耗时对比(采用互斥锁) 
- 1e7次写入  

| timestamp | path | theadinfo | args substitution | effeciency(k item/s) | size(bytes/item) | effeciency(Mb/s) 
|  ----  | ----  | ----  | ----  | ----  | ----  | ----  |
| √  | √ | √ | √ | 122 | 210 | 244 |
|   | √ | √ | √ | 125 | 190 | 238 |
|   |  | √ | √ | 297 | 120 | 356 |
|   |  |  | √ | 373 | 110 | 411 |
|   |  |  |  | 549 | 100 | 549 |
- 1e6次写入  

| timestamp | path | theadinfo | args substitution | effeciency(k item/s) | size(bytes/item) | effeciency(Mb/s) 
|  ----  | ----  | ----  | ----  | ----  | ----  | ----  |
| √  | √ | √ | √ | 120 | 210 | 252 |
|   | √ | √ | √ | 134 | 190 | 255 |
|   |  | √ | √ | 265 | 120 | 318 |
|   |  |  | √ | 353 | 110 | 388 |
|   |  |  |  | 501 | 100 | 501 |
- 参数替换较为耗时,后面考虑实现延迟format
### 自旋锁与互斥锁对比
- 1e7次写入  

|  | effeciency(k item/s) | size(bytes/item) | effeciency(Mb/s) 
| ----  | ----  | ----  | ----  |
| 互斥锁 | ~~122~~ | ~~210~~ | ~~244~~ |
| 自旋锁-单线程 | 1335 | 180 | 240 |
| 自旋锁-5线程 | 1351 | 180 | 243 |

- 1e6次写入  

|  | effeciency(k item/s) | size(bytes/item) | effeciency(Mb/s) 
| ----  | ----  | ----  | ----  |
| 互斥锁 | ~~120~~ | ~~210~~ | ~~252~~ |
| 自旋锁-单线程 | 833 | 179 | 149 |
| 自旋锁-5线程  | 830 | 179 | 149 |

# Memory check
Use Valgrind to check memory leak.
See result at [valgrind_analysis](docs/valgrind_analysis.md)

# Code count
## Languages
| language | files | code | comment | blank | total | comment rate |
| :--- | ---: | ---: | ---: | ---: | ---: | ---: |
| C++ | 18 | 1007 | 396 | 251 | 1,554 | 30.39% |
| Markdown | 3 | 22 | 24 | 11 | 57 | 52.17% |