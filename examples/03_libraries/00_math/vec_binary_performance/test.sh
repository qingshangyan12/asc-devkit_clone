INPUT_TYPE=1 INPUT_SIZE=4096 # 输入Tensor的数据类型是half，数据大小是32K(32*1024=32768)
mkdir -p build && cd build; # 创建并进入build目录
cmake .. -DINPUT_TYPE=$INPUT_TYPE -DINPUT_SIZE=$INPUT_SIZE;make -j; # 编译工程
python3 ../scripts/gen_data.py -inputType=$INPUT_TYPE -inputSize=$INPUT_SIZE # 生成测试输入数据
./demo # 执行编译生成的可执行程序，执行样例
# python3 ../scripts/verify_result.py output/output_z.bin output/golden.bin # 验证输出结果是否正确，确认算法逻辑正确