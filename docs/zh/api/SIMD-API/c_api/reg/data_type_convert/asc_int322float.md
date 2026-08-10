# asc_int322float

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

头文件路径：`"c_api/reg_compute/reg_convert.h"`。

将int32_t类型转化为float类型，并支持多种舍入模式。
$$
dst_i = (float)src_i
$$

关于舍入模式的详细说明，请参见[舍入模式](../data_type_convert/rounding_mode.md)。

```python
# src: int32[64]   (vector_int32_t, 256B = 64个int32元素)
# dst: float[64]   (vector_float, 256B = 64个float元素)
# mask: bool[64]   (b32位宽)
#
# 等宽转换(1:1): 逐元素将int32转换为float, 支持多种舍入模式
# 舍入模式后缀: rd(FLOOR)/rn(RINT)/rna(ROUND)/ru(CEIL)/rz(TRUNC)

def asc_int322float_rn(dst, src, mask):
    for i in range(64):
        if mask[i]:
            dst[i] = float(src[i])       # int32 -> float, RINT舍入
        else:
            dst[i] = 0.0                 # mask未选中, 置零
```

## 函数原型

### 模板原型（占位符形式）

本接口包含5种舍入模式，模板原型如下：

```c
__simd_callee__ inline void asc_int322float_<round_mode>(vector_float& dst,
                                                         vector_int32_t src,
                                                         vector_bool mask)
```

`<round_mode>`支持`rd`（FLOOR）、`rn`（RINT）、`rna`（ROUND）、`ru`（CEIL）和`rz`（TRUNC）。

src与dst的dtype固定，src支持的数据类型为`int32_t`，dst支持的数据类型为`float`

### 典型示例

```c
// RINT舍入模式
__simd_callee__ inline void asc_int322float_rn(vector_float& dst,
                                               vector_int32_t src,
                                               vector_bool mask)
```

## 参数说明

**表1**参数说明

| 参数名 | 输入/输出 | 描述                                                                                                             |
| ------ | --------- | ---------------------------------------------------------------------------------------------------------------- |
| dst    | 输出      | 目的操作数（矢量数据寄存器）。                                                                                   |
| src    | 输入      | 源操作数（矢量数据寄存器）。                                                                                     |
| mask   | 输入      | 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 约束说明

### 通用约束

- src与dst的数据类型需要与函数原型匹配。
- 掩码位为0时，dst对应元素置0。
- 当int32_t的绝对值大于2^24时，按函数后缀指定的舍入模式进行舍入。

## 调用示例

```cpp
__simd_vf__ inline void int322float_vf(__ubuf__ int32_t* src_addr, __ubuf__ float* dst_addr, uint32_t count, uint16_t one_repeat_size, uint16_t repeat_time)
{
    vector_int32_t src;
    vector_float dst;
    vector_bool mask;
    for (uint16_t i = 0; i < repeat_time; ++i) {
        mask = asc_update_mask_b32(count);
        asc_loadalign_postupdate(src, src_addr, one_repeat_size);
        asc_int322float_rn(dst, src, mask);
        asc_storealign_postupdate(dst_addr, dst, one_repeat_size, mask);
    }
}
```
