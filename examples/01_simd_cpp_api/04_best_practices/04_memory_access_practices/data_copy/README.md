
  cd ..
  rm -rf build
  SCENARIO_NUM=3                       # 选择执行场景
  COPY_DST=UB
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_RUN_MODE=npu  -DCMAKE_ASC_ARCHITECTURES=dav-2201 -DCOPY_DST=$COPY_DST ..;make -j;  # 编译工程（默认npu模式）
  python3 ../scripts/gen_data.py -scenarioNum $SCENARIO_NUM -copyDst $COPY_DST   # 生成测试输入数据
  msprof ./demo

  ./demo                           # 执行
  msprof ./demo
  # COPY_DST=UB时可执行结果校验；COPY_DST=L1仅演示GM到L1搬运，不产生可校验输出。
  if [ "$COPY_DST" = "UB" ]; then python3 ../scripts/verify_result.py output/output.bin output/golden.bin; fi

