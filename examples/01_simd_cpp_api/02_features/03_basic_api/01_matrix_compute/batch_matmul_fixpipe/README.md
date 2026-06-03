# Batch Matmul Fixpipe样例

## 概述

本样例介绍基于Tensor API验证带batch的L0C -> GM搬出能力。样例中输入A/B在GM上仍按batch存放，但kernel内每轮通过GM指针偏移构造2D Tensor，GM -> L1、L1 -> L0A/L0B和Mmad均按单个矩阵执行；Mmad结果写入L0C的batch slice后，最后通过一次`Copy(CopyL0C2GM)`将L0C上的3个batch一次性搬出到GM。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```text
├── batch_matmul_fixpipe
│   ├── scripts
│   │   ├── gen_data.py                 // 输入数据和真值数据生成脚本文件
│   │   └── verify_result.py            // 真值对比文件
│   ├── CMakeLists.txt                  // 编译工程文件
│   ├── data_utils.h                    // 数据读取写出函数
│   ├── batch_matmul_fixpipe.asc        // Ascend C样例实现与调用样例
│   ├── run.sh                          // 编译运行脚本
│   └── README.md                       // 样例说明文档
```

## 样例描述

- 样例功能：

  本样例用于验证L0C上携带batch信息时，Tensor API的L0C -> GM搬出路径是否能正确完成NZ -> ND转换并写入GM上的`[B, M, N]`连续空间。

  1. 输入A/B在GM上分别为`[B, M, K]`和`[B, K, N]`。
  2. kernel内每轮用`a + batchIndex * M * K`和`b + batchIndex * K * N`构造2D GM Tensor，避免依赖batch GM2L1能力。
  3. 每轮按2D路径完成GM -> L1、L1 -> L0A/L0B和Mmad。
  4. 每轮Mmad结果写入L0C的第`batchIndex`个batch slice。
  5. 所有batch计算完成后，调用一次`Copy(CopyL0C2GM)`，将L0C上的3个batch一次性搬出到GM。

- 样例规格：

  本样例默认使用`B=3, M=56, K=80, N=64`。M、K、N互不相同，且M不是16对齐规格；L0C按对齐后大小计算为`3 * 64 * 64 * sizeof(float) = 49152B`，小于256KB。

  <table border="2">
  <caption>表1：样例规格表</caption>
  <tr><td rowspan="1" align="left">样例类型(OpType)</td><td colspan="4" align="left">Batch Matmul Fixpipe</td></tr>
  <tr><td rowspan="2" align="left">样例输入</td><td align="left">name</td><td align="left">shape</td><td align="left">data type</td><td align="left">format</td></tr>
  <tr><td align="left">x1</td><td align="left">[3, 56, 80]</td><td align="left">half</td><td align="left">ND</td></tr>
  <tr><td align="left"></td><td align="left">x2</td><td align="left">[3, 80, 64]</td><td align="left">half</td><td align="left">ND</td></tr>
  <tr><td rowspan="1" align="left">样例输出</td><td align="left">output</td><td align="left">[3, 56, 64]</td><td align="left">float</td><td align="left">ND</td></tr>
  <tr><td rowspan="1" align="left">核函数名</td><td colspan="4" align="left">batch_matmul_fixpipe</td></tr>
  </table>

  `gen_data.py`中将每个batch的B矩阵构造为单位矩阵，因此输出真值等于对应batch的A矩阵。该数据构造便于检查batch维、M维、N维搬出顺序是否正确。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
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
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # NPU仿真模式
  python3 ../scripts/gen_data.py   # 生成测试输入数据
  ./demo                        # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
  ```

  如需NPU模式，示例如下：

  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=npu -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # NPU模式
  ```

  > **注意：** 切换编译模式前需清理cmake缓存，可在build目录下执行`rm CMakeCache.txt`后重新cmake。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  | :--- | :----- | :--- |
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`sim` | 运行模式：NPU运行、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510`（默认） | NPU架构：dav-3510 对应 Ascend 950PR/Ascend 950DT |

- 执行结果
  执行结果如下，说明精度对比成功。

  ```bash
  test pass!
  ```
