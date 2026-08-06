# asc_xor

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

asc_xor支持两种接口：

- 对矢量数据寄存器操作：
    根据mask对矢量数据寄存器src0和src1进行按位异或（^）操作，将结果写入dst。未被mask筛选的位置被置为0。
- 对掩码寄存器操作：
    根据mask对两个掩码寄存器src0和src1进行按位异或（^）操作，将结果写入dst。未被mask筛选的位置被置为0。

## 函数原型

- 操作数为矢量数据寄存器

```cpp
__simd_callee__ inline void asc_xor(vector_<dtype>& dst,
                                    vector_<dtype> src0,
                                    vector_<dtype> src1,
                                    vector_bool mask)
```

dtype支持的数据类型为`bool`、`uint8_t`、`int8_t`、`uint16_t`、`int16_t`、`uint32_t`、`int32_t`

- 操作数为掩码寄存器

```cpp
__simd_callee__ inline void asc_xor(vector_bool& dst,
                                    vector_bool src0,
                                    vector_bool src1,
                                    vector_bool mask)
```

- 典型示例

```cpp
// 示例：对int32_t矢量数据寄存器执行按位异或
__simd_callee__ inline void asc_xor(vector_int32_t& dst,
                                    vector_int32_t src0,
                                    vector_int32_t src1,
                                    vector_bool mask)
```

## 参数说明

**表1** 参数说明（操作数为矢量数据寄存器）

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dst | 输出 | 目的操作数（矢量数据寄存器）。 |
| src0 | 输入 | 源操作数（矢量数据寄存器）。 |
| src1 | 输入 | 源操作数（矢量数据寄存器）。 |
| mask | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 |

**表2** 参数说明（操作数为掩码寄存器）

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dst | 输出 | 目的操作数（掩码寄存器）。 |
| src0 | 输入 | 源操作数（掩码寄存器）。 |
| src1 | 输入 | 源操作数（掩码寄存器）。 |
| mask | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 约束说明

dst中未被mask筛选的位置被置为0。


### 通用约束

- 本接口仅在AIV上生效，非AIV调用直接返回。
- src0、src1、dst三者dtype必须完全一致，否则触发数据类型异常。
- mask必须为`vector_bool`类型掩码寄存器，需通过掩码设置接口（如`asc_update_mask`系列）预先赋值后再传入；未赋值的掩码寄存器内容不确定，会导致有效元素位置错误。
- mask比特位按元素大小步进过滤：矢量数据寄存器按位异或中每个元素占用`sizeof(dtype)`个比特位，元素i的有效mask比特位于`i × sizeof(dtype)`位置；掩码寄存器按位异或中每个比特独立对应1个掩码位。
- 参与计算的元素个数由矢量长度（VL）决定，矢量数据寄存器按位异或中元素个数=VL÷sizeof(dtype)；掩码寄存器按位异或中比特个数=VL。
- mask比特位为0的位置采用清零模式（ZEROING）：dst对应位置写0，不保留src0、src1原值也不保留dst原值。
- 按位异或按元素的二进制位逐位执行，无溢出、无进位、无NaN/Inf异常上报。

### 矢量数据寄存器按位异或约束

- dtype仅支持uint8_t/int8_t/uint16_t/int16_t/uint32_t/int32_t共6种整数矢量数据寄存器类型，浮点dtype（half/bfloat16_t/float等）与其他未暴露重载的类型触发数据类型异常。

### 掩码寄存器按位异或约束

- dst、src0、src1必须均为`vector_bool`类型，与其他矢量数据寄存器类型混用触发数据类型异常。
- 掩码寄存器按位异或用于复合掩码生成：典型用法是将两个由`asc_update_mask`系列生成的源掩码按位异或，得到仅在两个源掩码中条件不一致的位置为真的复合掩码，再传给后续计算接口。

## 调用示例

- 对矢量数据寄存器操作

    ```cpp
    __simd_vf__ inline void xor_vf(__ubuf__ int8_t* dst_addr, __ubuf__ int8_t* src0_addr, __ubuf__ int8_t* src1_addr, uint32_t count, uint16_t one_repeat_size, uint16_t repeat_time)
    {
        vector_int8_t dst;
        vector_int8_t src0;
        vector_int8_t src1;
        vector_bool mask;
        for (uint16_t i = 0; i < repeat_time; ++i) {
            mask = asc_update_mask_b8(count);
            asc_loadalign(src0, src0_addr + i * one_repeat_size);
            asc_loadalign(src1, src1_addr + i * one_repeat_size);
            asc_xor(dst, src0, src1, mask);
            asc_storealign(dst_addr + i * one_repeat_size, dst, mask);
        }
    }
    ```

- 对掩码寄存器操作

    ```cpp
    __simd_vf__ inline void xor_vf(__ubuf__ int8_t* dst_addr, __ubuf__ int8_t* src_addr, uint32_t count, uint32_t one_repeat_size, uint16_t repeat_time)
    {
        vector_int8_t src;
        vector_bool dst;
        vector_bool mask = asc_create_mask_b8(PAT_ALL);
        vector_bool mask0 = asc_create_mask_b8(PAT_ALL);
        vector_bool mask1 = asc_create_mask_b8(PAT_ALLF);
        asc_xor(dst, mask0, mask1, mask);
        for (uint16_t i = 0; i < repeat_time; ++i) {
            asc_loadalign(src, src_addr + i * one_repeat_size);
            asc_add_scalar(src, src, 0, dst);
            asc_storealign(dst_addr + i * one_repeat_size, src, mask);
        }
    }
    ```
