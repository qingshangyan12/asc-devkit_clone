#!/bin/bash
set -e                                      # 任一命令失败立即退出，避免沿用过期产物继续运行

# 架构与场景可通过环境变量覆盖：
#   ARCH         dav-3510(默认) / dav-2201
#   SCENARIO_NUM 1(默认, Nd2Nz, 双架构) / 2(Dn2Nz, 仅 dav-3510)
ARCH=${ARCH:-dav-2201}
SCENARIO_NUM=${SCENARIO_NUM:-1}
export SCENARIO_NUM                         # gen_data.py 按同一场景生成对应布局的输入

rm -rf build                                # 清除历史缓存，防止复用其它CANN版本的CMakeCache导致头文件与编译器不匹配
mkdir -p build && cd build                  # 创建并进入build目录
cmake -DCMAKE_ASC_ARCHITECTURES=${ARCH} -DCMAKE_ASC_RUN_MODE=sim -DSCENARIO_NUM=${SCENARIO_NUM} ..
make -j                                     # 编译工程
python3 ../scripts/gen_data.py              # 生成测试输入数据
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
