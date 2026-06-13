#!/bin/bash
set -e                                      # 任一命令失败立即退出，避免沿用过期产物继续运行

rm -rf build                                # 清除历史缓存，防止复用其它CANN版本的CMakeCache导致头文件与编译器不匹配
mkdir -p build && cd build                  # 创建并进入build目录
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCMAKE_ASC_RUN_MODE=sim ..
make -j                                     # 编译工程
python3 ../scripts/gen_data.py              # 生成测试输入数据
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
