# asc_or

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

该接口可用于对矢量数据寄存器或掩码寄存器操作：

- 对矢量数据寄存器操作：根据mask对源操作数src0、src1执行按位或（|）操作，将结果写入目的操作数dst。

- 对掩码寄存器操作：根据mask对源操作数src0、src1的有效bit执行按位或（|）操作，将结果写入目的操作数dst。

计算公式如下：

$$
dst_i = src0_i | src1_i
$$

本接口仅在AIV上执行有效，在AIC上调用为空操作。

## 函数原型

### 矢量数据寄存器按位或

#### 模板原型

```c
__simd_callee__ inline void asc_or(vector_<dtype>& dst,
                                   vector_<dtype> src0,
                                   vector_<dtype> src1,
                                   vector_bool mask)
```

#### dtype支持数据类型

dtype支持的数据类型为`uint8_t`、`int8_t`、`uint16_t`、`int16_t`、`half`、`uint32_t`、`int32_t`、`float`

#### 典型示例

```c
// 示例：对half矢量数据寄存器执行按位或
__simd_callee__ inline void asc_or(vector_half& dst,
                                   vector_half src0,
                                   vector_half src1,
                                   vector_bool mask)
```

### 掩码寄存器按位或

```c
__simd_callee__ inline void asc_or(vector_bool& dst,
                                   vector_bool src0,
                                   vector_bool src1,
                                   vector_bool mask)
```

## 参数说明

**表1** 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst | 输出 | 目的操作数（矢量数据寄存器或掩码寄存器）。 |
| src0 | 输入 | 源操作数（矢量数据寄存器或掩码寄存器）。 |
| src1 | 输入 | 源操作数（矢量数据寄存器或掩码寄存器）。 |
| mask | 输入 | 源操作数掩码（掩码寄存器）。<br>&bull;源操作数为矢量数据寄存器时，对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。<br>&bull;源操作数为掩码寄存器时，指示在计算过程中哪些bit有效。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 约束说明

### 通用约束

- 本接口仅在AIV上生效，非AIV调用直接返回。
- src0、src1、dst三者dtype必须完全一致，否则触发数据类型异常。
- mask必须为`vector_bool`类型掩码寄存器，需通过掩码设置接口（如`asc_update_mask`系列）预先赋值后再传入；未赋值的掩码寄存器内容不确定，会导致有效元素位置错误。
- mask比特位按元素大小步进过滤：矢量数据寄存器按位或中每个元素占用`sizeof(dtype)`个比特位，元素i的有效mask比特位于`i × sizeof(dtype)`位置；掩码寄存器按位或中每个比特独立对应1个掩码位。
- 参与计算的元素个数由矢量长度（VL）决定，矢量数据寄存器按位或中元素个数 = VL ÷ sizeof(dtype)；掩码寄存器按位或中比特个数 = VL。
- mask比特位为0的位置采用清零模式（ZEROING）：dst对应位置写0，不保留src0、src1原值也不保留dst原值。
- 按位或按元素的二进制位逐位执行，无溢出、无进位、无NaN/Inf异常上报；浮点dtype（half、float）的按位或直接对其二进制表示位执行，结果通常无浮点语义意义，常用于位掩码操作（如置位符号位或指数位、合并多个位域标记）。

### 矢量数据寄存器按位或约束

- dtype仅支持int8_t、uint8_t、int16_t、uint16_t、half、int32_t、uint32_t、float共8种矢量数据寄存器类型，其他dtype触发数据类型异常。

### 掩码寄存器按位或约束

- dst、src0、src1必须均为`vector_bool`类型，其他dtype触发数据类型异常。
- 掩码寄存器按位或用于复合掩码生成：典型用法是将两个由`asc_update_mask`系列生成的源掩码按位或，得到满足任一条件的复合掩码，再传给后续计算接口。

## 调用示例

```cpp
__simd_vf__ inline void or_vf(__ubuf__ half* dst_addr, __ubuf__ half* src0_addr, __ubuf__ half* src1_addr, uint32_t count, uint16_t one_repeat_size, uint16_t repeat_time)
{
    vector_half dst;
    vector_half src0;
    vector_half src1;
    vector_bool mask;
    for (uint16_t i = 0; i < repeat_time; ++i) {
        mask = asc_update_mask_b16(count);
        asc_loadalign(src0, src0_addr + i * one_repeat_size);
        asc_loadalign(src1, src1_addr + i * one_repeat_size);
        asc_or(dst, src0, src1, mask);
        asc_storealign(dst_addr + i * one_repeat_size, dst, mask);
    }
}
```
