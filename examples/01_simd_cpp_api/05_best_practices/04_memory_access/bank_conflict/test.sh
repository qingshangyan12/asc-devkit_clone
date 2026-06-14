rm -rf build
mkdir -p build && cd build;   # 创建并进入build目录
cmake ..;make -j;             # 编译工程
python3 ../scripts/gen_data.py   # 生成测试输入数据
./demo                        # 执行编译生成的可执行程序，执行样例
python3 ../scripts/verify_result.py output/output_z.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
# python3 ../scripts/verify_result.py output/output_z_v2.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确