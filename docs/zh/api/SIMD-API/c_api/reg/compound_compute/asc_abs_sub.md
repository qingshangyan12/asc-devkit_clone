# asc_abs_sub

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

根据mask对源操作数src0、src1进行按元素相减再求绝对值的操作，将结果写入目的操作数dst。

计算公式如下：

$$
dst_i = abs(src0_i - src1_i)
$$

```python
# src0: half[128]  (vector_half, 256B = 128个half元素)
# src1: half[128]  (vector_half)
# dst:  half[128]  (vector_half)
# mask: bool[128]  (b16位宽)
#
# 计算两个源操作数差的绝对值: dst = |src0 - src1|, mask为0的元素置零

def asc_abs_sub(dst, src0, src1, mask):
    for i in range(128):
        if mask[i]:
            dst[i] = abs(src0[i] - src1[i])
        else:
            dst[i] = 0
```

## 函数原型

### 模板原型（占位符形式）

本接口使用`<dtype>`表示支持的数据类型，模板原型如下：

```c
__simd_callee__ inline void asc_abs_sub(vector_<dtype>& dst,
                                        vector_<dtype> src0,
                                        vector_<dtype> src1,
                                        vector_bool mask)
```

#### dtype支持数据类型

dtype支持的数据类型为`half`、`float`

### 典型示例

```c
__simd_callee__ inline void asc_abs_sub(vector_float& dst,
                                        vector_float src0,
                                        vector_float src1,
                                        vector_bool mask)
```

## 参数说明

| 参数名 | 输入/输出 | 描述                                                                                                             |
| ------ | --------- | ---------------------------------------------------------------------------------------------------------------- |
| dst    | 输出      | 目的操作数（矢量数据寄存器）。                                                                                   |
| src0   | 输入      | 源操作数（矢量数据寄存器）。                                                                                     |
| src1   | 输入      | 源操作数（矢量数据寄存器）。                                                                                     |
| mask   | 输入      | 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 约束说明

### 通用约束

- dst、src0和src1的数据类型需要保持一致。
- 掩码位为0时，dst对应元素置0。

## 调用示例

```cpp
__simd_vf__ inline void abs_sub_vf(__ubuf__ half* src0_addr, __ubuf__ half* src1_addr, __ubuf__ half* dst_addr, uint32_t count, uint16_t one_repeat_size, uint16_t repeat_time)
{
    vector_half src0, src1, dst;
    vector_bool mask;
    for (uint16_t i = 0; i < repeat_time; ++i) {
        mask = asc_update_mask_b16(count);
        asc_loadalign_postupdate(src0, src0_addr, one_repeat_size);
        asc_loadalign_postupdate(src1, src1_addr, one_repeat_size);
        asc_abs_sub(dst, src0, src1, mask);
        asc_storealign_postupdate(dst_addr, dst, one_repeat_size, mask);
    }
}
```
