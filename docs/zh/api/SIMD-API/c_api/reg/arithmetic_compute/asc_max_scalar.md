# asc_max_scalar

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

头文件路径：`"c_api/reg_compute/reg_vector.h"`。

矢量src的逐个元素与标量value比较大小，接着按照对应的比特位将最大值存入dst中。计算公式如下：

$$
dst_i = max(src_i, value)
$$

```python
# src: half[128]  (vector_half, 256B = 128个half元素; 以half为例, b32类型为64元素)
# dst: half[128]  (vector_half)
# value: half     (标量)
# mask: bool[128] (b16位宽, 每元素1bit)
#
# 逐元素取src与value的较大值, mask为0的元素置零(MODE_ZEROING)

def asc_max_scalar(dst, src, value, mask):
    for i in range(128):
        if mask[i]:
            dst[i] = max(src[i], value)
        else:
            dst[i] = 0
```

## 函数原型

### 模板原型（占位符形式）

本接口使用`<dtype>`表示支持的数据类型，模板原型如下：

```c
__simd_callee__ inline void asc_max_scalar(vector_<dtype>& dst,
                                           vector_<dtype> src,
                                           <dtype> value,
                                           vector_bool mask)
```

#### dtype支持数据类型

dtype支持的数据类型为`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`

### 典型示例

```c
// 示例：half寄存器形式（mask控制有效元素范围）
__simd_callee__ inline void asc_max_scalar(vector_half& dst,
                                           vector_half src,
                                           half value,
                                           vector_bool mask)
```

## 参数说明

**表1** 参数说明
| 参数名 | 输入/输出 | 描述                                                                                                                                     |
| ------ | --------- | ---------------------------------------------------------------------------------------------------------------------------------------- |
| dst    | 输出      | 目的操作数（矢量数据寄存器）。                                                                                                           |
| src    | 输入      | 源操作数（矢量数据寄存器）。                                                                                                             |
| value  | 输入      | 源操作数（标量）。                                                                                                                       |
| mask   | 输入      | 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 约束说明

### 通用约束

- dst与src的数据类型需要保持一致。
- 掩码位为0时，dst对应元素置0。
- 输入src为-0、value为+0的情况下，输出dst为+0。

## 调用示例

```cpp
__simd_vf__ inline void max_scalar_vf(__ubuf__ half* src_addr, __ubuf__ half* dst_addr, half scalar, uint32_t count, uint16_t one_repeat_size, uint16_t repeat_time)
{
    vector_half src, dst;
    vector_bool mask;
    for (uint16_t i = 0; i < repeat_time; ++i) {
        mask = asc_update_mask_b16(count);
        asc_loadalign_postupdate(src, src_addr, one_repeat_size);
        asc_max_scalar(dst, src, scalar, mask);
        asc_storealign_postupdate(dst_addr, dst, one_repeat_size, mask);
    }
}
```
