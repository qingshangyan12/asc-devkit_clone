# asc_select

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

根据mask从源操作数src0，src1中选择元素，得到目的操作数dst。选择的规则为：当mask的比特位为1时，从src0中选取对应位置的数；当mask的比特位为0时，从src1中选取对应位置的数。计算公式如下：

$$
dst_i = 
\begin{cases}
 src0_i, & mask_i = 1 \\
 src1_i, & mask_i = 0 \\
\end{cases}
$$

## 函数原型

### 模板原型（占位符形式）

```c
__simd_callee__ inline void asc_select(vector_<dtype>& dst,
                                       vector_<dtype> src0,
                                       vector_<dtype> src1,
                                       vector_bool mask)
```

### dtype支持数据类型

dtype支持的数据类型为`bool`、`uint8_t`、`int8_t`、`fp8_e4m3fn_t`、`hifloat8_t`、`fp8_e5m2_t`、`uint16_t`、`int16_t`、`half`、`bfloat16_t`、`uint32_t`、`int32_t`、`float`

### 典型示例

```c
// 示例：对half矢量数据寄存器执行按掩码选择
__simd_callee__ inline void asc_select(vector_half& dst,
                                       vector_half src0,
                                       vector_half src1,
                                       vector_bool mask)
```

## 参数说明

**表1** 参数说明
| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst | 输出 | 目的操作数（掩码寄存器或矢量数据寄存器）。 |
| src0 | 输入 | 源操作数0（掩码寄存器或矢量数据寄存器）。 |
| src1 | 输入 | 源操作数1（掩码寄存器或矢量数据寄存器）。 |
| mask | 输入 | 源操作数掩码（掩码寄存器）。指定选择src0或src1为有效数据。mask的比特位为1时，选取src0；mask的比特位为0时，选取src1。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 约束说明

### 通用约束

- 通过mask参数控制的未选中的元素保持源操作数的值而非清零。
- src0、src1和dst的数据类型需要保持一致。

## 调用示例

```cpp
__simd_vf__ inline void select_vf(__ubuf__ half* src0_addr, __ubuf__ half* src1_addr, __ubuf__ half* dst_addr, uint32_t count, uint16_t one_repeat_size, uint16_t repeat_time)
{
    vector_half src0, src1, dst;
    vector_bool mask;
    for (uint16_t i = 0; i < repeat_time; ++i) {
        mask = asc_update_mask_b16(count);
        asc_loadalign_postupdate(src0, src0_addr, one_repeat_size);
        asc_loadalign_postupdate(src1, src1_addr, one_repeat_size);
        asc_select(dst, src0, src1, mask);
        asc_storealign_postupdate(dst_addr, dst, one_repeat_size, mask);
    }
}
```
