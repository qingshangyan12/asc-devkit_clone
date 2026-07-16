# asc_pair_reduce_sum

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id3 -->

## 功能说明

头文件路径：`"c_api/reg_compute/reg_vector.h"`。

将传入的src中相邻两个数值相加，并将产生的结果保存在dst中的低位位置。以half数据类型为例，相加示意图如下：

![asc_pair_reduce_sum示意图](../../../../figures/reg_pair_reduce_elem.png)

## 函数原型

```cpp
__simd_callee__ inline void asc_pair_reduce_sum(vector_half& dst, vector_half src, vector_bool mask)
__simd_callee__ inline void asc_pair_reduce_sum(vector_float& dst, vector_float src, vector_bool mask)
```

## 参数说明

**表1**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dst | 输出 | 目的操作数（矢量数据寄存器）。<br>类型为[vector_[data_type]](../reg_data_types/data_type_definition.md#矢量数据寄存器)，data_type的取值范围请参考[数据类型](#数据类型)。 |
| src | 输入 | 源操作数（矢量数据寄存器）。<br>类型为[vector_[data_type]](../reg_data_types/data_type_definition.md#矢量数据寄存器)，data_type的取值范围请参考[数据类型](#数据类型)。 |
| mask | 输入 | 源操作数元素操作的有效指示。<br>类型为[vector_bool](../reg_data_types/data_type_definition.md#掩码寄存器)。mask用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 |

## 数据类型

目的操作数与源操作数的数据类型需要保持一致。支持的数据类型为：half、float。

## 返回值说明

无

## 约束说明

- mask指定源操作数是否参与计算，不参与计算的元素，被当作0进行累加。例如：
  - 若连续两个元素a、b均不参与计算，目的操作数结果为0。
  - 若连续两个元素a、b仅元素a参与计算，目的操作数结果为a。
  - 若连续两个元素a、b仅元素b参与计算，目的操作数结果为b。

- 求和后，dst中仅前一半元素为有效数据，后一半填充为0。搬出至UB时需要避免数据踩踏。

## 调用示例

```c++
vector_half dst;
vector_half src;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
asc_loadalign(src, src_addr); // src_addr是外部输入的UB内存空间地址。
asc_pair_reduce_sum(dst, src, mask);
```