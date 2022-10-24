![GitHub](https://img.shields.io/github/license/JavarisLyn/FlashLogger)
# Description
FlashLogger is a C++ 11 based log library. Support both synchronous and asynchronous logging. Asynchronous strategy performence can reach 7,460,000 log items per second.

# Features
- Use buffer to delay flush
- Implement a thread-safe thread pool
- Record timestamp in memory to avoid frequently systemcall
- Use rdtsc to Reduce timestamp acqusition delay by rdtsc
- Use #define\inline function\constexpr to further reduce delay
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

## Benchmarks
### ablation test(with mutex lock) 
- 1e7 iterations, 5 threads

| timestamp | path | theadinfo | args substitution | duration(ms) | effeciency(k item/s) | size(bytes/item) | effeciency(Mb/s) 
|  ----  | ----  | ----  | ----  | ----  | ----  | ----  | ----  |
| √  | √ | √ | √ | 7373 | 1356 | 180 | 244 |
|   | √ | √ | √ | 7004 | 1428 | 160 | 229 |
| √ |  | √ | √ | 3601 | 2777 | 110 | 305 |
| √ | √ |  | √ | 6702 | 1492 | 170 | 254 |
| √ | √ | √ |  |  |  |  |  |
|  |  |  |  | 1828 | 5470 | 73 | 399 |

ps: format takes up a lot of time, delay format is used in nanolog.
 
### spinlock & mutex lock
- 1e7 iterations, 5 threads  

- spinlock  

| round | duration(ms) | effeciency(k item/s) | size(bytes/item) | effeciency(Mb/s) 
| ----  | ----  | ----  | ----  | ----  |
| 1 | 7320 | 1366 | 180 | 246 |
| 2 | 7436 | 1344 | 180 | 244 |
| 3 | 7370 | 1357 | 180 | 243 |
| 4 | 7300 | 1370 | 180 | 247 |
| 5 | 7440 | 1344 | 180 | 242 |
| avg | 7373 | 1356 | 180 | 246 |

- mutex lock  

| round | duration(ms) | effeciency(k item/s) | size(bytes/item) | effeciency(Mb/s) 
| ----  | ----  | ----  | ----  | ----  |
| 1 | 8465 | 1181 | 180 | 213 |
| 2 | 8648 | 1156 | 180 | 208 |
| 3 | 8431 | 1186 | 180 | 213 |
| 4 | 8365 | 1195 | 180 | 215 |
| 5 | 8499 | 1177 | 180 | 212 |
| avg | 8482 | 1179 | 180 | 212 |

# Memory check
Use Valgrind to check memory leak.
See result at [valgrind_analysis](docs/valgrind_analysis.md)

# Code count
## Languages
| language | files | code | comment | blank | total | comment rate |
| :--- | ---: | ---: | ---: | ---: | ---: | ---: |
| C++ | 18 | 1007 | 396 | 251 | 1,554 | 30.39% |
| Markdown | 3 | 22 | 24 | 11 | 57 | 52.17% |