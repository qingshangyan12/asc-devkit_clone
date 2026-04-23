SCENARIO=2                                                           # 选择执行场景（1为MemBase GELU，2为RegBase GELU/VF融合）
mkdir -p build && cd build;                                          # 创建并进入 build 目录
cmake -DSCENARIO_NUM=$SCENARIO -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;  # 编译工程（默认npu模式）
python3 ../scripts/gen_data.py                                       # 生成测试输入数据
./demo                                                               # 执行编译生成的可执行程序
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
