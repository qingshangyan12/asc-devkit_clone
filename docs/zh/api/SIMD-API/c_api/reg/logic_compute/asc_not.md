# asc_not

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

该接口用于对源操作数src执行按位取反操作，将结果写入目的操作数dst。

计算公式如下：

$$
dst_i = \sim src_i
$$

本接口仅在AIV上执行有效，在AIC上调用为空操作。

## 函数原型

### 模板原型

```c
__simd_callee__ inline void asc_not(vector_bool& dst,
                                    vector_bool src,
                                    vector_bool mask)
__simd_callee__ inline void asc_not(vector_<dtype>& dst,
                                    vector_<dtype> src,
                                    vector_bool mask)
```

dtype支持的数据类型为`bool`、`uint8_t`、`int8_t`、`uint16_t`、`int16_t`、`half`、`uint32_t`、`int32_t`、`float`

### 典型示例

```c
// 示例：对half矢量数据寄存器执行按位取反
__simd_callee__ inline void asc_not(vector_half& dst,
                                    vector_half src,
                                    vector_bool mask)
```

## 参数说明

**表1** 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst | 输出 | 目的操作数（矢量数据寄存器）。|
| src | 输入 | 源操作数（矢量数据寄存器）。|
| mask | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 约束说明

### 通用约束

- 本接口仅在AIV上生效，非AIV调用直接返回。
- src、dst两者dtype必须完全一致，否则触发数据类型异常。
- mask必须为`vector_bool`类型掩码寄存器，需通过掩码设置接口（如`asc_update_mask`系列）预先赋值后再传入；未赋值的掩码寄存器内容不确定，会导致有效元素位置错误。
- mask比特位按元素大小步进过滤：矢量数据寄存器按位取反中每个元素占用`sizeof(dtype)`个比特位，元素i的有效mask比特位于`i × sizeof(dtype)`位置；掩码寄存器按位取反中每个比特独立对应1个掩码位。
- 参与计算的元素个数由矢量长度（VL）决定，矢量数据寄存器按位取反中元素个数=VL ÷ sizeof(dtype)；掩码寄存器按位取反中比特个数=VL。
- mask比特位为0的位置采用清零模式（ZEROING）：dst对应位置写0，不保留src原值也不保留dst原值。
- 按位取反按元素的二进制位逐位执行，无溢出、无进位、无NaN/Inf异常上报；浮点dtype（half、float）的按位取反直接对其二进制表示位执行，结果通常无浮点语义意义，常用于位掩码操作（如翻转符号位或指数位）。

### 矢量数据寄存器按位取反约束

- dtype仅支持int8_t、uint8_t、int16_t、uint16_t、half、int32_t、uint32_t、float共8种矢量数据寄存器类型，其他dtype触发数据类型异常。

### 掩码寄存器按位取反约束

- dst、src必须均为`vector_bool`类型，与其他矢量数据寄存器类型混用触发数据类型异常。
- 掩码寄存器按位取反用于掩码的反转：典型用法是将一个由`asc_update_mask`系列生成的源掩码按位取反，得到原掩码的逻辑非，再传给后续计算接口作为互补掩码。

## 调用示例

```cpp
__simd_vf__ inline void not_vf(__ubuf__ half* dst_addr, __ubuf__ half* src_addr, uint32_t count, uint16_t one_repeat_size, uint16_t repeat_time)
{
    vector_half dst;
    vector_half src;
    vector_bool mask;
    for (uint16_t i = 0; i < repeat_time; ++i) {
        mask = asc_update_mask_b16(count);
        asc_loadalign(src, src_addr + i * one_repeat_size);
        asc_not(dst, src, mask);
        asc_storealign(dst_addr + i * one_repeat_size, dst, mask);
    }
}
```
