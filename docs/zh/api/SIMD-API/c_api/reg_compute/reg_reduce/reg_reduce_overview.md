# Reg归约计算概述

Reg归约计算接口将一个矢量数据寄存器中的多个元素沿寄存器维度归约为一个或一组结果，归约结果写入目的操作数的低位，其余位置置0。接口支持整体归约、按DataBlock归约和相邻元素两两归约三种粒度。

所有Reg归约计算接口仅在AIV上生效，且需在Vector Function（`__simd_vf__`标记的函数）内调用。归约结果为标量或少量元素，通常需要搬出到Unified Buffer（UB）后使用，搬出时需注意目的地址范围，避免数据踩踏。

## 接口概览

**表1** Reg归约计算接口

| 接口名称 | 归约范围 | 计算公式 | 输出布局 | 支持数据类型 |
| --- | --- | --- | --- | --- |
| [asc_pair_reduce_sum](asc_pair_reduce_sum.md) | 相邻两个元素 | $dst_i = src_{2i} + src_{2i+1} \quad (0 \le i < \frac{N}{2})$ | `dst`的前`N/2`个元素为有效结果，后一半置0。 | half、float |
| [asc_reduce_max](asc_reduce_max.md) | 整个矢量寄存器 | $dst_0 = \max\{src_i \mid mask_i = 1\}$<br>$dst_1 = \operatorname{argmax}\{src_i \mid mask_i = 1\}$ | `dst[0]`为最大值，`dst[1]`为最大值索引，其余置0。 | int16_t、uint16_t、half、int32_t、uint32_t、float |
| [asc_reduce_max_datablock](asc_reduce_max_datablock.md) | 每个DataBlock（32B） | $dst_k = \max\{src_i \mid kB \le i < (k + 1)B,\ mask_i = 1\}$ | `dst[0]`~`dst[7]`依次为8个DataBlock的最大值，其余置0。 | int16_t、uint16_t、half、int32_t、uint32_t、float |
| [asc_reduce_min](asc_reduce_min.md) | 整个矢量寄存器 | $dst_0 = \min\{src_i \mid mask_i = 1\}$<br>$dst_1 = \operatorname{argmin}\{src_i \mid mask_i = 1\}$ | `dst[0]`为最小值，`dst[1]`为最小值索引，其余置0。 | int16_t、uint16_t、half、int32_t、uint32_t、float |
| [asc_reduce_min_datablock](asc_reduce_min_datablock.md) | 每个DataBlock（32B） | $dst_k = \min\{src_i \mid kB \le i < (k + 1)B,\ mask_i = 1\}$ | `dst[0]`~`dst[7]`依次为8个DataBlock的最小值，其余置0。 | int16_t、uint16_t、half、int32_t、uint32_t、float |
| [asc_reduce_sum](asc_reduce_sum.md) | 整个矢量寄存器 | $dst_0 = \sum_{mask_i = 1} src_i$ | `dst[0]`为总和，其余置0。 | half、float、int32_t、uint32_t；int16_t输入提升为int32_t，uint16_t输入提升为uint32_t |
| [asc_reduce_sum_datablock](asc_reduce_sum_datablock.md) | 每个DataBlock（32B） | $dst_k = \sum_{kB \le i < (k + 1)B,\ mask_i = 1} src_i$ | `dst[0]`~`dst[7]`依次为8个DataBlock的和，其余置0。 | half、float、int32_t、uint32_t；int16_t输入提升为int32_t，uint16_t输入提升为uint32_t |

其中，$N$为矢量数据寄存器中的元素个数，$k$为DataBlock索引，$B$为一个DataBlock内的元素个数。矢量寄存器位宽为256字节时，DataBlock数量为8。

## 关键特性

归约接口的输出只占目的寄存器低位，且不同归约粒度的输出布局、索引形式和精度提升规则不同。以下从算子实现角度说明这些特殊能力。

### 整体归约与DataBlock归约

**表2** 整体归约与DataBlock归约对比

| 对比项 | 整体归约（`asc_reduce_sum`、`asc_reduce_max`、`asc_reduce_min`） | DataBlock归约（`*_datablock`） |
| --- | --- | --- |
| 归约范围 | 整个矢量寄存器 | 每个DataBlock（32B）内独立归约 |
| 输出个数 | 1个归约值，`max`/`min`额外输出1个索引 | 8个归约值（矢量寄存器位宽为256字节时） |
| 输出布局 | `dst[0]`为归约值，`max`/`min`的`dst[1]`为索引，其余置0 | `dst[0]`~`dst[7]`依次为各DataBlock的结果，其余置0 |
| 索引输出 | `asc_reduce_max`、`asc_reduce_min`输出极值索引 | 不输出索引 |
| `mask`未选中元素 | 求和按0处理；求最大值按数据类型最小值处理；求最小值按数据类型最大值处理 | 与整体归约相同 |

两种归约的有效输出范围不同：整体归约只写`dst`低位的1个元素（`max`/`min`为2个），DataBlock归约写前8个元素，其余位置均置0。写算子时若按固定长度搬出结果，必须按实际有效元素个数截取，否则会把置0区域当成有效数据。只有整体归约的`asc_reduce_max`和`asc_reduce_min`提供索引，DataBlock变体无法定位极值所在位置。

### 求和

**表3** 求和类接口对比

| 接口名称 | 归约粒度 | 计算方式 | 输出 |
| --- | --- | --- | --- |
| [asc_reduce_sum](asc_reduce_sum.md) | 整个矢量寄存器 | 所有参与计算元素按二叉树方式逐层累加。 | `dst[0]`为总和，其余置0。 |
| [asc_reduce_sum_datablock](asc_reduce_sum_datablock.md) | 每个DataBlock（32B） | 每个DataBlock内独立按二叉树方式累加。 | `dst[0]`~`dst[7]`为8个分块和，其余置0。 |
| [asc_pair_reduce_sum](asc_pair_reduce_sum.md) | 相邻两个元素 | 每两个相邻元素相加，不做跨对累加。 | `dst`的前`N/2`个元素为结果，后一半置0。 |

三种求和的输出粒度不同：`asc_reduce_sum`只产生1个总和，`asc_reduce_sum_datablock`产生8个分块和，`asc_pair_reduce_sum`产生`N/2`个成对和并占用`dst`的低半部分。写算子时需按对应长度读取有效结果；`asc_pair_reduce_sum`不跨对累加，因此不能直接替代整体求和。求和类接口将`mask`未选中的元素按0处理。

`asc_reduce_sum`和`asc_reduce_sum_datablock`对int16_t、uint16_t输入会提升精度到int32_t、uint32_t再计算；`asc_pair_reduce_sum`不提升精度，输入和输出数据类型一致。

### 极值

**表4** 极值类接口对比

| 对比项 | [asc_reduce_max](asc_reduce_max.md) | [asc_reduce_min](asc_reduce_min.md) | [asc_reduce_max_datablock](asc_reduce_max_datablock.md)、[asc_reduce_min_datablock](asc_reduce_min_datablock.md) |
| --- | --- | --- | --- |
| 极值方向 | 最大值 | 最小值 | 各DataBlock内的最大值或最小值 |
| 索引输出 | `dst[1]`为最大值索引 | `dst[1]`为最小值索引 | 不输出索引 |
| `mask`未选中元素 | 按数据类型最小值处理，浮点为`-inf` | 按数据类型最大值处理，浮点为`+inf` | 与整体归约相同 |
| 负零与正零比较 | $max(-0, +0) = +0$ | $min(-0, +0) = -0$ | 与整体归约相同 |
| NaN处理 | `dst[0]`写入NaN，`dst[1]`写入第一个NaN的索引 | 同左侧 | 文档未单独说明NaN处理方式。 |

`asc_reduce_max`和`asc_reduce_min`的特殊之处在于同时产生值和索引两个结果，且索引以`dst`数据类型的位模式存储：half结果中的索引按uint16_t位模式解释，float结果中的索引按uint32_t位模式解释。写算子时不能把索引当普通数值读取，必须先按位重解释为整型。DataBlock变体只输出值，不提供定位极值位置的能力。当所有元素均未被`mask`选中时，整体归约将对应数据类型的最小值或最大值写入`dst[0]`，其余元素置0。

## 通用约束

- Reg矢量计算C API通用约束请参见[通用约束](../overview.md#通用约束)。
- 接口仅在AIV上生效，需在Vector Function内调用。
- 归约结果只占用`dst`的低位，其余元素置0；将结果搬出到UB时，需按有效结果个数控制搬运范围，避免覆盖其他数据。
- `mask`未选中元素的处理方式随归约类型不同：求和类按0处理，求最大值按数据类型最小值处理，求最小值按数据类型最大值处理。
- 本组接口仅Ascend 950PR&950DT系列产品支持。
