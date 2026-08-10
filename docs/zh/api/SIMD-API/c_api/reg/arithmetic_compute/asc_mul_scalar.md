# asc_mul_scalar

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

根据mask将源操作数src按元素与标量value相乘，将结果写入目的操作数dst。计算公式如下：

$$
dst_i = src_i * value
$$

## 函数原型

### 模板原型（占位符形式）

```c
__simd_callee__ inline void asc_mul_scalar(vector_<dtype>& dst,
                                           vector_<dtype> src,
                                           <dtype> value,
                                           vector_bool mask)
```

### dtype支持数据类型

dtype支持的数据类型为`int16_t`、`uint16_t`、`half`、`int32_t`、`uint32_t`、`float`

### 典型示例

```c
// 示例：half 寄存器形式（mask 控制有效元素范围）
__simd_callee__ inline void asc_mul_scalar(vector_half& dst,
                                           vector_half src,
                                           half value,
                                           vector_bool mask)
```

## 参数说明

**表1** 参数说明
| 参数名 | 输入/输出 | 描述                                                                                                             |
| :----- | :-------- | :--------------------------------------------------------------------------------------------------------------- |
| dst    | 输出      | 目的操作数（矢量数据寄存器）。                                                                                   |
| src    | 输入      | 源操作数（矢量数据寄存器）。                                                                                     |
| value  | 输入      | 源操作数（标量）。                                                                                               |
| mask   | 输入      | 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 约束说明

### 通用约束

- dst与src的数据类型需要保持一致。
- mask掩码位为0时，dst对应元素置0。

## 调用示例

```cpp
__simd_vf__ inline void mul_scalar_vf(__ubuf__ half* src_addr, __ubuf__ half* dst_addr, half scalar, uint32_t count, uint16_t one_repeat_size, uint16_t repeat_time)
{
    vector_half src, dst;
    vector_bool mask;
    for (uint16_t i = 0; i < repeat_time; ++i) {
        mask = asc_update_mask_b16(count);
        asc_loadalign_postupdate(src, src_addr, one_repeat_size);
        asc_mul_scalar(dst, src, scalar, mask);
        asc_storealign_postupdate(dst_addr, dst, one_repeat_size, mask);
    }
}
```
