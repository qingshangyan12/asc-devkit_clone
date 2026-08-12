# Reg Compute基础与复合计算接口直调样例（Tensor API）

## 概述

本样例基于Reg Compute返回值风格接口，实现Vector Compute手册“基础计算”和“复合计算”章节中的寄存器计算功能。除原有基础计算外，样例新增验证`Axpy`、`AbsSub`、`ExpSub`、`MulDstAdd`、`MulAddDst`和`MulsCast`六个复合计算接口。

本样例通过Ascend C编程语言实现Reg Compute基础计算，使用`<<<>>>`内核调用符完成算子核函数在NPU侧运行验证的基础流程，给出了对应的端到端实现。Kernel使用Tensor API完成GM与UB之间的数据搬运，通过`local_tensor::load`加载`reg_tensor`，执行寄存器计算后使用`local_tensor::store`写回UB。

## 支持的产品

- Ascend 950PR/Ascend 950DT

> **说明：** 本样例依赖本仓新增的Reg Compute接口。请先使用包含该接口的asc-devkit源码构建并安装配套开发套件包，不能直接使用尚未包含该接口的标准CANN 9.2.0头文件。

## 目录结构介绍

```text
├── reg_compute_tensor_api
│   ├── scripts
│   │   ├── gen_data.py             // 输入数据和真值数据生成脚本
│   │   └── verify_result.py        // 输出数据和真值数据校验脚本
│   ├── CMakeLists.txt              // 编译工程文件
│   ├── data_utils.h                // 数据读入和写出函数
│   ├── README.md                   // 中文说明文档
│   ├── README_En.md                // 英文说明文档
│   └── reg_compute_tensor_api.asc  // Ascend C算子实现和调用样例
```

执行数据生成脚本后会在`build`目录下生成`input`和`output`目录，其中的二进制文件无需提交。

## 算子描述

- 算子功能：

  本样例分别对64个`float`元素、64个`int32_t`元素和32个`int64_t`元素执行Reg Compute计算。浮点计算生成44组结果，`int32_t`计算生成14组结果，`int64_t`计算生成3组结果，验证脚本将所有输出与NumPy生成的真值数据逐元素比较。

- 算子规格：

  本样例中实现的是固定Shape的Reg Compute基础计算算子。

  <table>
  <tr><td rowspan="1" align="center">算子类型（OpType）</td><td colspan="5" align="center">Reg Compute</td></tr>
  <tr><td rowspan="9" align="center">算子输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td><td align="center">default</td></tr>
  <tr><td align="center">float_a</td><td align="center">[64]</td><td align="center">float</td><td align="center">ND</td><td align="center">\</td></tr>
  <tr><td align="center">float_b</td><td align="center">[64]</td><td align="center">float</td><td align="center">ND</td><td align="center">\</td></tr>
  <tr><td align="center">float_slope</td><td align="center">[64]</td><td align="center">float</td><td align="center">ND</td><td align="center">\</td></tr>
  <tr><td align="center">int_a</td><td align="center">[64]</td><td align="center">int32_t</td><td align="center">ND</td><td align="center">\</td></tr>
  <tr><td align="center">int_b</td><td align="center">[64]</td><td align="center">int32_t</td><td align="center">ND</td><td align="center">\</td></tr>
  <tr><td align="center">int_shift</td><td align="center">[64]</td><td align="center">int32_t</td><td align="center">ND</td><td align="center">\</td></tr>
  <tr><td align="center">int64_a</td><td align="center">[32]</td><td align="center">int64_t</td><td align="center">ND</td><td align="center">\</td></tr>
  <tr><td align="center">int64_b</td><td align="center">[32]</td><td align="center">int64_t</td><td align="center">ND</td><td align="center">\</td></tr>
  <tr><td rowspan="4" align="center">算子输出</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td><td align="center">default</td></tr>
  <tr><td align="center">float_output</td><td align="center">[44, 64]</td><td align="center">float</td><td align="center">ND</td><td align="center">\</td></tr>
  <tr><td align="center">int_output</td><td align="center">[14, 64]</td><td align="center">int32_t</td><td align="center">ND</td><td align="center">\</td></tr>
  <tr><td align="center">int64_output</td><td align="center">[3, 32]</td><td align="center">int64_t</td><td align="center">ND</td><td align="center">\</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="5" align="center">reg_compute_tensor_api</td></tr>
  </table>

- 算子实现：

  - Kernel实现

    Kernel实现流程分为创建GM/UB Tensor、CopyIn、Compute和CopyOut四个步骤。CopyIn将输入Tensor从Global Memory搬运至Unified Buffer，Compute通过`ComputeFloat`和`ComputeInt`完成Reg Compute计算，CopyOut将计算结果从Unified Buffer搬运至Global Memory。

    `float`和`int32_t`在dav-3510架构上的寄存器宽度均为64个元素。样例使用`ones<T>()`生成全lane mask，并通过`with_mask`将mask附加到加载结果；第37行额外使用`a > 0.0f`生成部分mask，验证计算和存储过程中的mask传播。

    `float_output`各行的接口调用顺序如下。

    | 行 | 接口调用 |
    | :--- | :--- |
    | 0至7 | `abs(a)`、`exp(a)`、`sqrt(positive)`、`log(positive)`、`log2(positive)`、`log10(positive)`、`-a`、`relu(a)` |
    | 8至12 | `prelu(a, slope)`、`prelu(a, 0.2f)`、`prelu(a)`、`leaky_relu(a, 0.1f)`、`leaky_relu(a)` |
    | 13至15 | `a + b`、`a + 1.0f`、`1.0f + a` |
    | 16至18 | `a - b`、`a - 1.0f`、`1.0f - a` |
    | 19至21 | `a * b`、`a * 2.0f`、`2.0f * a` |
    | 22至24 | `a / b`、`a / 2.0f`、`2.0f / positive` |
    | 25至27 | `max(a, b)`、`max(a, 0.0f)`、`max(0.0f, a)` |
    | 28至30 | `min(a, b)`、`min(a, 0.0f)`、`min(0.0f, a)` |
    | 31至36 | `a == b`、`a != 0.0f`、`0.0f < a`、`a <= 0.0f`、`0.0f > a`、`a >= b` |
    | 37 | 对`a > 0.0f`对应lane执行`a + 1.0f`，其余lane保持为`0.0f` |
    | 38至43 | `axpy(b, a, 0.25f)`、`abs_sub(a, b)`、`exp_sub(a, b)`、`mul_dst_add(a, b, slope)`、`mul_add_dst(a, b, slope)`、`muls_cast(a, 1.5f)` |

    `int_output`各行的接口调用顺序如下。

    | 行 | 接口调用 |
    | :--- | :--- |
    | 0 | `!a` |
    | 1至3 | `a & b`、`a & 15`、`15 & a` |
    | 4至6 | `a \| b`、`a \| 16`、`16 \| a` |
    | 7至9 | `a ^ b`、`a ^ 7`、`7 ^ a` |
    | 10至11 | `a << shift`、`a << 2` |
    | 12至13 | `a >> shift`、`a >> 2` |

    `int64_output`的3行依次调用`axpy(b, a, -3)`、`abs_sub(a, b)`和`mul_add_dst(b, a, b)`，用于验证复合接口的64位软件模拟路径。由于当前Tensor API搬运接口不直接支持`int64_t`，样例按两个连续`uint32_t`字搬运每个64位元素，寄存器计算前后保持位级重解释，Host侧仍以`int64_t`逐元素校验。

    比较接口返回`reg_tensor<bool>`。样例先写入全0寄存器，再以比较结果作为mask写入全1寄存器，将谓词结果转换为`0.0f`或`1.0f`，用于Host侧验证。

  - 调用实现

    使用内核调用符`<<<>>>`调用核函数。核函数执行完成后，Host侧将结果写入`float_output.bin`和`int_output.bin`，`verify_result.py`负责与`golden_float.bin`和`golden_int.bin`进行精度校验。数学函数、`ExpSub`和`MulsCast`采用绝对误差和相对误差均为`5e-3`的组合容差，其余浮点计算采用`1e-4`，整数和比较结果要求精确一致。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行算子。

- 配置环境变量

  首先根据[安装方式](https://gitcode.com/cann/asc-devkit/blob/master/docs/zh/quick_start.md#prepare&install)，构建并安装包含本接口的asc-devkit开发套件包，然后选择对应的环境变量配置命令。

  - 默认路径，root用户安装CANN软件包

    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - 默认路径，非root用户安装CANN软件包

    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - 指定路径`install_path`安装CANN软件包

    ```bash
    source ${install_path}/cann/set_env.sh
    ```

- 样例执行

  ```bash
  mkdir -p build && cd build
  cmake .. -DCMAKE_ASC_ARCHITECTURES=dav-3510
  cmake --build . -j
  python3 ../scripts/gen_data.py
  ./demo
  python3 ../scripts/verify_result.py \
    output/float_output.bin output/golden_float.bin \
    output/int_output.bin output/golden_int.bin \
    output/int64_output.bin output/golden_int64.bin
  ```

  验证脚本输出如下，说明所有接口计算结果均通过精度校验。

  ```text
  Float results passed, max absolute error: <value>
  Int32 results passed.
  Int64 results passed.
  test pass!
  ```

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  | :--- | :--- | :--- |
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认） | NPU运行模式 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510`（默认） | NPU架构，对应Ascend 950PR/Ascend 950DT |
