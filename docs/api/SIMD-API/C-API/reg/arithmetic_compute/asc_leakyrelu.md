# asc_leakyrelu

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

该接口用于按元素执行Leaky ReLU(Leaky Rectified Linear Unit)操作：当源操作数中某元素大于0时，直接将该元素写入目的操作数；否则将该元素乘以标量值value后写入目的操作数。计算公式如下：

$$dst_i = \begin{cases} src_i & src_i > 0 \\ src_i \times value & src_i \leq 0 \end{cases}$$

## 函数原型

```cpp
__simd_callee__ inline void asc_leakyrelu(vector_float& dst, vector_float src, float value, vector_bool mask)
__simd_callee__ inline void asc_leakyrelu(vector_half& dst, vector_half src, half value, vector_bool mask)
```

## 参数说明

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

无

## 调用示例

 ```cpp
vector_half dst;
vector_half src;
half value = 0.1;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
asc_loadalign(src, src_addr); // src_addr是外部输入的UB内存空间地址。
asc_leakyrelu(dst, src, value, mask);
```