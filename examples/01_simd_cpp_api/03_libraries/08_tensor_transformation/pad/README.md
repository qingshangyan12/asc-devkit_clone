# Pad样例

## 概述

本样例基于Pad高阶API实现将二维Tensor行数据填充到32B对齐的功能。

如果Tensor的行已32B对齐，且全部为有效数据，则不支持调用本接口对齐。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```plain
├── pad
│   ├── scripts
│   │   └── gen_data.py         // 输入数据和真值数据生成脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── pad.asc                 // Ascend C样例实现 & 调用样例
```

## 样例描述

- 样例功能：

  将二维Tensor行数据填充到32B对齐的功能，如果Tensor的行已32B对齐，且全部为有效数据，则不支持调用本接口对齐。

- 样例规格：

  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center"> pad </td></tr>

  <tr><td rowspan="3" align="center">样例输入</td></tr>
  <tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">src</td><td align="center">[16, 31]</td><td align="center">float</td><td align="center">ND</td></tr>

  <tr><td rowspan="2" align="center">样例输出</td></tr>
  <tr><td align="center">dst</td><td align="center">[16, 32]</td><td align="center">float</td><td align="center">ND</td></tr>

  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">pad_custom</td></tr>
  </table>

- 样例实现：

  本样例中实现的是固定shape为输入src[16, 31]，输出dst[16, 32]的pad_custom样例。

  - Kernel实现

    核心计算步骤：将数据从Global Memory搬运到LocalTensor后，调用Pad高阶API接口完成数据填充，再将结果搬回Global Memory。

  - Tiling实现

    1. 调用GetPadMaxMinTmpSize接口获取Pad接口完成计算所需的最小和最大临时空间大小
    2. 调用PadTilingFunc接口获取Pad所需的Tiling信息，并传入Pad接口参与计算。

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
  cmake -DNPU_ARCH=dav-2201 ..;make -j;             # 编译工程
  python3 ../scripts/gen_data.py   # 生成测试输入数据
  ./demo                        # 执行编译生成的可执行程序，执行样例
  ```

  执行结果如下，说明精度对比成功。

  ```bash
  test pass!
  ```

  **编译选项说明**

  | 参数 | 说明 | 可选值 | 默认值 |
  |------|------|---------|---------|
  | `RUN_MODE` | 运行模式：`npu`（NPU模式）、`cpu`（CPU调试模式）、`sim`（NPU仿真模式） | `npu`、`cpu`、`sim` | ``npu` |
  | `NPU_ARCH` | NPU硬件架构 | `dav-2201`、`dav-3510` | `dav-2201` |

  **NPU_ARCH 对应产品**

  | NPU_ARCH | 对应产品 |
  |----------|---------|
  | `dav-2201` | Atlas A2 训练/推理系列产品、Atlas A3 训练/推理系列产品 |
  | `dav-3510` | Ascend 950PR/Ascend 950DT |

  **多模式编译示例**

  - CPU调试模式：

    ```bash
    cmake -DRUN_MODE=cpu -DNPU_ARCH=dav-2201 ..;make -j
    ```

  - NPU仿真模式：

    ```bash
    cmake -DRUN_MODE=sim -DNPU_ARCH=dav-2201 ..;make -j
    ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。
