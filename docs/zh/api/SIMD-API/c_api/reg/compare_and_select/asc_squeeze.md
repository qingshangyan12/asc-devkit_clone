# asc_squeeze

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

将src中被mask选择的有效元素依次复制到dst中，有效元素在dst中从低到高连续排列，剩余位置元素置为0。

本接口不会将有效数据大小保存至AR寄存器，如果想将有效数据大小保存至AR寄存器，请参考[asc_squeeze_with_status](asc_squeeze_with_status.md)。

本接口仅在AIV上执行有效，在AIC上调用为空操作。

## 函数原型

### 模板原型

```c
__simd_callee__ inline void asc_squeeze(vector_<dtype>& dst,
                                        vector_<dtype> src,
                                        vector_bool mask)
```

dtype支持的数据类型为`int8_t`、`uint8_t`、`hifloat8_t`、`fp8_e8m0_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。

### 典型示例

```c
// 示例：对half矢量数据寄存器执行按掩码压缩
__simd_callee__ inline void asc_squeeze(vector_half& dst,
                                        vector_half src,
                                        vector_bool mask)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dst | 输出 | 目的操作数（矢量数据寄存器）。 |
| src | 输入 | 源操作数（矢量数据寄存器）。 |
| mask | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

- 本接口仅在AIV上生效，非AIV调用直接返回。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用，dst与src为矢量数据寄存器，不涉及UB地址，无UB容量与地址对齐约束。
- mask为`vector_bool`类型掩码寄存器，需通过配套的掩码设置接口预先赋值后再传入；未赋值的掩码寄存器内容不确定，会导致压缩结果错误。
- dst与src的dtype需要保持一致。
- mask比特位为1的src元素按原顺序紧凑排列到dst低位；mask比特位为0的src元素不参与压缩，dst中压缩结果之后的剩余高位统一写0。
- 浮点dtype仅按元素搬移src的二进制数据，不进行浮点数值计算，因此不检查NaN/Inf，也不上报浮点异常；src包含NaN/Inf时仍可正常压缩。

## 调用示例

```cpp
__simd_vf__ inline void squeeze_vf(__ubuf__ int8_t* dst_addr, __ubuf__ int8_t* src_addr, uint32_t count, uint16_t one_repeat_size, uint16_t repeat_time)
{
    vector_int8_t dst;
    vector_int8_t src;
    vector_bool mask;
    addr_reg addr_reg;
    for (uint16_t i = 0; i < repeat_time; ++i) {
        addr_reg = asc_update_addr_reg_b8(one_repeat_size);
        mask = asc_update_mask_b8(count);
        asc_loadalign(src, src_addr, addr_reg);
        asc_squeeze(dst, src, mask);
        asc_storealign(dst_addr, dst, addr_reg, mask);
    }
}
```
