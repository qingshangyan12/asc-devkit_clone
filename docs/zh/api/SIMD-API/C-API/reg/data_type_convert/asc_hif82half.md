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

由于源操作数和目的操作数位宽比为1:2，每两个hifloat8_t类型的元素中选择一个进行类型转换。asc_hif82half选择第1个元素参与转换，asc_hif82half_v2选择第2个元素参与转换。

- asc_hif82half：取src的偶数索引元素（layout=0，即索引0, 2, 4, ...）进行转换，mask每bit为1时选取对应位置的hifloat8_t元素。

    ```python
    # layout = 0，选取第1个（偶数索引）元素
    dst[0:128] = src[0:256:2].astype(float16)
    ```

- asc_hif82half_v2：取src的奇数索引元素（layout=1，即索引1, 3, 5, ...）进行转换，mask每bit为1时选取对应位置的hifloat8_t元素。

    ```python
    # layout = 1，选取第2个（奇数索引）元素
    dst[0:128] = src[1:256:2].astype(float16)
    ```

## 函数原型

- 选择第1个hifloat8_t元素转换

    ```cpp
    __simd_callee__ inline void asc_hif82half(vector_half& dst, vector_hifloat8_t src, vector_bool mask)
    ```

- 选择第2个hifloat8_t元素转换

    ```cpp
    __simd_callee__ inline void asc_hif82half_v2(vector_half& dst, vector_hifloat8_t src, vector_bool mask)
    ```

## 参数说明

**表1**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dst | 输出 | 目的操作数（矢量数据寄存器）。<br>类型为[vector_half](../reg_data_types/data_type_definition.md#矢量数据寄存器)。 |
| src | 输入 | 源操作数（矢量数据寄存器）。<br>类型为[vector_hifloat8_t](../reg_data_types/data_type_definition.md#矢量数据寄存器)。 |
| mask | 输入 | 源操作数元素操作的有效指示（掩码寄存器）。<br>类型为[vector_bool](../reg_data_types/data_type_definition.md#掩码寄存器)。对应位置为1的元素参与类型转换。 |

## 数据类型

**表2**  数据类型

| src | dst |
| --- | --- |
| hifloat8_t | half |

## 返回值说明

无返回值

## 约束说明

dst中未被mask筛选的位置被置为0。

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
