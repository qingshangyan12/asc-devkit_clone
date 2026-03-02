SCENARIO_NUM=1
 
mkdir -p build && cd build;   # 创建并进入build目录 


echo ">>>>>>>>>>>>>>>>>> start compile" 
cmake .. -DSCENARIO_NUM=$SCENARIO_NUM;make -j;             # 编译工程 


echo ">>>>>>>>>>>>>>>>>> start gen golden" 
python3 ../scripts/gen_data.py -scenario_num=$SCENARIO_NUM  # 生成测试输入数据 


echo ">>>>>>>>>>>>>>>>>> start run kernel" 
# ./demo                        # 执行编译生成的可执行程序，执行样例 
./demo 


echo ">>>>>>>>>>>>>>>>>> start compare result" 
python3 ../scripts/verify_result.py -scenario_num=${SCENARIO_NUM} -output=output/output_z.bin -golden=output/golden.bin -input_x=input/input_x.bin -input_y=input/input_y.bin # 验证输出结果是否正确，确认算法逻辑正确