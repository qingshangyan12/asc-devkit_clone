  mkdir -p build && cd build;      # 创建并进入build目录
  cmake -DRUN_MODE=npu -DNPU_ARCH=dav-2201 ..;make -j;
  python3 ../scripts/gen_data.py   # 生成测试输入数据
  ./demo                           # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin 
