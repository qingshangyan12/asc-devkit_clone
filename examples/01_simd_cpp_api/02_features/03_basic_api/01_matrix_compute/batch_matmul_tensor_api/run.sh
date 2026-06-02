source /mnt/workspace/cann0530/cann/set_env.sh
mkdir -p build && cd build;   # 创建并进入build目录
cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # NPU仿真模式
python3 ../scripts/gen_data.py   # 生成测试输入数据
./demo                        # 执行编译生成的可执行程序，执行样例
python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确