rm -rf build
mkdir -p build && cd build;   # 创建并进入build目录
cmake ..;make -j VERBOSE=1;             # 编译工程
# 在build目录执行以下内容
./add                         # 执行样例