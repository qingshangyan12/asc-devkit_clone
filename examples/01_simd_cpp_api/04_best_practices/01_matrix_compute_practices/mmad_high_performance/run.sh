mkdir -p build && cd build;                                          # 创建并进入 build 目录
cmake -DNPU_ARCH=dav-2201 -DCASE_TYPE=1 ..;make -j;                 # 编译 Case 0: 基础高性能 mmad（默认）
# cmake -DNPU_ARCH=dav-2201 -DCASE_TYPE=1 ..;make -j;               # 编译 Case 1: L2 Cache 切分优化
python3 ../scripts/gen_data.py                                       # 生成测试输入数据
./demo                                                               # 执行编译生成的可执行程序
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
