# asc_deintlv

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

将src0和src1中的元素解交织存入dst0和dst1中。解交织操作的流程如下图所示，每个方格代表一个数据类型为int8_t的元素：

![](../../figures/capi_deintlv_reg.png)

## 函数原型

- 操作数为矢量数据寄存器

    ```cpp
    __simd_callee__ inline void asc_deintlv(vector_int8_t& dst0, vector_int8_t& dst1, vector_int8_t src0, vector_int8_t src1)
    __simd_callee__ inline void asc_deintlv(vector_uint8_t& dst0, vector_uint8_t& dst1, vector_uint8_t src0, vector_uint8_t src1)
    __simd_callee__ inline void asc_deintlv(vector_hifloat8_t& dst0, vector_hifloat8_t& dst1, vector_hifloat8_t src0, vector_hifloat8_t src1)
    __simd_callee__ inline void asc_deintlv(vector_fp8_e8m0_t& dst0, vector_fp8_e8m0_t& dst1, vector_fp8_e8m0_t src0, vector_fp8_e8m0_t src1)
    __simd_callee__ inline void asc_deintlv(vector_fp8_e5m2_t& dst0, vector_fp8_e5m2_t& dst1, vector_fp8_e5m2_t src0, vector_fp8_e5m2_t src1)
    __simd_callee__ inline void asc_deintlv(vector_fp8_e4m3fn_t& dst0, vector_fp8_e4m3fn_t& dst1, vector_fp8_e4m3fn_t src0, vector_fp8_e4m3fn_t src1)
    __simd_callee__ inline void asc_deintlv(vector_int16_t& dst0, vector_int16_t& dst1, vector_int16_t src0, vector_int16_t src1)
    __simd_callee__ inline void asc_deintlv(vector_uint16_t& dst0, vector_uint16_t& dst1, vector_uint16_t src0, vector_uint16_t src1)
    __simd_callee__ inline void asc_deintlv(vector_half& dst0, vector_half& dst1, vector_half src0, vector_half src1)
    __simd_callee__ inline void asc_deintlv(vector_bfloat16_t& dst0, vector_bfloat16_t& dst1, vector_bfloat16_t src0, vector_bfloat16_t src1)
    __simd_callee__ inline void asc_deintlv(vector_int32_t& dst0, vector_int32_t& dst1, vector_int32_t src0, vector_int32_t src1)
    __simd_callee__ inline void asc_deintlv(vector_uint32_t& dst0, vector_uint32_t& dst1, vector_uint32_t src0, vector_uint32_t src1)
    __simd_callee__ inline void asc_deintlv(vector_float& dst0, vector_float& dst1, vector_float src0, vector_float src1)
    ```

- 操作数为掩码寄存器

    ```cpp
    __simd_callee__ inline void asc_deintlv_b8(vector_bool& dst0, vector_bool& dst1, vector_bool src0, vector_bool src1)
    __simd_callee__ inline void asc_deintlv_b16(vector_bool& dst0, vector_bool& dst1, vector_bool src0, vector_bool src1)
    __simd_callee__ inline void asc_deintlv_b32(vector_bool& dst0, vector_bool& dst1, vector_bool src0, vector_bool src1)
    ```

## 参数说明

**表1**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dst0 | 输出 | 目的操作数（矢量数据寄存器）。<br>类型为[vector_[data_type]](../reg_data_types/data_type_definition.md#矢量数据寄存器)，data_type的取值范围请参考[数据类型](#数据类型)。 |
| dst1 | 输出 | 目的操作数（矢量数据寄存器）。<br>类型为[vector_[data_type]](../reg_data_types/data_type_definition.md#矢量数据寄存器)，data_type的取值范围请参考[数据类型](#数据类型)。 |
| src0 | 输入 | 源操作数（矢量数据寄存器）。<br>类型为[vector_[data_type]](../reg_data_types/data_type_definition.md#矢量数据寄存器)，data_type的取值范围请参考[数据类型](#数据类型)。 |
| src1 | 输入 | 源操作数（矢量数据寄存器）。<br>类型为[vector_[data_type]](../reg_data_types/data_type_definition.md#矢量数据寄存器)，data_type的取值范围请参考[数据类型](#数据类型)。 |

## 数据类型

目的操作数与源操作数的数据类型需要保持一致。支持的数据类型为：bool、int8_t、uint8_t、hifloat8_t、fp8_e8m0_t、fp8_e5m2_t、fp8_e4m3fn_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float。

## 返回值说明

无

## 约束说明

- src0、src1、dst0、dst1的数据类型需要保持一致。
- src0和src1可以为同一个RegTensor。
- dst0和dst1不能为同一个RegTensor。
- 允许源操作数和目的操作数为同一个矢量数据寄存器，例如asc_deintlv(src0, src1, src0, src1)。

## 调用示例

```cpp
constexpr uint32_t one_repeat_size = 256 / sizeof(half);
uint32_t total_length = 256;
uint32_t repeat_time = total_length / one_repeat_size;
__simd_vf__ inline void deintlv_vf(__ubuf__ half* dst0_addr, __ubuf__ half* dst1_addr, __ubuf__ half* src0_addr, __ubuf__ half* src1_addr, uint32_t one_repeat_size, uint16_t repeat_time)
{
    vector_half dst0;
    vector_half dst1;
    vector_half src0;
    vector_half src1;
    vector_bool mask = asc_create_mask_b16(PAT_ALL);
    for (uint16_t i = 0; i < repeat_time; ++i) {
        asc_loadalign(src0, src0_addr + i * one_repeat_size);
        asc_loadalign(src1, src1_addr + i * one_repeat_size);
        asc_deintlv(dst0, dst1, src0, src1);
        asc_storealign(dst0_addr + i * one_repeat_size, dst0, mask);
        asc_storealign(dst1_addr + i * one_repeat_size, dst1, mask);
    }
}
```