# asc_select

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
<!-- npu="x90" id8 -->
- Kirin X90：不支持
<!-- end id8 -->
<!-- npu="9030" id9 -->
- Kirin 9030：不支持
<!-- end id9 -->

## 功能说明

头文件路径：`"c_api/reg_compute/reg_vector.h"`。

根据mask的比特位值，从源操作数src0，src1中选择元素，得到目的操作数dst。选择的规则为：当mask的比特位为1时，从src0中选取对应位置的数；当mask的比特位为0时，从src1中选取对应位置的数。

计算公式如下：

$$
dst_i = (mask_i == 1)\ ?\ src0_i : src1_i
$$

## 函数原型

```cpp
__simd_callee__ inline void asc_select(vector_bool& dst, vector_bool src0, vector_bool src1, vector_bool mask)
__simd_callee__ inline void asc_select(vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask)
__simd_callee__ inline void asc_select(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask)
__simd_callee__ inline void asc_select(vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src0, vector_fp8_e4m3fn_t src1, vector_bool mask)
__simd_callee__ inline void asc_select(vector_hifloat8_t& dst, vector_hifloat8_t src0, vector_hifloat8_t src1, vector_bool mask)
__simd_callee__ inline void asc_select(vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src0, vector_fp8_e5m2_t src1, vector_bool mask)
__simd_callee__ inline void asc_select(vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask)
__simd_callee__ inline void asc_select(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask)
__simd_callee__ inline void asc_select(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask)
__simd_callee__ inline void asc_select(vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask)
__simd_callee__ inline void asc_select(vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask)
__simd_callee__ inline void asc_select(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask)
__simd_callee__ inline void asc_select(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask)
```

## 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst | 输出 | 目的操作数（掩码寄存器或矢量数据寄存器）。 |
| src0 | 输入 | 源操作数0（掩码寄存器或矢量数据寄存器）。 |
| src1 | 输入 | 源操作数1（掩码寄存器或矢量数据寄存器）。 |
| mask | 输入 | 源操作数掩码（掩码寄存器）。指定选择src0或src1为有效数据。mask的比特位为1时，选取src0；mask的比特位为0时，选取src1。 |

矢量数据寄存器和掩码寄存器的详细说明请参见 [reg数据类型定义](../reg数据类型定义.md)。


## 数据类型支持

| 输入A类型 (src0) | 输入B类型 (src1) | 输出类型 (dst) |
| :----------------- | :----------------- | :------------- |
| bool | bool | bool |
| uint8_t | uint8_t | uint8_t |
| int8_t | int8_t | int8_t |
| fp8_e4m3fn_t | fp8_e4m3fn_t | fp8_e4m3fn_t |
| hifloat8_t | hifloat8_t | hifloat8_t |
| fp8_e5m2_t | fp8_e5m2_t | fp8_e5m2_t |
| uint16_t | uint16_t | uint16_t |
| int16_t | int16_t | int16_t |
| half | half | half |
| bfloat16_t | bfloat16_t | bfloat16_t |
| uint32_t | uint32_t | uint32_t |
| int32_t | int32_t | int32_t |
| float | float | float |


## 返回值说明

无返回值。

## 流水类型

PIPE_V

## 约束说明

- 通过mask参数控制的未选中的元素保持源操作数的值而非清零。

## 理论性能


| 输入类型 | 输出类型 | 理论并行度(element/cycle) |
|---|---|---|
| bool | bool | 256 |
| int8_t | int8_t | 256 |
| uint8_t | uint8_t | 256 |
| fp8_e4m3fn_t | fp8_e4m3fn_t | 256 |
| hifloat8_t | hifloat8_t | 256 |
| fp8_e5m2_t | fp8_e5m2_t | 256 |
| int16_t | int16_t | 128 |
| uint16_t | uint16_t | 128 |
| half | half | 128 |
| bfloat16_t | bfloat16_t | 128 |
| int32_t | int32_t | 64 |
| uint32_t | uint32_t | 64 |
| float | float | 64 |

## 调用示例

```cpp
vector_half dst;
vector_half src0;
vector_half src1;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
asc_loadalign(src0, src0_addr); // src0_addr是外部输入的UB内存空间地址。
asc_loadalign(src1, src1_addr); // src1_addr是外部输入的UB内存空间地址。
// mask为源操作数掩码
asc_select(dst, src0, src1, mask);
```