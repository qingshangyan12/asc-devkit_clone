  SCENARIO=1                                                           # 选择执行场景
  mkdir -p build && cd build;                                          # 创建并进入 build 目录
  cmake -DSCENARIO_NUM=$SCENARIO -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j;  # 编译工程（默认npu模式）
  python3 ../scripts/gen_data.py
  ./demo
#  python3 ../scripts/verify_result.py output/output.bin output/golden.bin
