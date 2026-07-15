# asc_int162uint8

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

将src中int16_t类型的元素转换为uint8_t类型，并将结果写入dst。支持非饱和与饱和两种模式。

由于src和dst位宽比为2:1（int16_t为16bit、uint8_t为8bit），dst中每2个uint8_t位置为一组，各组内只有1个位置有效。

- asc_int162uint8：非饱和模式，结果写入dst各组内第0个位置，直接截断。mask每2bit为一组，仅组内LSB为1时选取对应位置的int16_t元素。

    ```python
    # 结果写入dst各组内第0个位置
    dst[:] = 0
    dst[0:256:2] = src[0:128].astype(uint8)
    ```

- asc_int162uint8_v2：非饱和模式，结果写入dst各组内第1个位置，直接截断。mask每2bit为一组，仅组内LSB为1时选取对应位置的int16_t元素。

    ```python
    # 结果写入dst各组内第1个位置
    dst[:] = 0
    dst[1:256:2] = src[0:128].astype(uint8)
    ```

- asc_int162uint8_sat：饱和模式，超出[0, 255]范围的值钳位到边界。RegLayout::ZERO，结果写入dst各组内第0个位置。mask每2bit为一组、仅组内LSB为1时选取对应位置的int16_t元素。

    ```python
    # 饱和模式，钳位到[0, 255]
    src_clamped = src.clamp(0, 255)
    # 结果写入dst各组内第0个位置
    dst[:] = 0
    dst[0:256:2] = src_clamped[0:128].astype(uint8)
    ```

- asc_int162uint8_sat_v2：饱和模式，超出[0, 255]范围的值钳位到边界。RegLayout::ONE，结果写入dst各组内第1个位置。mask每2bit为一组、仅组内LSB为1时选取对应位置的int16_t元素。

    ```python
    # 饱和模式，钳位到[0, 255]
    src_clamped = src.clamp(0, 255)
    # 结果写入dst各组内第1个位置
    dst[:] = 0
    dst[1:256:2] = src_clamped[0:128].astype(uint8)
    ```

## 函数原型

- 非饱和模式

    ```cpp
    __simd_callee__ inline void asc_int162uint8(vector_uint8_t& dst, vector_int16_t src, vector_bool mask)
    __simd_callee__ inline void asc_int162uint8_v2(vector_uint8_t& dst, vector_int16_t src, vector_bool mask)
    ```

- 饱和模式

    ```cpp
    __simd_callee__ inline void asc_int162uint8_sat(vector_uint8_t& dst, vector_int16_t src, vector_bool mask)
    __simd_callee__ inline void asc_int162uint8_sat_v2(vector_uint8_t& dst, vector_int16_t src, vector_bool mask)
    ```

## 参数说明

**表1**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dst | 输出 | 目的操作数（矢量数据寄存器）。<br>类型为[vector_uint8_t](../reg_data_types/data_type_definition.md#矢量数据寄存器)。 |
| src | 输入 | 源操作数（矢量数据寄存器）。<br>类型为[vector_int16_t](../reg_data_types/data_type_definition.md#矢量数据寄存器)。 |
| mask | 输入 | 源操作数元素操作的有效指示（掩码寄存器）。<br>类型为[vector_bool](../reg_data_types/data_type_definition.md#掩码寄存器)。对应位置为1的元素参与类型转换。 |

## 数据类型

**表2**  数据类型

| src | dst |
| --- | --- |
| int16_t | uint8_t |

## 返回值说明

无返回值

## 约束说明

dst中未被mask筛选的位置被置为0。

## 调用示例

```cpp
__simd_vf__ inline void int162uint8_vf(__ubuf__ uint8_t* dst_addr, __ubuf__ int16_t* src_addr, uint32_t count, uint32_t one_repeat_size, uint16_t repeat_time)
{
    vector_uint8_t dst;
    vector_int16_t src;
    vector_bool mask;
    for (uint16_t i = 0; i < repeat_time; ++i) {
        mask = asc_update_mask_b16(count);
        asc_loadalign(src, src_addr + i * one_repeat_size);
        asc_int162uint8(dst, src, mask);
        asc_storealign(dst_addr + i * one_repeat_size, dst, mask);
    }
}
```
