
# Bank_Conflict_L1样例

## 概述

本样例基于完整矩阵计算通路，介绍在L1上优化Bank读写冲突的方案，并展示了优化前后实测数据上的变化。

## 支持的产品

- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── bank_conflict_l1
│   ├── scripts
│   │   ├── gen_data.py             // 输入数据和真值数据生成脚本
│   │   └── verify_result.py        // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt              // 编译工程文件
│   ├── data_utils.h                // 数据读入写出函数
│   ├── bank_conflict_l1_v1.asc     // 无优化
│   └── bank_conflict_l1_v2.asc     // 有优化
```

## 算子描述

bank_conflict_l1_v1和bank_conflict_l1_v2样例均实现的是[m, n, k]固定为[128, 256, 4096]的Matmul算子，并延k轴切分32次进行计算。
两个样例的实现均为在L1上为A、B矩阵各申请两块内存空间，并通过循环次数的奇偶性选择一轮计算所使用的内存空间。
其中，bank_conflict_l1_v1样例申请的内存空间均位于Bank0上，以此构造矩阵搬入和搬出的读写冲突。
bank_conflict_l1_v2样例A、B矩阵各自的两块内存空间分别位于Bank0和Bank1上，以此优化矩阵搬入和搬出的读写冲突。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行算子。
- 配置环境变量  
  请根据当前环境上CANN开发套件包的[安装方式](../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
  - 默认路径，root用户安装CANN软件包
    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - 默认路径，非root用户安装CANN软件包
    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - 指定路径install_path，安装CANN软件包
    ```bash
    source ${install_path}/cann/set_env.sh
    ```
- 样例执行
  ```bash
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake ..;make -j;                # 编译工程，注意CMakeLists中根据是否优化Bank冲突在以下两个.asc文件中切换
  python3 ../scripts/gen_data.py   # 生成测试输入数据
  msprof ./demo                    # 执行编译生成的可执行程序，执行样例，采集性能数据
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```
  在build/PROF_*/mindstudio_profiler_output/op_summary_*.csv中可查看性能数据

## 性能结果



