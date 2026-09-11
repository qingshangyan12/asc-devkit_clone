# compare样例

## 概述

本样例基于Memory矢量计算接口[asc_lt](../../../../../docs/zh/api/SIMD-API/c_api/vector_compute/vector_compare/asc_lt.md)、[asc_gt_scalar](../../../../../docs/zh/api/SIMD-API/c_api/vector_compute/vector_compare/asc_gt_scalar.md)和`asc_get_cmp_mask`，演示比较结果的小端bit压缩格式。

本样例支持三种比较场景，通过CMake编译参数`SCENARIO_NUM`选择。

<table>
<caption>表1：SCENARIO_NUM参数说明</caption>
<tr><td align="center">SCENARIO_NUM</td><td align="center">比较场景</td></tr>
<tr><td align="center">1</td><td align="center">向量比较</td></tr>
<tr><td align="center">2</td><td align="center">CmpMask读取</td></tr>
<tr><td align="center">3</td><td align="center">向量标量比较</td></tr>
</table>

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | >= CANN 9.2.0 |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | >= CANN 9.2.0 |

## 目录结构

```text
compare/
├── scripts/gen_data.py
├── CMakeLists.txt
├── data_utils.h
├── compare.asc
├── README.md
└── README_en.md
```

## 样例描述

本样例将两个各含64个float元素的输入向量从GM搬入UB后执行比较。比较结果中第`i`个元素存放在输出的第`i`个bit，按小端bit顺序组织为8B有效数据。核函数会搬出32B比较结果缓冲区，主机侧使用内核调用符`<<<>>>`调用`compare_kernel`，并逐字节校验前8B有效结果。

**场景1：向量比较**

- 样例功能：逐元素比较`x[i] < y[i]`，并直接输出压缩后的比较结果。
- 样例规格：

  <table>
  <caption>表2：场景1样例规格</caption>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">compare</td></tr>

  <tr><td rowspan="4" align="center">样例输入</td></tr>
  <tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x</td><td align="center">[1, 64]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">y</td><td align="center">[1, 64]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="2" align="center">样例输出</td></tr>
  <tr><td align="center">output</td><td align="center">[1, 8]</td><td align="center">uint8_t</td><td align="center">ND</td></tr>

  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">compare_kernel</td></tr>
  </table>

- 样例实现：
  - 搬入：调用`asc_copy_gm2ub_align`将`x`和`y`从GM搬入UB。
  - 计算：调用带输出参数的`asc_lt`完成向量比较，并将压缩结果写入UB输出缓冲区。
  - 搬出：调用`asc_copy_ub2gm_align`将32B输出缓冲区搬回GM，主机侧校验前8B有效数据。

**场景2：CmpMask读取**

- 样例功能：逐元素比较`x[i] < y[i]`，并通过CmpMask读取比较结果。
- 样例规格：

  <table>
  <caption>表3：场景2样例规格</caption>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">compare</td></tr>

  <tr><td rowspan="4" align="center">样例输入</td></tr>
  <tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x</td><td align="center">[1, 64]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">y</td><td align="center">[1, 64]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="2" align="center">样例输出</td></tr>
  <tr><td align="center">output</td><td align="center">[1, 8]</td><td align="center">uint8_t</td><td align="center">ND</td></tr>

  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">compare_kernel</td></tr>
  </table>

- 样例实现：
  - 搬入：调用`asc_copy_gm2ub_align`将`x`和`y`从GM搬入UB。
  - 计算：调用不带输出参数的`asc_lt`生成CmpMask，再调用`asc_get_cmp_mask`将CmpMask写入UB输出缓冲区。
  - 搬出：调用`asc_copy_ub2gm_align`将32B输出缓冲区搬回GM，主机侧校验前8B有效数据。

**场景3：向量标量比较**

- 样例功能：逐元素比较`x[i] > y[0]`，其中`y[0]`作为标量。
- 样例规格：

  <table>
  <caption>表4：场景3样例规格</caption>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">compare</td></tr>

  <tr><td rowspan="4" align="center">样例输入</td></tr>
  <tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x</td><td align="center">[1, 64]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">y</td><td align="center">[1, 64]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="2" align="center">样例输出</td></tr>
  <tr><td align="center">output</td><td align="center">[1, 8]</td><td align="center">uint8_t</td><td align="center">ND</td></tr>

  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">compare_kernel</td></tr>
  </table>

- 样例实现：
  - 搬入：调用`asc_copy_gm2ub_align`将`x`和`y`从GM搬入UB。
  - 计算：调用`asc_gt_scalar`比较`x[i]`与标量`y[0]`，并将压缩结果写入UB输出缓冲区。
  - 搬出：调用`asc_copy_ub2gm_align`将32B输出缓冲区搬回GM，主机侧校验前8B有效数据。

数据生成脚本为各场景生成确定性输入和相应的8B真值数据。

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

程序会对关键ACL调用失败立即返回非零，不会把未执行内核的输出当作通过结果。

- 编译选项说明

| 选项 | 可选值 | 说明 |
|------|--------|------|
| `CMAKE_ASC_ARCHITECTURES` | `dav-2201`（默认） | NPU 架构：dav-2201对应Atlas A2 训练系列产品/Atlas A2 推理系列产品和Atlas A3 训练系列产品/Atlas A3 推理系列产品 |
| `SCENARIO_NUM` | `1`（默认）、`2`、`3` | 样例场景：向量比较、CmpMask读取、向量标量比较 |

- 执行结果

输出与golden数据比较通过时，打印如下信息：

```text
test pass!
```
