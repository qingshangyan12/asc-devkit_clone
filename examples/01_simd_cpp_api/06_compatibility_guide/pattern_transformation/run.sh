  mkdir -p build && cd build
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 -DCMAKE_ASC_RUN_MODE=cpu -DSCENARIO_NUM=1 .. && make -j       # 编译（默认 NPU 模式）
  python3 ../scripts/gen_data.py                                # 生成测试输入数据
  ./demo                                                        # 执行编译生成的可执行程序
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin  # 验证结果