  OUTPUTFORMAT=1 PREQUANTMODE=1 PRERELUMODE=1 ENUNITFLAG=false  USEDATACOPY=false # OUTPUTFORMAT 1:ND 2:NZ;PREQUANTMODE 1:SCALAR 2:VECTOR; PRERELUMODE 0:disable 1:Norm relu 2:Leaky relu
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake .. -DOUTPUT_FORMAT=$OUTPUTFORMAT -DPRE_QUANT_MODE=$PREQUANTMODE -DPRE_RELU_MODE=$PRERELUMODE -DEN_UNIT_FLAG=$ENUNITFLAG -DUSE_DATA_COPY=$USEDATACOPY;
  make -j;    # 编译工程
  python3 ../scripts/gen_data.py -output_format=$OUTPUTFORMAT -pre_quant_mode=$PREQUANTMODE -pre_relu_mode=$PRERELUMODE; # 生成测试输入数据
  ./demo                           # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
