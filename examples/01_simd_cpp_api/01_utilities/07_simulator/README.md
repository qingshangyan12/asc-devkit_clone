# 基于MatmulLeakyRelu算子的CAmodel仿真样例

## 概述

本样例基于MatmulLeakyRelu算子，聚焦 CAmodel 仿真与问题分析流程。用户可根据输出的性能数据，快速定位算子的软、硬件性能瓶颈，提升算子性能的分析效率。

## 支持的产品

- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── 07_simulator
│   ├── CMakeLists.txt          # 编译工程文件（仅生成demo_sim）
│   ├── data_utils.h            # 数据读入写出函数
│   ├── matmul_leakyrelu.asc    # Ascend C算子实现与调用样例
│   └── scripts
│       ├── gen_data.py         # 输入数据和真值数据生成脚本
│       └── verify_result.py    # 真值比对脚本
```

## 算子描述
  MatmulLeakyRelu算子通过将矩阵乘加（Matmul）与LeakyRelu激活函数计算实现，具体功能描述可参考[MatmulLeakyRelu算子详情](../../../00_introduction/03_fusion_operation/matmul_leakyrelu/README.md)章节。


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
  mkdir -p build && cd build
  cmake -DNPU_ARCH=dav-2201 ..
  make -j
  python3 ../scripts/gen_data.py
  ./demo_sim
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin
  ```
