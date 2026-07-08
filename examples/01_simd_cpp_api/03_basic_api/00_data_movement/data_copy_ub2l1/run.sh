rm build -rf
mkdir -p build && cd build;                                                                        # 创建并进入build目录
cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=2 ..;make -j;                      # 编译工程，默认npu模式
python3 ../scripts/gen_data.py                                                                     # 生成测试输入数据
./demo                                                                                             # 执行编译生成的可执行程序，执行样例