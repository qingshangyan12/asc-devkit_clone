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

将src0和src1中的元素解交织存入dst0和dst1中。以int8_t数据类型为例，asc_deintlv的实现流程如图1所示：

**图1** 解交织实现流程

![解交织实现流程](../../figures/capi_deintlv_reg.png)

## 函数原型

- 操作数为矢量数据寄存器

    ```cpp
    __simd_callee__ inline void asc_deintlv(vector_<dtype>& dst0,
                                            vector_<dtype>& dst1,
                                            vector_<dtype> src0,
                                            vector_<dtype> src1)
    ```

    支持的数据类型：int8_t、uint8_t、hifloat8_t、fp8_e8m0_t、fp8_e5m2_t、fp8_e4m3fn_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float。

- 操作数为掩码寄存器

    ```cpp
    __simd_callee__ inline void asc_deintlv_b8(vector_bool& dst0,
                                               vector_bool& dst1,
                                               vector_bool src0,
                                               vector_bool src1)
    __simd_callee__ inline void asc_deintlv_b16(vector_bool& dst0,
                                                vector_bool& dst1,
                                                vector_bool src0,
                                                vector_bool src1)
    __simd_callee__ inline void asc_deintlv_b32(vector_bool& dst0,
                                                vector_bool& dst1,
                                                vector_bool src0,
                                                vector_bool src1)
    ```

- 典型示例

```cpp
// 示例：对half矢量数据寄存器执行解交织
__simd_callee__ inline void asc_deintlv(vector_half& dst0,
                                        vector_half& dst1,
                                        vector_half src0,
                                        vector_half src1)
```

## 参数说明

**表1** 参数说明（操作数为矢量数据寄存器）

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dst0 | 输出 | 目的操作数（矢量数据寄存器）。 |
| dst1 | 输出 | 目的操作数（矢量数据寄存器）。 |
| src0 | 输入 | 源操作数（矢量数据寄存器）。 |
| src1 | 输入 | 源操作数（矢量数据寄存器）。 |

**表2** 参数说明（操作数为掩码寄存器）

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dst0 | 输出 | 目的操作数（掩码寄存器）。 |
| dst1 | 输出 | 目的操作数（掩码寄存器）。 |
| src0 | 输入 | 源操作数（掩码寄存器）。 |
| src1 | 输入 | 源操作数（掩码寄存器）。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 约束说明

- src0、src1、dst0、dst1的数据类型需要保持一致。
- src0和src1可以为同一个矢量数据寄存器或掩码寄存器。
- dst0和dst1不能为同一个矢量数据寄存器或掩码寄存器。
- 允许源操作数和目的操作数为同一个矢量数据寄存器，例如asc_deintlv(src0, src1, src0, src1)。


### 通用约束

- 本接口仅在AIV上生效，非AIV调用直接返回。
- 矢量数据解交织模式下，dst0、dst1、src0、src1四者dtype必须完全一致，否则触发数据类型异常。
- 本接口不使用掩码寄存器过滤元素，对全部元素执行解交织，dst0、dst1各写入全部有效元素的一半。
- 同名接口的矢量数据解交织与掩码解交织为参数列表完全不同的功能模式，用户必须按使用场景选择对应重载系列，不可混用。

### 矢量数据解交织约束

- dtype仅支持int8_t/uint8_t/int16_t/uint16_t/int32_t/uint32_t/half/bfloat16_t/float/fp8_e4m3fn_t/fp8_e5m2_t/fp8_e8m0_t/hifloat8_t共13种，其他dtype触发数据类型异常。
- 解交织为双目的操作：dst0与dst1必须为不同的矢量数据寄存器实例，若两者引用同一寄存器将导致写后读冲突，结果未定义。

### 掩码解交织约束

- 源与目的必须均为掩码寄存器（vector_bool），不可与矢量数据寄存器混用，否则触发数据类型异常。
- 解交织粒度由变体名决定：asc_deintlv_b8按1字节粒度解交织、asc_deintlv_b16按2字节粒度解交织、asc_deintlv_b32按4字节粒度解交织。不同粒度变体不可混用。
- 解交织为双目的操作：dst0与dst1必须为不同的掩码寄存器实例，若两者引用同一寄存器将导致写后读冲突，结果未定义。

## 调用示例

```cpp
__simd_vf__ inline void deintlv_vf(__ubuf__ half* dst0_addr, __ubuf__ half* dst1_addr, __ubuf__ half* src0_addr, __ubuf__ half* src1_addr, uint16_t one_repeat_size, uint16_t repeat_time)
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
