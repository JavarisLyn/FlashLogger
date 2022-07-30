#显示执行过程
set -x

SOURCE_DIR=`pwd`
cd build
cmake ..
make