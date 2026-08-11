# fixpipe_l0c2gm样例

## 概述

本样例介绍如何使用Fixpipe将矩阵乘的结果从L0C Buffer搬出到GM（Global Memory），支持多种输出格式（Nz、ND、DN）、数据类型转换、随路量化、ReLU以及ChannelSplit等功能。这些接口用于将L0C中的矩阵乘计算结果高效地传输到全局内存，并支持各种数据格式转换和预处理能力。

针对Ascend 950PR/Ascend 950DT（dav-3510架构），本样例覆盖了Fixpipe L0C到GM通路支持的所有数据类型组合，包括float和int32_t两种L0C源数据类型，以及int8_t、uint8_t、half、bfloat16_t、fp8_e4m3fn_t、hifloat8_t、float、int32_t等输出数据类型，每种组合支持NoQuant、Cast、Scalar量化、Tensor量化等模式，并分别测试ND和NZ输出格式，共计59个场景。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | >= CANN 9.0.0 |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | >= CANN 9.0.0 |

## 目录结构介绍

```
├── fixpipe_l0c2gm
│   ├── figures                        // 图示
│   ├── scripts
│   │   ├── gen_data.py                // 输入数据和真值数据生成脚本
│   │   ├── gen_data_f322s8.py         // float→int8 量化参考脚本（独立）
│   │   ├── gen_data_s322s8.py         // int32→int8 量化参考脚本（独立）
│   │   ├── gen_data_s322f16.py        // int32→half 量化参考脚本（独立）
│   │   └── verify_result.py           // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt                 // 编译工程文件
│   ├── data_utils.h                   // 数据读入写出函数
│   ├── fixpipe_l0c2gm.asc             // Ascend C样例实现 & 调用样例
│   └── README.md                      // 样例说明文档
```

## FixpipeParamsV220 与 FixpipeParamsArch3510 结构体对比

不同产品支持的参数结构体不同：
- **Ascend 950PR/Ascend 950DT**：支持 `FixpipeParamsV220` 和 `FixpipeParamsArch3510` 两种参数结构体，推荐使用 `FixpipeParamsArch3510`
- **Atlas A3 训练/推理系列、Atlas A2 训练/推理系列**：仅支持 `FixpipeParamsV220`

本样例通过编译参数 `CMAKE_ASC_ARCHITECTURES` 选择不同架构，根据架构自动选择对应的参数结构体：
- `dav-2201` 架构：使用 `FixpipeParamsV220`
- `dav-3510` 架构：使用 `FixpipeParamsArch3510`

<a name="表1"></a>
<table border="2" align="center">
<caption style="font-weight: normal;">
    <span style="font-weight: bold; font-size: 1.2em;">📌 表1：参数结构体对比</span></caption>
<tr><td rowspan="1" align="center">成员名</td><td align="center">FixpipeParamsV220</td><td align="center">FixpipeParamsArch3510</td><td align="center">说明</td></tr>
<tr><td align="center"><code>nSize</code></td><td align="center">✅</td><td align="center">✅</td><td>输出矩阵在N方向上的大小</td></tr>
<tr><td align="center"><code>mSize</code></td><td align="center">✅</td><td align="center">✅</td><td>输出矩阵在M方向上的大小</td></tr>
<tr><td align="center"><code>srcStride</code></td><td align="center">✅</td><td align="center">✅</td><td>源Nz矩阵中相邻Z排布的起始地址偏移</td></tr>
<tr><td align="center"><code>dstStride</code></td><td align="center">✅</td><td align="center">✅</td><td>目的矩阵中相邻Z排布的起始地址偏移（Nz格式）或每行元素个数（ND/DN格式）</td></tr>
<tr><td align="center"><code>quantPre</code></td><td align="center">✅</td><td align="center">✅</td><td>量化模式控制</td></tr>
<tr><td align="center"><code>deqScalar</code></td><td align="center">✅</td><td align="center">✅</td><td>scalar量化参数</td></tr>
<tr><td align="center"><code>reluEn</code></td><td align="center">✅</td><td align="center">✅</td><td>ReLU开关</td></tr>
<tr><td align="center"><code>unitFlag</code></td><td align="center">✅</td><td align="center">✅</td><td>Mmad与Fixpipe细粒度并行控制</td></tr>
<tr><td align="center"><code>isChannelSplit</code></td><td align="center">✅</td><td align="center">✅</td><td>通道拆分开关</td></tr>
<tr><td align="center"><code>ndNum</code> / <code>srcNdStride</code> / <code>dstNdStride</code></td><td align="center">✅</td><td align="center">✅（在<code>TransformParams</code>中）</td><td>NZ2ND场景下控制多矩阵传输的参数，在V220中为独立成员，在Arch3510中被整合到<code>TransformParams</code>结构体中</td></tr>
<tr><td align="center"><code>dnNum</code> / <code>srcNzMatrixStride</code> / <code>dstDnMatrixStride</code> / <code>srcNzC0Stride</code></td><td align="center">❌</td><td align="center">✅（在<code>TransformParams</code>中）</td><td>NZ2DN场景下控制多矩阵传输的参数，仅Arch3510支持</td></tr>
<tr><td align="center"><code>TransformParams</code></td><td align="center">❌</td><td align="center">✅</td><td>基于模板参数的类型选择器，根据CO2Layout自动选择参数类型</td></tr>
<tr><td align="center"><code>dualDstCtrl</code></td><td align="center">❌</td><td align="center">✅</td><td>双目标模式控制，支持M维度拆分或N维度拆分</td></tr>
<tr><td align="center"><code>subBlockId</code></td><td align="center">❌</td><td align="center">✅</td><td>单目标模式下指示目标UB的编号</td></tr>
</table>

## 场景详细说明

本样例通过编译参数 `SCENARIO_NUM` 选择不同的输出场景，SCENARIO_NUM不同取值对应的含义如下表所示。所有场景基于相同的矩阵乘规格：[M, N, K] = [128, 256, 128]，核函数名为 `fixpipe_l0c2gm`。

- 场景1-7支持 `dav-2201` 和 `dav-3510` 架构（场景3仅 `dav-3510`）
- 场景8-59仅支持 `dav-3510` 架构（使用 `FixpipeParamsArch3510` 特有的数据类型和量化模式）

<a name="表2"></a>
<table border="2" align="center">
<caption style="font-weight: normal;">
    <span style="font-weight: bold; font-size: 1.2em;">📌 表2：scenarioNum不同取值的含义</span></caption>
<tr><td align="center">scenarioNum</td><td align="center">输入类型</td><td align="center">L0C数据类型</td><td align="center">输出数据类型</td><td align="center">quantPre</td><td align="center">量化方式</td><td align="center">输出格式</td><td align="center">说明</td></tr>
<tr><td align="center">1</td><td align="center">half</td><td align="center">float</td><td align="center">float</td><td align="center">NoQuant</td><td align="center">-</td><td align="center">Nz</td><td align="center">基本NZ输出</td></tr>
<tr><td align="center">2</td><td align="center">half</td><td align="center">float</td><td align="center">float</td><td align="center">NoQuant</td><td align="center">-</td><td align="center">ND</td><td align="center">基本ND输出</td></tr>
<tr><td align="center">3</td><td align="center">half</td><td align="center">float</td><td align="center">float</td><td align="center">NoQuant</td><td align="center">-</td><td align="center">DN</td><td align="center">基本DN输出（仅3510）</td></tr>
<tr><td align="center">4</td><td align="center">half</td><td align="center">float</td><td align="center">int8_t</td><td align="center">QF322B8_PRE</td><td align="center">scalar</td><td align="center">ND</td><td align="center">float→int8 量化</td></tr>
<tr><td align="center">5</td><td align="center">half</td><td align="center">float</td><td align="center">int8_t</td><td align="center">VQF322B8_PRE</td><td align="center">tensor</td><td align="center">ND</td><td align="center">float→int8 量化</td></tr>
<tr><td align="center">6</td><td align="center">half</td><td align="center">float</td><td align="center">float</td><td align="center">NoQuant</td><td align="center">-</td><td align="center">ND</td><td align="center">使能ReLU</td></tr>
<tr><td align="center">7</td><td align="center">half</td><td align="center">float</td><td align="center">float</td><td align="center">NoQuant</td><td align="center">-</td><td align="center">Nz</td><td align="center">使能ChannelSplit</td></tr>
<tr><td align="center">8-9</td><td align="center">half</td><td align="center">float</td><td align="center">int8_t</td><td align="center">QF/VQF322B8_PRE</td><td align="center">scalar/tensor</td><td align="center">Nz</td><td align="center">float→int8 NZ输出</td></tr>
<tr><td align="center">10-13</td><td align="center">half</td><td align="center">float</td><td align="center">uint8_t</td><td align="center">QF/VQF322B8_PRE</td><td align="center">scalar/tensor</td><td align="center">ND/Nz</td><td align="center">float→uint8 量化</td></tr>
<tr><td align="center">14-19</td><td align="center">half</td><td align="center">float</td><td align="center">half</td><td align="center">F322F16 / QF/VQF322F16_PRE</td><td align="center">cast/scalar/tensor</td><td align="center">ND/Nz</td><td align="center">float→half 转换/量化</td></tr>
<tr><td align="center">20-25</td><td align="center">half</td><td align="center">float</td><td align="center">bfloat16_t</td><td align="center">F322BF16 / QF/VQF322BF16_PRE</td><td align="center">cast/scalar/tensor</td><td align="center">ND/Nz</td><td align="center">float→bf16 转换/量化</td></tr>
<tr><td align="center">26-29</td><td align="center">half</td><td align="center">float</td><td align="center">fp8_e4m3fn_t</td><td align="center">QF/VQF322FP8_PRE</td><td align="center">scalar/tensor</td><td align="center">ND/Nz</td><td align="center">float→fp8 量化</td></tr>
<tr><td align="center">30-37</td><td align="center">half</td><td align="center">float</td><td align="center">hifloat8_t</td><td align="center">QF/VQF322HIF8_PRE(_HYBRID)</td><td align="center">scalar/tensor</td><td align="center">ND/Nz</td><td align="center">float→hif8 量化（含HYBRID）</td></tr>
<tr><td align="center">38-41</td><td align="center">half</td><td align="center">float</td><td align="center">float</td><td align="center">QF/VQF322F32_PRE</td><td align="center">scalar/tensor</td><td align="center">ND/Nz</td><td align="center">float→float 量化</td></tr>
<tr><td align="center">42-43</td><td align="center">int8_t</td><td align="center">int32_t</td><td align="center">int32_t</td><td align="center">NoQuant</td><td align="center">-</td><td align="center">ND/Nz</td><td align="center">int32→int32 直接输出</td></tr>
<tr><td align="center">44-47</td><td align="center">int8_t</td><td align="center">int32_t</td><td align="center">int8_t</td><td align="center">REQ8 / VREQ8</td><td align="center">scalar/tensor</td><td align="center">ND/Nz</td><td align="center">int32→int8 重量化</td></tr>
<tr><td align="center">48-51</td><td align="center">int8_t</td><td align="center">int32_t</td><td align="center">uint8_t</td><td align="center">REQ8 / VREQ8</td><td align="center">scalar/tensor</td><td align="center">ND/Nz</td><td align="center">int32→uint8 重量化</td></tr>
<tr><td align="center">52-55</td><td align="center">int8_t</td><td align="center">int32_t</td><td align="center">half</td><td align="center">DEQF16 / VDEQF16</td><td align="center">scalar/tensor</td><td align="center">ND/Nz</td><td align="center">int32→half 反量化</td></tr>
<tr><td align="center">56-59</td><td align="center">int8_t</td><td align="center">int32_t</td><td align="center">bfloat16_t</td><td align="center">QS/VQS322BF16_PRE</td><td align="center">scalar/tensor</td><td align="center">ND/Nz</td><td align="center">int32→bf16 量化</td></tr>
</table>

### 数据类型组合汇总

针对Ascend 950PR/Ascend 950DT（dav-3510架构），Fixpipe L0C→GM通路支持的完整数据类型组合如下：

| L0C源数据类型 | 输入A/B类型 | 输出数据类型 | quantPre模式 | 场景编号 |
|-------------|-----------|-----------|-------------|---------|
| float | half | float | NoQuant / QF322F32_PRE / VQF322F32_PRE | 1-3, 6-7, 38-41 |
| float | half | int8_t | QF322B8_PRE / VQF322B8_PRE | 4-5, 8-9 |
| float | half | uint8_t | QF322B8_PRE / VQF322B8_PRE | 10-13 |
| float | half | half | F322F16 / QF322F16_PRE / VQF322F16_PRE | 14-19 |
| float | half | bfloat16_t | F322BF16 / QF322BF16_PRE / VQF322BF16_PRE | 20-25 |
| float | half | fp8_e4m3fn_t | QF322FP8_PRE / VQF322FP8_PRE | 26-29 |
| float | half | hifloat8_t | QF322HIF8_PRE / VQF322HIF8_PRE_HYBRID / V变体 | 30-37 |
| int32_t | int8_t | int32_t | NoQuant | 42-43 |
| int32_t | int8_t | int8_t | REQ8 / VREQ8 | 44-47 |
| int32_t | int8_t | uint8_t | REQ8 / VREQ8 | 48-51 |
| int32_t | int8_t | half | DEQF16 / VDEQF16 | 52-55 |
| int32_t | int8_t | bfloat16_t | QS322BF16_PRE / VQS322BF16_PRE | 56-59 |

### quantPre量化模式说明

| 量化方式 | quantPre模式 | 说明 | 是否需要cbufWorkspace |
|---------|-------------|------|---------------------|
| 不量化 | NoQuant | 直接输出，无量化 | 否 |
| Cast | F322F16 / F322BF16 | float→half/bfloat16_t 类型转换（CAST_RINT） | 否 |
| Scalar量化 | QF322B8_PRE / QF322F16_PRE / QF322BF16_PRE / QF322FP8_PRE / QF322HIF8_PRE / QF322HIF8_PRE_HYBRID / QF322F32_PRE / DEQF16 / REQ8 / QS322BF16_PRE | 整个矩阵使用一个量化参数（deqScalar） | 否 |
| Tensor量化 | VQF322B8_PRE / VQF322F16_PRE / VQF322BF16_PRE / VQF322FP8_PRE / VQF322HIF8_PRE / VQF322HIF8_PRE_HYBRID / VQF322F32_PRE / VDEQF16 / VREQ8 / VQS322BF16_PRE | 每列使用独立量化参数（cbufWorkspace） | 是 |

### 场景1-7详细说明

**场景1：输出格式Nz，输出数据类型float**
- 输入：A [128, 128] half类型，ND格式；B [128, 256] half类型，ND格式
- 输出：C [128, 256] float类型，Nz格式
- 实现：使用 `Fixpipe<outputType, l0cType, AscendC::CFG_NZ>` 将数据从L0C搬出到GM，输出为Nz格式
- 说明：L0C数据为Nz格式直接输出到GM的Nz格式，数据保持原格式不变
<p align="center">
  <img src="figures/fixpipe_l0c2gm_NZ2NZ.png" width="800">
</p>

**场景2：输出格式ND，输出数据类型float**
- 输入：A [128, 128] half类型，ND格式；B [128, 256] half类型，ND格式
- 输出：C [128, 256] float类型，ND格式
- 实现：使用 `Fixpipe<outputType, l0cType, AscendC::CFG_ROW_MAJOR>` 指定ROW_MAJOR格式转换
- 说明：将CO1中的Nz格式数据转换为ND格式输出到GM，ND格式没有Nz格式的对齐要求，输出时需要按实际大小配置参数
<p align="center">
  <img src="figures/fixpipe_l0c2gm_NZ2ND.png" width="800">
</p>

**场景3：输出格式DN，输出数据类型float（仅Ascend 950PR/Ascend 950DT支持）**
- 输入：A [128, 128] half类型，ND格式；B [128, 256] half类型，ND格式
- 输出：C [256, 128] float类型，DN格式
- 实现：使用 `Fixpipe<outputType, l0cType, AscendC::CFG_COLUMN_MAJOR>` 指定COLUMN_MAJOR格式转换
- 说明：将CO1中的Nz格式数据转换为DN格式输出到GM
<p align="center">
  <img src="figures/fixpipe_l0c2gm_NZ2DN.png" width="800">
</p>

**场景4：输出格式ND，输出数据类型int8_t，使能Scalar量化**
- 输入：A [128, 128] half类型，ND格式；B [128, 256] half类型，ND格式
- 输出：C [128, 256] int8_t类型，ND格式
- 实现：设置 `fixpipeParams.quantPre = QuantMode_t::QF322B8_PRE`，使用Scalar量化模式
- 说明：将float类型数据量化为int8_t类型，整个C矩阵使用一个量化参数

**场景5：输出格式ND，输出数据类型int8_t，使能Vector量化**
- 输入：A [128, 128] half类型，ND格式；B [128, 256] half类型，ND格式
- 输出：C [128, 256] int8_t类型，ND格式
- 实现：设置 `fixpipeParams.quantPre = QuantMode_t::VQF322B8_PRE`，使用Vector量化模式，并通过quantAlphaTensor传入每列的量化参数
- 说明：将float类型数据量化为int8_t类型，C矩阵的每一列对应一个量化参数，使用的量化参数需要从GM拷贝量化参数到L1

**场景6：输出格式ND，输出数据类型float，使能ReLU**
- 输入：A [128, 128] half类型，ND格式；B [128, 256] half类型，ND格式
- 输出：C [128, 256] float类型，ND格式
- 实现：设置 `fixpipeParams.reluEn = true` 开启ReLU功能
- 说明：在数据从L0C搬出到GM的过程中执行ReLU操作，即将负值置为0

**场景7：输出格式Nz，输出数据类型float，使能ChannelSplit**
- 输入：A [128, 128] half类型，ND格式；B [128, 256] half类型，ND格式
- 输出：C [128, 512] float类型，Nz格式（使能通道拆分）
- 实现：设置 `fixpipeParams.isChannelSplit = true` 开启ChannelSplit功能
- 说明：在数据从L0C搬出到GM的过程中开启通道拆分功能，即将16x16小z分形矩阵拆分成两个独立的16x8小z分形矩阵输出到GM，Fixpipe接口的输入输出必须均为float类型，且仅支持Nz格式

### 场景8-41详细说明（float L0C，仅dav-3510）

以下场景使用half类型作为矩阵乘输入（A和B），L0C结果为float类型，通过Fixpipe搬出到GM时支持多种输出数据类型和量化模式。每个类型组合同时测试ND和NZ两种输出格式，以及scalar和tensor两种量化方式（如适用）。

**场景8-9：float→int8_t，NZ格式**
- 场景8：QF322B8_PRE（scalar量化），NZ输出
- 场景9：VQF322B8_PRE（tensor量化），NZ输出
- 说明：与场景4-5相同的量化模式，但输出为NZ格式

**场景10-13：float→uint8_t**
- 场景10-11：ND格式，QF322B8_PRE / VQF322B8_PRE
- 场景12-13：NZ格式，QF322B8_PRE / VQF322B8_PRE
- 说明：将float量化为uint8_t，与int8_t使用相同的quantPre模式，但sign位为0（无符号）

**场景14-19：float→half**
- 场景14-15：F322F16（Cast模式），ND/NZ输出
- 场景16-17：QF322F16_PRE（scalar/tensor），ND输出
- 场景18-19：QF322F16_PRE（scalar/tensor），NZ输出
- 说明：Cast模式直接将float转换为half（CAST_RINT）；量化模式通过scale和offset进行量化转换

**场景20-25：float→bfloat16_t**
- 场景20-21：F322BF16（Cast模式），ND/NZ输出
- 场景22-23：QF322BF16_PRE（scalar/tensor），ND输出
- 场景24-25：QF322BF16_PRE（scalar/tensor），NZ输出
- 说明：Cast模式直接将float转换为bfloat16_t；量化模式通过scale和offset进行量化转换

**场景26-29：float→fp8_e4m3fn_t**
- 场景26-27：QF322FP8_PRE（scalar/tensor），ND输出
- 场景28-29：QF322FP8_PRE（scalar/tensor），NZ输出
- 说明：将float量化为fp8_e4m3fn_t格式（1位符号+4位指数+3位尾数，范围[-448, 448]）

**场景30-37：float→hifloat8_t**
- 场景30-31：QF322HIF8_PRE / VQF322HIF8_PRE，ND输出（Half to Away Round）
- 场景32-33：QF322HIF8_PRE_HYBRID / VQF322HIF8_PRE_HYBRID，ND输出（Hybrid Round）
- 场景34-37：NZ输出，对应上述4种模式
- 说明：将float量化为hifloat8_t格式，支持两种舍入模式

**场景38-41：float→float（量化模式）**
- 场景38-39：QF322F32_PRE / VQF322F32_PRE，ND输出
- 场景40-41：QF322F32_PRE / VQF322F32_PRE，NZ输出
- 说明：L0C的float数据经过量化（scale和offset）后仍以float格式输出到GM

### 场景42-59详细说明（int32_t L0C，仅dav-3510）

以下场景使用int8_t类型作为矩阵乘输入（A和B），Mmad计算结果存放在L0C中为int32_t类型，通过Fixpipe搬出到GM时支持多种输出数据类型和量化模式。

**场景42-43：int32_t→int32_t，NoQuant**
- 场景42：ND输出；场景43：NZ输出
- 说明：将int32_t的L0C结果直接搬出到GM，不做量化转换

**场景44-47：int32_t→int8_t，REQ8 / VREQ8**
- 场景44-45：ND输出，REQ8（scalar）/ VREQ8（tensor）
- 场景46-47：NZ输出，REQ8 / VREQ8
- 说明：将int32_t重量化为int8_t，n_shift为右移操作

**场景48-51：int32_t→uint8_t，REQ8 / VREQ8**
- 场景48-49：ND输出；场景50-51：NZ输出
- 说明：将int32_t重量化为uint8_t，sign位为0

**场景52-55：int32_t→half，DEQF16 / VDEQF16**
- 场景52-53：ND输出；场景54-55：NZ输出
- 说明：将int32_t反量化为half，通过scale参数进行浮点缩放

**场景56-59：int32_t→bfloat16_t，QS322BF16_PRE / VQS322BF16_PRE**
- 场景56-57：ND输出；场景58-59：NZ输出
- 说明：将int32_t量化为bfloat16_t

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。
- 配置环境变量  
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/zh/quick_start.md#prepare&install)，配置环境变量。
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。
- 样例执行

  在本样例目录下执行如下命令。
  ```bash
  SCENARIO_NUM=15
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;    # 编译工程，默认npu模式
  python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO_NUM   # 生成测试输入数据
  ./demo                           # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin ./output/golden.bin -scenarioNum=$SCENARIO_NUM  # 验证输出结果是否正确
  ```

  > **注意：** 场景3及场景8-59仅支持 `dav-3510` 架构。场景1-7同时支持 `dav-2201` 和 `dav-3510`。

  使用 CPU调试 或 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。
  
  示例如下：

  ```bash
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # CPU调试模式
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # NPU仿真模式
  ```
  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU 运行、CPU调试、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201`（默认）、`dav-3510` | NPU 架构：dav-2201 对应 Atlas A2 训练系列产品/Atlas A2 推理系列产品和 Atlas A3 训练系列产品/Atlas A3 推理系列产品，dav-3510 对应 Ascend 950PR/Ascend 950DT |
  | `SCENARIO_NUM` | 1-59 | 场景编号（场景3和8-59仅支持dav-3510） |

  执行结果如下，说明精度对比成功。

  ```bash
  test pass!
  ```
