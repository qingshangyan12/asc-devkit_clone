  ISCHANNELSPLIT=true USEDATACOPY=false # ISCHANNELSPLIT true 打开channelsplit功能 USEDATACOPY true 使用DataCopy接口
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake .. -DIS_CHANNEL_SPLIT=$ISCHANNELSPLIT -DUSE_DATA_COPY=$USEDATACOPY;
  make -j;    # 编译工程
  python3 ../scripts/gen_data.py -is_channel_split=$ISCHANNELSPLIT; # 生成测试输入数据
  ./demo                           # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
