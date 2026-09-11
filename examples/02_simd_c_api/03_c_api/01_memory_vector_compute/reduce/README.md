# reduce样例

## 概述

本样例基于Memory矢量计算接口[asc_repeat_reduce_sum](../../../../../docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_sum.md)和[asc_datablock_reduce_sum](../../../../../docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_datablock_reduce_sum.md)实现float数据的求和归约。

本样例支持两种求和归约场景，通过CMake编译参数`SCENARIO_NUM`选择。

<table>
<caption>表1：SCENARIO_NUM参数说明</caption>
<tr><td align="center">SCENARIO_NUM</td><td align="center">求和归约场景</td></tr>
<tr><td align="center">1</td><td align="center">Repeat归约</td></tr>
<tr><td align="center">2</td><td align="center">DataBlock归约</td></tr>
</table>

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | >= CANN 9.2.0 |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | >= CANN 9.2.0 |

## 目录结构

```text
reduce/
├── scripts/gen_data.py
├── CMakeLists.txt
├── data_utils.h
├── reduce.asc
├── README.md
└── README_en.md
```

## 样例描述

本样例将64个float输入元素从GM搬入UB后进行求和归约，再将结果搬回GM。主机侧使用内核调用符`<<<>>>`调用`reduce_kernel`，按`1e-4`绝对误差比对全部有效输出与`output/golden.bin`。

**场景1：Repeat归约**

- 样例功能：对一个repeat内的64个float元素求和。
- 样例规格：

  <table>
  <caption>表2：场景1样例规格</caption>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">reduce</td></tr>

  <tr><td rowspan="3" align="center">样例输入</td></tr>
  <tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x</td><td align="center">[1, 64]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="2" align="center">样例输出</td></tr>
  <tr><td align="center">y</td><td align="center">[1, 1]</td><td align="center">float</td><td align="center">ND</td></tr>

  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">reduce_kernel</td></tr>
  </table>

- 样例实现：
  - 搬入：调用`asc_copy_gm2ub_align`将64个float元素从GM搬入UB。
  - 计算：调用`asc_repeat_reduce_sum`对一个repeat内的64个元素求和。
  - 搬出：调用`asc_copy_ub2gm_align`将1个float结果从UB搬回GM。

**场景2：DataBlock归约**

- 样例功能：分别对8个32BDataBlock内的8个float元素求和。
- 样例规格：

  <table>
  <caption>表3：场景2样例规格</caption>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">reduce</td></tr>

  <tr><td rowspan="3" align="center">样例输入</td></tr>
  <tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x</td><td align="center">[1, 64]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="2" align="center">样例输出</td></tr>
  <tr><td align="center">y</td><td align="center">[1, 8]</td><td align="center">float</td><td align="center">ND</td></tr>

  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">reduce_kernel</td></tr>
  </table>

- 样例实现：
  - 搬入：调用`asc_copy_gm2ub_align`将64个float元素从GM搬入UB。
  - 计算：调用`asc_datablock_reduce_sum`对每个32BDataBlock内的8个float元素求和。
  - 搬出：调用`asc_copy_ub2gm_align`将8个float结果从UB搬回GM。

数据生成脚本为各场景生成确定性float输入和对应golden数据。

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
SCENARIO_NUM=1                                                                                  # 执行场景1
mkdir -p build && cd build;                                                                       # 创建并进入build目录
cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # 编译工程（默认NPU模式）
python3 ../scripts/gen_data.py -scenario_num=$SCENARIO_NUM                                       # 生成测试输入数据和真值数据
./demo                                                                                           # 执行编译生成的可执行程序，执行样例
```

- 编译选项说明

| 选项 | 可选值 | 说明 |
|------|--------|------|
| `CMAKE_ASC_ARCHITECTURES` | `dav-2201`（默认） | NPU 架构：dav-2201对应Atlas A2 训练系列产品/Atlas A2 推理系列产品和Atlas A3 训练系列产品/Atlas A3 推理系列产品 |
| `SCENARIO_NUM` | `1`（默认）、`2` | 样例场景：Repeat归约、DataBlock归约 |

- 执行结果

输出与golden数据比较通过时，打印如下信息：

```text
test pass!
```
