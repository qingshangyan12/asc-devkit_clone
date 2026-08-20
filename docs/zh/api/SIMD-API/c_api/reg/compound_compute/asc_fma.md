# asc_fma

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
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->

## 功能说明

完成src0与src1逐元素相乘再加src2的融合乘加运算，根据mask返回计算结果。计算公式如下：

$$
dst_i = src0_i \times src1_i + src2_i
$$

## 函数原型

### 融合乘加（占位符形式）

```c
__simd_callee__ inline vector_<dtype> asc_fma(vector_<dtype> src0,
                                              vector_<dtype> src1,
                                              vector_<dtype> src2,
                                              vector_bool mask)
```

**dtype取值如下：**`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。

**典型示例**

```c
// 示例：float类型的融合乘加运算
__simd_callee__ inline vector_float asc_fma(vector_float src0,
                                            vector_float src1,
                                            vector_float src2,
                                            vector_bool mask)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- | 
| src0 | 输入 | 源操作数（矢量数据寄存器）。|
| src1 | 输入 | 源操作数（矢量数据寄存器）。|
| src2 | 输入 | 加法源操作数（矢量数据寄存器）。|
| mask | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在返回结果中置零。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

返回计算结果，返回类型与输入矢量类型一致。mask未筛选的元素在返回结果中置零。

## 约束说明

无

## 调用示例

```cpp
vector_half src0;
vector_half src1;
vector_half src2;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
asc_loadalign(src0, src0_addr); // src0_addr是外部输入的Unified Buffer（UB）内存空间地址。
asc_loadalign(src1, src1_addr); // src1_addr是外部输入的UB内存空间地址。
asc_loadalign(src2, src2_addr); // src2_addr是外部输入的UB内存空间地址。
vector_half dst = asc_fma(src0, src1, src2, mask);
```
