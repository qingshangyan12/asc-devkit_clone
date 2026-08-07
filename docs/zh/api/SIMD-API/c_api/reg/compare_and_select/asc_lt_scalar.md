# asc_lt_scalar

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

根据mask，对src中的元素逐个与标量value进行比较（小于），将比较结果写入dst。dst为vector_bool类型，每个元素的比较结果占1个bit，1表示条件成立，0表示不成立。计算公式如下：

$$
dst_i = (src_i < value)
$$

本接口仅在AIV上执行有效，在AIC上调用为空操作。

## 函数原型

### 模板原型

```c
__simd_callee__ inline void asc_lt_scalar(vector_bool& dst,
                                          vector_<dtype> src,
                                          <dtype> value,
                                          vector_bool mask)
```

dtype支持的数据类型为`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`

### 典型示例

```c
// 示例：half源向量与标量小于比较，结果写入vector_bool掩码寄存器
__simd_callee__ inline void asc_lt_scalar(vector_bool& dst,
                                          vector_half src,
                                          half value,
                                          vector_bool mask)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dst | 输出 | 目的操作数（掩码寄存器）。 |
| src | 输入 | 源操作数（矢量数据寄存器）。 |
| value | 输入 | 源操作数（标量）。 |
| mask | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 约束说明

- 本接口仅在AIV上生效，非AIV调用直接返回。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用，dst、src、mask均为寄存器操作数，不涉及UB地址，无UB容量与地址对齐约束。
- src与value的dtype必须完全一致（int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float之一），其他dtype触发数据类型异常。
- dst为`vector_bool`类型掩码寄存器，写入1 bit/元素的比较结果，不可与其他矢量数据寄存器类型混用。
- mask为`vector_bool`类型掩码寄存器，需通过掩码设置接口预先赋值后再传入；未赋值的掩码寄存器内容不确定，会导致有效元素位置错误。
- mask比特位按元素大小步进过滤：每个元素占用`sizeof(dtype)`个比特位，元素i的有效mask比特位于`i × sizeof(dtype)`位置。
- mask比特位为0的位置采用清零模式（ZEROING）：dst对应比特位写0，不参与比较也不保留dst原值。
- 浮点dtype（`half`、`bfloat16_t`、`float`）比较时，+0与-0视为相等。
- 浮点dtype（`half`、`bfloat16_t`、`float`）比较输入含NaN时，比较结果按IEEE 754标准返回false（NaN与任何值都不满足小于关系），dst对应比特位写0。

## 调用示例

```cpp
__simd_vf__ inline void lt_scalar_vf(__ubuf__ uint16_t* dst_addr, __ubuf__ half* src_addr, half value, uint32_t count, uint16_t one_repeat_size, uint16_t one_block_size, uint16_t repeat_time)
{
    vector_half src;
    vector_bool dst;
    vector_bool mask;
    for (uint16_t i = 0; i < repeat_time; ++i) {
        mask = asc_update_mask_b16(count);
        asc_loadalign_postupdate(src, src_addr, one_repeat_size);
        asc_lt_scalar(dst, src, value, mask);
        asc_storealign_postupdate(dst_addr, dst, one_block_size);
    }
}
```
