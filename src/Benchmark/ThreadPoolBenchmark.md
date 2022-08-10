<!--
 * @Descripttion: 
 * @version: 
 * @Author: justin
 * @Date: 2022-08-02 11:42:59
 * @LastEditTime: 2022-08-10 11:31:30
 * @copyright: Copyright (c) 2022
-->
# 线程池效率测试
### 4vCPUs | 8GiB | c7.xlarge.2 CentOS 8.2 64bit
| 计算量   | 单线程  | 多线程  | 线程池(size=4)  |线程池(size=10)  |
|  ----  | ----  | ----  | ----  | ----  |
| 100*1e8  | 21206ms | 6441ms | 6747ms | 6692ms |
| 300*1e8  | 63535ms | 19645ms | 19834ms | 19706ms |