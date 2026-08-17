# asc_bfloat162float

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

将bfloat16_t类型转化为float类型，无舍入模式。

由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个VL大小的数据分为两部分，根据不同接口选择输入数据索引为奇数的位置或偶数的位置。

参考伪代码如下：

```python
def asc_bfloat162float(dst, src, mask, src_pos):
    for i in range(64):
        if mask[i]:
            dst[2 * i] = bfloat16(src[i])   # float -> bfloat16, RINT舍入, 写入偶数索引
        else:
            dst[2 * i] = 0
    for i in range(64):
        dst[2 * i + 1] = 0                  # 奇数索引置零
    for i in range(64):
        dst[2 * i] = 0                      # 偶数索引置零
        if mask[i]:
            dst[2 * i + 1] = bfloat16(src[i])  # float -> bfloat16, RINT舍入, 写入奇数索引
        else:
            dst[2 * i + 1] = 0
```

## 函数原型

```c
__simd_callee__ inline void asc_bfloat162float(vector_float& dst,
                                               vector_bfloat16_t src,
                                               vector_bool mask,
                                               std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
```

定义在 C:/Users/admin/asc-devkit_7540/include/c_api/utils/enum.h:35：

  enum class asc_position_mode { EVEN = 0, ODD };

  constexpr std::integral_constant<
      asc_position_mode, asc_position_mode::EVEN> ASC_POSITION_EVEN;

  ASC_POSITION_EVEN 是一个 constexpr 标签对象，而非运行时枚举参数。

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| dst | 输出 | 目的操作数（矢量数据寄存器）。 |
| src | 输入 | 源操作数（矢量数据寄存器）。 |
| mask | 输入 | 源操作数掩码（掩码寄存器）。用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 约束说明

- 本接口仅在AIV上生效，非AIV调用直接返回。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用，src、dst为矢量数据寄存器，不涉及UB地址，无UB容量与地址对齐约束。
- src的dtype必须为`bfloat16_t`，dst的dtype必须为`float`，dtype固定不可混用，否则触发异常。
- mask为`vector_bool`类型掩码寄存器，需通过掩码设置接口预先赋值后再传入；未赋值的掩码寄存器内容不确定，会导致有效元素位置错误。

## 调用示例

```cpp
__simd_vf__ inline void bfloat162float_vf(__ubuf__ float* dst_addr, __ubuf__ bfloat16_t* src_addr, int32_t src_repeat_size, int32_t dst_repeat_size, uint16_t repeat_time)
{
    vector_bfloat16_t src;
    vector_float dst;
    vector_bool mask = asc_create_mask_b16(PAT_ALL);
    for (uint16_t i = 0; i < repeat_time; ++i) {
        asc_loadalign_postupdate(src, src_addr, src_repeat_size);
        asc_bfloat162float(dst, src, mask);
        asc_storealign_postupdate(dst_addr, dst, dst_repeat_size, mask);
    }
}
```
