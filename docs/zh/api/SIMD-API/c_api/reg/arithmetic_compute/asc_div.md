# asc_div

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

该接口根据mask，对源操作数src0、src1进行按元素求商操作，将结果写入目的操作数dst。

计算公式如下：

$$
dst_i = src0_i \div src1_i
$$

## 函数原型

### 占位符形式

```c
// 通过引用参数输出结果
__simd_callee__ inline void asc_div(vector_<dtype>& dst,
                                    vector_<dtype> src0,
                                    vector_<dtype> src1,
                                    vector_bool mask)

// 通过函数返回值返回结果
__simd_callee__ inline vector_<dtype> asc_div(vector_<dtype> src0,
                                              vector_<dtype> src1,
                                              vector_bool mask)
```

**dtype取值如下：**`int16_t`、`uint16_t`、`half`、`int32_t`、`uint32_t`、`float`。

**函数原型典型示例**

```c
// 通过引用参数输出结果示例：float类型
__simd_callee__ inline void asc_div(vector_float& dst,
                                    vector_float src0,
                                    vector_float src1,
                                    vector_bool mask)

// 通过函数返回值返回结果示例：float类型
__simd_callee__ inline vector_float asc_div(vector_float src0,
                                              vector_float src1,
                                              vector_bool mask)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的操作数（矢量数据寄存器），仅适用于通过引用参数输出结果的函数原型。 |
| src0 | 输入 | 源操作数（矢量数据寄存器）。 |
| src1 | 输入 | 源操作数（矢量数据寄存器）。 |
| mask | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

- 通过引用参数输出结果的函数原型无返回值。
- 通过函数返回值返回结果的函数原型返回逐元素除法结果，返回值类型与源操作数类型一致。

## 约束说明

- 通过引用参数输出结果的函数原型在非AIV上调用时直接返回。
- 通过函数返回值输出结果的函数原型在非AIV上调用时返回对应矢量类型的默认构造值。

- mask控制源操作数是否参与计算，源操作数不参与计算的元素在输出对应位置置零。

- 注意除零错误，结果请参考[Div特殊值/边界值输入的计算结果说明](../../../../appendix/reg_vector_compute_interface_boundary_value_summary.md#table1325925915217)。

## 调用示例

```cpp
__simd_vf__ inline void div_vf(__ubuf__ half* dst_addr, __ubuf__ half* src0_addr, __ubuf__ half* src1_addr, uint32_t count, int32_t one_repeat_size, uint16_t repeat_time)
{
    vector_half src0;
    vector_half src1;
    vector_half dst;
    vector_bool mask;
    for (uint16_t i = 0; i < repeat_time; ++i) {
        mask = asc_update_mask_b16(count);
        asc_loadalign_postupdate(src0, src0_addr, one_repeat_size);
        asc_loadalign_postupdate(src1, src1_addr, one_repeat_size);
        asc_div(dst, src0, src1, mask);
        asc_storealign_postupdate(dst_addr, dst, one_repeat_size, mask);
    }
}
```
