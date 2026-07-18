# asc_hif82half

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

将src中hifloat8_t数据类型的元素转换为half数据类型，并将结果写入dst。

由于源操作数和目的操作数位宽比为1:2，每两个hifloat8_t类型的元素中选择一个进行类型转换。

## 函数原型

- 取src的偶数索引元素（索引0, 2, 4, ...）进行转换，mask每bit为1时选取对应位置的hifloat8_t元素。

    ```cpp
    __simd_callee__ inline void asc_hif82half(vector_half& dst, vector_hifloat8_t src, vector_bool mask)
    ```

- 取src的奇数索引元素（索引1, 3, 5, ...）进行转换，mask每bit为1时选取对应位置的hifloat8_t元素。

    ```cpp
    __simd_callee__ inline void asc_hif82half_v2(vector_half& dst, vector_hifloat8_t src, vector_bool mask)
    ```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dst | 输出 | 目的操作数（[矢量数据寄存器](../reg_data_types/data_type_definition.md#矢量数据寄存器)）。 |
| src | 输入 | 源操作数（[矢量数据寄存器](../reg_data_types/data_type_definition.md#矢量数据寄存器)）。 |
| mask | 输入 | 源操作数元素操作的有效指示（[掩码寄存器](../reg_data_types/data_type_definition.md#掩码寄存器)）。mask用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 |

## 返回值说明

无

## 约束说明

mask控制源操作数是否参与计算，源操作数不参与计算的元素在输出对应位置置零。

## 调用示例

```cpp
__simd_vf__ inline void hif82half_vf(__ubuf__ half* dst_addr, __ubuf__ hifloat8_t* src_addr, uint32_t count, uint32_t one_src_repeat_size, uint32_t one_dst_repeat_size, uint16_t repeat_time)
{
    vector_half dst;
    vector_hifloat8_t src;
    vector_bool mask;
    for (uint16_t i = 0; i < repeat_time; ++i) {
        mask = asc_update_mask_b8(count);
        asc_loadalign(src, src_addr + i * one_src_repeat_size);
        asc_hif82half(dst, src, mask);
        asc_storealign(dst_addr + i * one_dst_repeat_size, dst, mask);
    }
}
```
