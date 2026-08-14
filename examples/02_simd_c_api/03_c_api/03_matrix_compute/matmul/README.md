# MatMul样例

## 概述

本样例演示如何通过Ascend C C API在单个Cube Core上实现固定规格的FP32矩阵乘加，计算公式为：

$$
C_{30 \times 70} = A_{30 \times 40} \times B_{40 \times 70} + bias_{1 \times 70}
$$

A矩阵和B矩阵均不转置。Host侧负责数据读写、内存管理、Kernel启动和同步，矩阵乘法及bias累加全部在Device侧完成。

## 支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## 目录结构

```text
├── matmul
│   ├── scripts
│   │   ├── gen_data.py             // 输入数据和真值数据生成脚本
│   │   └── verify_result.py        // 精度验证脚本
│   ├── CMakeLists.txt              // 编译工程文件
│   ├── data_utils.h                // 数据读写函数
│   ├── matmul.asc                  // Kernel实现和Host侧main函数
│   ├── README_en.md                // 英文样例说明文档
│   └── README.md                   // 样例说明文档
```

## 样例描述

输入和输出规格如下表所示。

| 参数 | 类型 | 数据类型 | Shape | 是否转置 |
|------|------|----------|-------|----------|
| `src0` | 输入 | float | `[30, 40]` | false |
| `src1` | 输入 | float | `[40, 70]` | false |
| `weight` | 输入 | float | `[1, 70]` | - |
| `dst` | 输出 | float | `[30, 70]` | - |

完整的数据处理流程如下：

1. 使用`asc_copy_gm2l1_nd2nz`和`asc_copy_gm2l1_dn2nz`将A矩阵、B矩阵从Global Memory搬运至L1 Buffer，并完成ND到Nz格式转换。
2. 使用`asc_copy_l12l0a`、`asc_copy_l12l0b`将A矩阵、B矩阵搬运至L0A Buffer和L0B Buffer。
3. 使用`asc_copy_l12bt`将bias从L1 Buffer搬运至BiasTable Buffer。
4. 使用`asc_mmad`完成矩阵乘加，结果写入L0C Buffer。
5. 使用`asc_copy_l0c2gm`将结果从L0C Buffer搬运至Global Memory，并完成Nz到ND格式转换。

Kernel固定启动1个Cube Core。A矩阵、B矩阵、bias和输出矩阵的Local Memory空间均按分形要求对齐，源码使用`static_assert`检查各级Buffer的容量。

## 精度测试用例

所有用例均使用固定Shape，输入数据范围为`[-100, 100]`。

| Case | 输入构造 | 验证目标 |
|------|----------|----------|
| P01 | A、B全零，bias为等差序列 | bias装载和广播 |
| P02 | A、B、bias全零 | 零值和输出初始化 |
| P03 | `-100`、`100`、`0`周期填充 | 输入边界和正负乘加 |
| P04 | 正负成对构造 | 消减场景 |
| P05 | B使用稀疏模式 | B矩阵布局和方向 |
| P06 | 固定随机种子20260804 | 随机精度 |
| P07 | 固定随机种子20260805 | 随机精度 |
| P08 | 固定随机种子20260806 | 随机精度 |

真值数据使用NumPy float64计算。当元素满足`abs(actual - golden) <= ATOL + RTOL * abs(golden)`时计为匹配，其中`RTOL = 2^-10`、`ATOL = 2^-16`，匹配比例不得低于0.99。此外，每个元素还必须满足绝对误差不超过`1e-2`或ULP误差不超过32。

## 编译运行

在本样例根目录下执行如下步骤，编译并运行样例。

- 配置环境变量和Python依赖

  ```bash
  source ${install_path}/cann/set_env.sh
  python3 -m pip install numpy
  ```

  > **说明：** `${install_path}`为CANN包安装目录，未指定时root用户默认安装至`/usr/local/Ascend`，非root用户默认安装至`$HOME/Ascend`。

- 编译样例

  ```bash
  cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCMAKE_ASC_RUN_MODE=npu
  cmake --build build -j
  ```

  使用NPU仿真模式时，将`CMAKE_ASC_RUN_MODE`设置为`sim`，并使用独立的构建目录：

  ```bash
  cmake -S . -B build_sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCMAKE_ASC_RUN_MODE=sim
  cmake --build build_sim -j
  ```

- 运行单个精度用例

  ```bash
  python3 scripts/gen_data.py --case P01 --output-dir input
  ./build/demo
  python3 scripts/verify_result.py output/output.bin input/golden.bin
  ```

- 运行全部精度用例

  ```bash
  for case_name in P01 P02 P03 P04 P05 P06 P07 P08; do
      python3 scripts/gen_data.py --case ${case_name} --output-dir input || exit 1
      ./build/demo || exit 1
      python3 scripts/verify_result.py output/output.bin input/golden.bin || exit 1
  done
  ```

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`sim` | NPU运行模式、NPU仿真模式 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510`（默认） | NPU架构，对应Ascend 950PR/Ascend 950DT |

- 执行结果

  精度验证成功时，输出中包含：

  ```text
  matched_ratio=1.000000
  status=PASSED
  ```
