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

## 线程池效率测试
### 
2022-10-11 17:30:50 20bytes
tid:5749  8bytes
/root/lyf/FlashLogger/src/Benchmark/AsynLoggerBenchmark.cpp line:27 70bytes
TRACE: 
AsynLoggerAsynLoggerAsynLoggerAsynLoggerAsynLoggerAsynLoggerAsynLoggerAsynLoggerAsynLogger

| timestamp | theadinfo | path | args substitution | effeciency(k item/s) |
|  ----  | ----  | ----  | ----  | ----  |
| √  | √ | √ | √ | 121 |
| √  | √ | √ |  | 131 |
| √  | √ |  |  | 373 |
| √  |  |  |  | 460 |
|   |  |  |  | 534 |

# Memory check
Use Valgrind to check memory leak.
See result at [valgrind_analysis](docs/valgrind_analysis.md)

# Code count
## Languages
| language | files | code | comment | blank | total | comment rate |
| :--- | ---: | ---: | ---: | ---: | ---: | ---: |
| C++ | 18 | 1007 | 396 | 251 | 1,554 | 30.39% |
| Markdown | 3 | 22 | 24 | 11 | 57 | 52.17% |