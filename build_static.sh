#显示执行过程
###
 # @Version: 
 # @Author: LiYangfan.justin
 # @Date: 2022-10-02 12:05:03
 # @LastEditTime: 2022-10-02 12:16:48
 # @Description: 
 # Copyright (c) 2022 by Liyangfan.justin, All Rights Reserved. 
### 
set -x

SOURCE_DIR=`pwd`
cd build
cmake -DCMAKE_INSTALL_PREFIX=/usr ..
make
make install