mkdir -p build && cd build;      # 创建并进入build目录
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCMAKE_ASC_RUN_MODE=sim ..;make -j;    # 编译工程
python3 ../scripts/gen_data.py   # 生成测试输入数据
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
