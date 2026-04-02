# Transpose样例

## 概述

本样例基于Transpose高阶API实现NZ2ND，1轴2轴互换的场景。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```plain
├── transpose
│   ├── scripts
│   │   └── gen_data.py         // 输入数据和真值数据生成脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── transpose.asc           // Ascend C样例实现 & 调用样例
```

## 样例描述

- 样例功能：  
  TransposeCustom样例，对输入数据进行数据排布及Reshape操作。
- 样例规格：  
  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">transpose</td></tr>

  <tr><td rowspan="3" align="center">样例输入</td></tr>
  <tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x</td><td align="center">[1, 2, 2, 4, 16, 16]</td><td align="center">half</td><td align="center">NZ</td></tr>
  <tr><td rowspan="2" align="center">样例输出</td></tr>
  <tr><td align="center">y</td><td align="center">[1, 64, 2, 32]</td><td align="center">half</td><td align="center">ND</td></tr>

  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">transpose_custom</td></tr>
  </table>

- 样例实现：  
  本样例中实现的是固定shape为输入x[1, 2, 2, 4, 16, 16]，输出y[1, 64, 2, 32]的TransposeCustom样例，针对NZ2ND场景，实现1、2轴互换。

  输入Tensor { shape:[B, N, H/N/16, S/16, 16, 16], origin_shape:[B, N, S, H/N], format:"NZ", origin_format:"ND"}

  输出Tensor { shape:[B, S, N, H/N], origin_shape:[B, S, N, H/N], format:"ND", origin_format:"ND"}

  - Kernel实现

    计算逻辑包含以下步骤：
    1. 将输入数据从Global Memory搬运到Local Memory
    2. 使用Transpose高阶API进行NZ到ND格式转换，实现1、2轴互换
    3. 将计算结果从Local Memory搬运到Global Memory

  - Tiling实现

    使用Ascend C提供的GetTransposeTilingInfo接口，获取所需的Tiling参数，并调用GetTransposeMaxMinTmpSize接口获取Transpose接口计算所需的临时空间大小。

  - 调用实现  
    使用内核调用符<<<>>>调用核函数。

## 编译运行  

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量  
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
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
  mkdir -p build && cd build;   # 创建并进入build目录
  cmake ..;make -j;             # 编译工程
  python3 ../scripts/gen_data.py   # 生成测试输入数据
  ./demo                        # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
  ```

  执行结果如下，说明精度对比成功。

  ```bash
  test pass!
  ```
