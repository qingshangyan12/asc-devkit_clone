# asc_gather

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

头文件路径：`"c_api/reg_compute/reg_load.h"`。

根据索引位置index将源操作数src按元素收集到目的操作数dst中。

**图 1**  收集UB中的元素 
![收集UB中的元素](../../figures/asc_gather_ub.png)

**图 2**  收集矢量数据寄存器中的元素 
![收集矢量数据寄存器中的元素](../../figures/asc_gather_vec.png)

## 函数原型

- 收集UB中的元素

    ```cpp
    __simd_callee__ inline void asc_gather(vector_int16_t& dst, __ubuf__ int8_t* src, vector_uint16_t index, vector_bool mask)
    __simd_callee__ inline void asc_gather(vector_uint16_t& dst, __ubuf__ uint8_t* src, vector_uint16_t index, vector_bool mask)
    __simd_callee__ inline void asc_gather(vector_int16_t& dst, __ubuf__ int16_t* src, vector_uint16_t index, vector_bool mask)
    __simd_callee__ inline void asc_gather(vector_uint16_t& dst, __ubuf__ uint16_t* src, vector_uint16_t index, vector_bool mask)
    __simd_callee__ inline void asc_gather(vector_int32_t& dst, __ubuf__ int32_t* src, vector_uint32_t index, vector_bool mask)
    __simd_callee__ inline void asc_gather(vector_uint32_t& dst, __ubuf__ uint32_t* src, vector_uint32_t index, vector_bool mask)
    __simd_callee__ inline void asc_gather(vector_half& dst, __ubuf__ half* src, vector_uint16_t index, vector_bool mask)
    __simd_callee__ inline void asc_gather(vector_float& dst, __ubuf__ float* src, vector_uint32_t index, vector_bool mask)
    __simd_callee__ inline void asc_gather(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, vector_uint16_t index, vector_bool mask)
    __simd_callee__ inline void asc_gather(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, vector_uint16_t index,vector_bool mask)
    __simd_callee__ inline void asc_gather(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, vector_uint16_t index, vector_bool mask)
    __simd_callee__ inline void asc_gather(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, vector_uint16_t index, vector_bool mask)
    __simd_callee__ inline void asc_gather(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, vector_uint16_t index, vector_bool mask)
    __simd_callee__ inline void asc_gather(vector_int16_t& dst, __ubuf__ int16_t* src, vector_uint32_t index, vector_bool mask)
    __simd_callee__ inline void asc_gather(vector_uint16_t& dst, __ubuf__ uint16_t* src, vector_uint32_t index, vector_bool mask)
    __simd_callee__ inline void asc_gather(vector_half& dst, __ubuf__ half* src, vector_uint32_t index, vector_bool mask)
    __simd_callee__ inline void asc_gather(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, vector_uint32_t index, vector_bool mask)
    ```

- 收集矢量数据寄存器中的元素

    ```cpp
    __simd_callee__ inline void asc_gather(vector_int8_t& dst, vector_int8_t src, vector_uint8_t index)
    __simd_callee__ inline void asc_gather(vector_uint8_t& dst, vector_uint8_t src, vector_uint8_t index)
    __simd_callee__ inline void asc_gather(vector_int16_t& dst, vector_int16_t src, vector_uint16_t index)
    __simd_callee__ inline void asc_gather(vector_uint16_t& dst, vector_uint16_t src, vector_uint16_t index)
    __simd_callee__ inline void asc_gather(vector_int32_t& dst, vector_int32_t src, vector_uint32_t index)
    __simd_callee__ inline void asc_gather(vector_uint32_t& dst, vector_uint32_t src, vector_uint32_t index)
    __simd_callee__ inline void asc_gather(vector_half& dst, vector_half src, vector_uint16_t index)
    __simd_callee__ inline void asc_gather(vector_bfloat16_t& dst, vector_bfloat16_t src, vector_uint16_t index)
    __simd_callee__ inline void asc_gather(vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src, vector_uint8_t index)
    __simd_callee__ inline void asc_gather(vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src, vector_uint8_t index)
    __simd_callee__ inline void asc_gather(vector_fp8_e8m0_t& dst, vector_fp8_e8m0_t src, vector_uint8_t index)
    __simd_callee__ inline void asc_gather(vector_hifloat8_t& dst, vector_hifloat8_t src, vector_uint8_t index)
    ```

## 参数说明

- 收集UB中的元素

    | 参数名  | 输入/输出 | 描述 |
    | :----- | :------- | :------- |
    | dst | 输出 | 目的操作数（矢量数据寄存器）。<br>类型为[vector\_\[data\_type\]](../reg_data_types/data_type_definition.md#reg数据类型定义)，data_type的取值范围请参考[数据类型](#数据类型)。 |
    | src | 输入 | 源操作数（矢量）的起始地址。 |
    | index | 输入 | 数据索引（矢量数据寄存器），dst中每个元素在UB中相对于src的索引位置。<br>类型为[vector\_\[data\_type\]](../reg_data_types/data_type_definition.md#reg数据类型定义)，data_type的取值范围请参考[数据类型](#数据类型)。 |
    | mask | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。<br>类型为[vector\_bool](../reg_data_types/data_type_definition.md#掩码寄存器)。 |

- 收集矢量数据寄存器中的元素

    | 参数名  | 输入/输出 | 描述 |
    | :----- | :------- | :------- |
    | dst | 输出 | 目的操作数（矢量数据寄存器）。<br>类型为[vector\_\[data\_type\]](../reg_data_types/data_type_definition.md#reg数据类型定义)，data_type的取值范围请参考[数据类型](#数据类型)。 |
    | src | 输入 | 源操作数（矢量数据寄存器）。<br>类型为[vector\_\[data\_type\]](../reg_data_types/data_type_definition.md#reg数据类型定义)，data_type的取值范围请参考[数据类型](#数据类型)。 |
    | index | 输入 | 数据索引（矢量数据寄存器）。<br>类型为[vector\_\[data\_type\]](../reg_data_types/data_type_definition.md#reg数据类型定义)，data_type的取值范围请参考[数据类型](#数据类型)。 |

## 数据类型

- 收集UB中的元素

| 目的操作数 | 源操作数 | 数据索引 |
|-----|-----|-----|
| int16_t | int8_t | uint16_t |
| uint16_t | uint8_t | uint16_t |
| fp8_e4m3fn_t | fp8_e4m3fn_t | uint16_t |
| fp8_e5m2_t | fp8_e5m2_t | uint16_t |
| fp8_e8m0_t | fp8_e8m0_t | uint16_t |
| hifloat8_t | hifloat8_t | uint16_t |
| int16_t | int16_t | uint16_t |
| uint16_t | uint16_t | uint16_t |
| half | half | uint16_t |
| bfloat16_t | bfloat16_t | uint16_t |
| int32_t | int32_t | uint32_t |
| uint32_t | uint32_t | uint32_t |
| float | float | uint32_t |
| int16_t | int16_t | uint32_t |
| uint16_t | uint16_t | uint32_t |
| half | half | uint32_t |
| bfloat16_t | bfloat16_t | uint32_t |

- 收集矢量数据寄存器中的元素

目的操作数和源操作数的数据类型需要保持一致。支持的数据类型为：int8_t、uint8_t、fp8_e4m3fn_t、fp8_e5m2_t、fp8_e8m0_t、hifloat8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t。

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

- 收集UB中的元素

    - 源操作数在UB中的起始地址需要32B对齐。

    - 对于mask未筛选的元素，对应的地址不会触发任何地址溢出异常，同时dst中对应的元素将被置零。

    - 当src的数据类型为b8数据类型，dst为b16数据类型，这种情况下目的操作数的低8位与源操作数相同，高8位自动补0。例如src为int8_t数据类型：

        40 = 0b00101000 -> 0b0000000000101000,扩充至16位后等于40；

        -40 = 0b11011000 -> 0b0000000011011000，扩充至16位后等于216。

    - 当src的数据类型为b16数据类型，dst为b32数据类型，这种情况下目的操作数的低16位与源操作数相同，高16位自动补0。例如src为int16_t数据类型：

        40 = 0x0028 -> 0x00000028,扩充至32位后等于40；

        -40 = 0xFFD8 -> 0x0000FFD8，扩充至32位后等于65496。

- 收集矢量数据寄存器中的元素

    src为矢量数据寄存器类型，位宽是固定的VL,存储的元素个数固定。如果index中索引值超出当前矢量数据寄存器中能存储的最大元素个数时，按照如下方式处理：设定当前矢量数据寄存器所能存储的最大数据元素个数为vl_length, index中索引值为i，索引值更新为i % vl_length。

## 调用示例

- 收集UB中的元素

    ```cpp
    vector_half dst;
    __ubuf__ half* src = (__ubuf__ half*)asc_get_phy_buf_addr(0);
    vector_uint16_t index;
    vector_bool mask = asc_create_mask_b16(PAT_ALL);
    asc_loadalign(index, index_addr); // index_addr是外部输入的UB内存空间地址。
    asc_gather(dst, src, index, mask);
    ```

- 收集矢量数据寄存器中的元素

    ```cpp
    vector_half dst;
    vector_half src;
    vector_uint16_t index;
    asc_loadalign(src, src_addr); // src_addr是外部输入的UB内存空间地址。
    asc_loadalign(index, index_addr); // index_addr是外部输入的UB内存空间地址。
    asc_gather(dst, src, index);
    ```
