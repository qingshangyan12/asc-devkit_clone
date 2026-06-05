#!/bin/bash
#source /mnt/workspace/z00940346/cann530/cann/set_env.sh
mkdir -p build && cd build;   # 创建并进入build目录
cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # NPU仿真模式
python3 ../scripts/gen_data.py   # 生成测试输入数据
./demo                        # 执行编译生成的可执行程序，执行样例
python3 ../scripts/verify_result.py output/case1/output.bin output/case1/golden.bin float32   # 验证case1输出结果
python3 ../scripts/verify_result.py output/case2/output.bin output/case2/golden.bin float16   # 验证case2输出结果
python3 ../scripts/verify_result.py output/case3/output.bin output/case3/golden.bin float16   # 验证case3输出结果
python3 ../scripts/verify_result.py output/case4/output.bin output/case4/golden.bin float32   # 验证case4输出结果
python3 ../scripts/verify_result.py output/case5/output.bin output/case5/golden.bin int8      # 验证case5输出结果
python3 ../scripts/verify_result.py output/case6/output.bin output/case6/golden.bin int8      # 验证case6输出结果
