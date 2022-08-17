<!--
 * @Version: 
 * @Author: LiYangfan.justin
 * @Date: 2022-08-14 00:56:09
 * @LastEditTime: 2022-08-17 11:17:52
 * @Description: 
 * Copyright (c) 2022 by Liyangfan.justin, All Rights Reserved. 
-->
# 时间函数耗时测试
### 4vCPUs | 8GiB | c7.xlarge.2 CentOS 8.2 64bit
| 计算量   | localtime  | gettimeofday  | clock_gettime | tscns |
|  ----  | ----  | ----  | ----  | ----  |
| 1e6  | 792,483,506ns | 19,295,216ns |  | 15,799,845ns |
| 1e8  | 79,877,716,099ns | 1,925,715,009ns | 1,929,916,877ns | 1,578,887,805ns |

## gettimeofday和clock_gettime都是vdso

