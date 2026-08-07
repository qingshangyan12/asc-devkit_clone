# asc_neg

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

该接口用于根据mask对源操作数src进行取相反数操作，将结果写入目的操作数dst。

计算公式如下：

$$
dst_i = -src_i
$$

本接口仅在AIV上执行有效，在AIC上调用为空操作。

## 函数原型

### 模板原型

```c
__simd_callee__ inline void asc_neg(vector_<dtype>& dst,
                                    vector_<dtype> src,
                                    vector_bool mask)
```

dtype支持的数据类型为`int8_t`、`int16_t`、`int32_t`、`half`、`float`

### 典型示例

```c
// 示例：对half矢量寄存器取相反数
__simd_callee__ inline void asc_neg(vector_half& dst,
                                    vector_half src,
                                    vector_bool mask)
```

## 参数说明

**表1** 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst | 输出 | 目的操作数（矢量数据寄存器）。 |
| src | 输入 | 源操作数（矢量数据寄存器）。 |
| mask | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 约束说明

- 本接口仅在AIV上生效，非AIV调用直接返回。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用，dst与src为矢量数据寄存器，不涉及UB地址，无UB容量与地址对齐约束。
- mask为`vector_bool`类型掩码寄存器，需通过掩码设置接口（如`asc_update_mask`系列）预先赋值后再传入；未赋值的掩码寄存器内容不确定，会导致有效元素位置错误。
- dst与src的dtype必须完全一致（int8_t、int16_t、half、int32_t、float之一），否则触发异常。
- mask比特位为0的位置采用清零模式（ZEROING）：dst对应位置写0，不保留src原值也不保留dst原值。
- 整数输入按有符号整数算术取反，当输入为该有符号整数类型的最小负值（int8_t为-128、int16_t为-32768、int32_t为-2147483648）时，相反数超出该类型可表示范围，结果保留原值不变。
- 浮点输入（half、float）通过翻转符号位实现取反：正数变为对应负数，负数变为对应正数，+0变为-0，-0变为+0，INF输入翻转符号位（+INF变为-INF，-INF变为+INF）。
- 浮点（half、float）输入NaN，取反结果为NaN。

## 调用示例

```cpp
__simd_vf__ inline void neg_vf(__ubuf__ half* dst_addr, __ubuf__ half* src_addr, uint32_t count, uint16_t one_repeat_size, uint16_t repeat_time)
{
    vector_half dst;
    vector_half src;
    vector_bool mask;
    for (uint16_t i = 0; i < repeat_time; ++i) {
        mask = asc_update_mask_b16(count);
        asc_loadalign(src, src_addr + i * one_repeat_size);
        asc_neg(dst, src, mask);
        asc_storealign(dst_addr + i * one_repeat_size, dst, mask);
    }
}
```
