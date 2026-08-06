# asc_unpack

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

将src中低半部分或高半部分的元素以扩充位宽的方式写入dst，支持：

- asc_unpack_lower：取src的低半部分（LOWER），每个元素高位填0扩充位宽后写入dst。
- asc_unpack_upper：取src的高半部分（HIGHER），每个元素高位填0扩充位宽后写入dst。

不同数据类型的扩充方式：

- 无符号整型：高位填0扩充。例如vector_uint8_t到vector_uint16_t，src中每个uint8_t元素高位补0扩展为uint16_t。
- 有符号整型：保持符号位扩充。例如vector_int8_t到vector_int16_t，src中每个int8_t元素按符号位扩展为int16_t。
- bool类型（掩码寄存器）：将每bit展开为2bit，高位填0。

矢量数据寄存器unpack流程如图1所示：

**图1** 矢量数据寄存器unpack流程

![矢量数据寄存器unpack流程](../../figures/capi_unpack_reg.png)

掩码数据寄存器unpack流程如图2所示：

**图2** 掩码数据寄存器unpack流程

![掩码数据寄存器unpack流程](../../figures/capi_unpack_mask.png)

## 函数原型

- 操作数为矢量数据寄存器

    ```c
    // 解压缩低位
    __simd_callee__ inline void asc_unpack_lower(vector_<dst_dtype>& dst,
                                                 vector_<src_dtype> src)
    // 解压缩高位
    __simd_callee__ inline void asc_unpack_upper(vector_<dst_dtype>& dst,
                                                 vector_<src_dtype> src)
    ```

    支持的数据类型组合：

    | src_dtype | dst_dtype |
    | :--- | :--- |
    | int8_t | int16_t |
    | uint8_t | uint16_t |
    | int16_t | int32_t |
    | uint16_t | uint32_t |

- 操作数为掩码寄存器

    ```c
    // 解压缩低位
    __simd_callee__ inline void asc_unpack_lower(vector_bool& dst,
                                                 vector_bool src)
    // 解压缩高位
    __simd_callee__ inline void asc_unpack_upper(vector_bool& dst,
                                                 vector_bool src)
    ```

- 典型示例

```c
// 示例：对uint8_t矢量数据寄存器解压缩到uint16_t矢量数据寄存器
__simd_callee__ inline void asc_unpack_lower(vector_uint16_t& dst,
                                             vector_uint8_t src)
```

## 参数说明

**表1** 参数说明（操作数为矢量数据寄存器）

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dst | 输出 | 目的操作数（矢量数据寄存器）。 |
| src | 输入 | 源操作数（矢量数据寄存器）。 |

**表2** 参数说明（操作数为掩码寄存器）

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dst | 输出 | 目的操作数（掩码寄存器）。 |
| src | 输入 | 源操作数（掩码寄存器）。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 约束说明

- 本接口仅在AIV上生效，非AIV调用直接返回。
- dst与src必须按函数原型中列出的5种固定dtype配对组合使用。
- 低半段解包与高半段解包配合可分别解包源寄存器前半段与后半段，两次调用即可将整个源寄存器的窄类型数据全部扩展为宽类型写入两个目的寄存器。
- dst全部有效位置被改写为src元素扩展后的结果，不保留dst调用前的旧值；调用方若需保留dst原值，应在调用前自行暂存。

## 调用示例

- 对矢量数据寄存器操作

    ```cpp
    __simd_vf__ inline void unpack_vf(__ubuf__ uint32_t* dst_addr, __ubuf__ uint16_t* src_addr, uint16_t one_src_repeat_size, uint16_t one_dst_repeat_size, uint16_t repeat_time)
    {
        vector_uint32_t dst;
        vector_uint16_t src;
        vector_bool mask = asc_create_mask_b32(PAT_ALL);
        for (uint16_t i = 0; i < repeat_time; ++i) {
            asc_loadalign(src, src_addr + i * one_src_repeat_size);
            asc_unpack_lower(dst, src);
            asc_storealign(dst_addr + i * one_dst_repeat_size, dst, mask);
        }
    }
    ```

- 对掩码寄存器操作

    ```cpp
    __simd_vf__ inline void unpack_vf(__ubuf__ uint8_t* dst_addr, __ubuf__ uint8_t* src_addr, uint32_t count, uint32_t one_repeat_size, uint16_t repeat_time)
    {
        vector_uint8_t src;
        vector_bool mask0 = asc_create_mask_b8(PAT_ALL);
        vector_bool mask1;
        asc_unpack_lower(mask1, mask0);
        for (uint16_t i = 0; i < repeat_time; ++i) {
            asc_loadalign(src, src_addr + i * one_repeat_size);
            asc_add_scalar(src, src, 0, mask1);
            asc_storealign(dst_addr + i * one_repeat_size, src, mask1);
        }
    }
    ```
