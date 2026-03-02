INPUT_TYPE=1 INPUT_SIZE=224 # 1: half, 2: float(无饱和模式)
 
mkdir -p build && cd build;   # 创建并进入build目录 


echo "====== start compile ======" 
cmake .. -DINPUT_TYPE=$INPUT_TYPE -DINPUT_SIZE=$INPUT_SIZE;make -j;             # 编译工程 


echo "====== start gen golden ======" 
python3 ../scripts/gen_data.py -inputType=$INPUT_TYPE -inputSize=$INPUT_SIZE  # 生成测试输入数据 


echo "====== start run kernel ======" 
# ./demo                        # 执行编译生成的可执行程序，执行样例 
./demo 


echo "====== start compare result ======" 
python3 ../scripts/verify_result.py -output_type=${INPUT_TYPE} -output=output/output.bin -golden=output/golden.bin -input=input/input_x.bin # 验证输出结果是否正确，确认算法逻辑正确