# fused_compute样例

## 概述

本样例基于Memory矢量计算接口[asc_leakyrelu](../../../../../docs/zh/api/SIMD-API/c_api/vector_compute/vector_fused/asc_leakyrelu.md)实现Leaky ReLU：当`x > 0`时`y = x`，否则`y = 0.01 * x`。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | >= CANN 9.2.0 |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | >= CANN 9.2.0 |

## 目录结构

```text
fused_compute/
├── scripts/gen_data.py
├── CMakeLists.txt
├── data_utils.h
├── element_wise_arithmetic.asc
├── README.md
└── README_en.md
```

## 样例描述

本样例包含一个Leaky ReLU场景。核函数将512个half输入元素从GM搬入UB，完成逐元素计算后将结果搬回GM。主机侧使用内核调用符`<<<>>>`调用`element_wise_arithmetic_kernel`，并逐元素比对输出与`output/golden.bin`。

**场景：Leaky ReLU**

- 样例功能：计算`y = x`（`x > 0`）或`y = 0.01 * x`（`x <= 0`）。
- 样例规格：

  <table>
  <caption>表1：样例规格</caption>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">fused_compute</td></tr>

  <tr><td rowspan="3" align="center">样例输入</td></tr>
  <tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x</td><td align="center">[1, 512]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td rowspan="2" align="center">样例输出</td></tr>
  <tr><td align="center">y</td><td align="center">[1, 512]</td><td align="center">half</td><td align="center">ND</td></tr>

  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">element_wise_arithmetic_kernel</td></tr>
  </table>

- 样例实现：
  - 搬入：调用`asc_copy_gm2ub_align`将512个half元素从GM搬入UB。
  - 计算：调用`asc_leakyrelu`，斜率参数为half类型的`0.01`。
  - 搬出：调用`asc_copy_ub2gm_align`将512个half结果从UB搬回GM。

数据生成脚本生成包含正数、负数和零的确定性half输入及对应golden数据。

## 编译运行

在本样例根目录执行以下步骤。

- 配置环境变量

  请根据CANN开发套件包的[安装方式](../../../../../docs/zh/quick_start.md#prepare&install)配置环境变量。

```bash
source ${install_path}/cann/set_env.sh
```

  > **说明：** `${install_path}`为CANN包安装目录；root用户默认为`/usr/local/Ascend`，非root用户默认为`${HOME}/Ascend`。

- NPU运行

```bash
mkdir -p build && cd build;                                                                       # 创建并进入build目录
cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # 编译工程（默认NPU模式）
python3 ../scripts/gen_data.py                                                                    # 生成测试输入数据和真值数据
./demo                                                                                           # 执行编译生成的可执行程序，执行样例
```

- 编译选项说明

| 选项 | 可选值 | 说明 |
|------|--------|------|
| `CMAKE_ASC_ARCHITECTURES` | `dav-2201`（默认） | NPU 架构：dav-2201对应Atlas A2 训练系列产品/Atlas A2 推理系列产品和Atlas A3 训练系列产品/Atlas A3 推理系列产品 |

- 执行结果

输出与golden数据比较通过时，打印如下信息：

```text
test pass!
```
