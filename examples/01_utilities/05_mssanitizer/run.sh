rm -rf build
# 参考的/data/cty/asc-devkit/examples/02_features/06_static_tensor_programming
mkdir -p build && cd build;      # 创建并进入build目录
cmake ..;make -j;                # 编译工程
python3 ../scripts/gen_data.py   # 生成测试输入数据
# --leak-check              开启内存泄漏检查
# --check-unused-memory=yes 开启分配内存未使用检查
# mssanitizer ./demo --leak-check=yes --check-unused-memory=yes  # 执行编译生成的可执行程序，执行样例          
# mssanitizer ./demo --tool=racecheck                            # 执行编译生成的可执行程序，执行样例   
mssanitizer ./demo --tool=initcheck                            # 执行编译生成的可执行程序，执行样例   
python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确