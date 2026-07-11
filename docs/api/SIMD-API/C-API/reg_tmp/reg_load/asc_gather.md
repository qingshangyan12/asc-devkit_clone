# asc_gather

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->

## 功能说明

头文件路径：`"c_api/reg_compute/reg_load.h"`。

根据索引位置index将源操作数src按元素收集到目的操作数dst中。

![图1 asc_gather功能说明](../../../../figures/asc_gather.png)

## 函数原型

<!-- npu="950" id2 -->
```cpp
// 收集UB中的元素
__simd_callee__ inline void asc_gather(vector_int16_t& dst, __ubuf__ int8_t* src, vector_uint16_t index, vector_bool mask)
__simd_callee__ inline void asc_gather(vector_uint16_t& dst, __ubuf__ uint8_t* src, vector_uint16_t index, vector_bool mask)
__simd_callee__ inline void asc_gather(vector_int16_t& dst, __ubuf__ int16_t* src, vector_uint16_t index, vector_bool mask)
__simd_callee__ inline void asc_gather(vector_uint16_t& dst, __ubuf__ uint16_t* src, vector_uint16_t index, vector_bool mask)
__simd_callee__ inline void asc_gather(vector_int32_t& dst, __ubuf__ int32_t* src, vector_uint32_t index, vector_bool mask)
__simd_callee__ inline void asc_gather(vector_uint32_t& dst, __ubuf__ uint32_t* src, vector_uint32_t index, vector_bool mask)
__simd_callee__ inline void asc_gather(vector_half& dst, __ubuf__ half* src, vector_uint16_t index, vector_bool mask)
__simd_callee__ inline void asc_gather(vector_float& dst, __ubuf__ float* src, vector_uint32_t index, vector_bool mask)
__simd_callee__ inline void asc_gather(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, vector_uint16_t index, vector_bool mask)
__simd_callee__ inline void asc_gather(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, vector_uint16_t index, vector_bool mask)
__simd_callee__ inline void asc_gather(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, vector_uint16_t index, vector_bool mask)
__simd_callee__ inline void asc_gather(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, vector_uint16_t index, vector_bool mask)
__simd_callee__ inline void asc_gather(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, vector_uint16_t index, vector_bool mask)
__simd_callee__ inline void asc_gather(vector_int16_t& dst, __ubuf__ int16_t* src, vector_uint32_t index, vector_bool mask)
__simd_callee__ inline void asc_gather(vector_uint16_t& dst, __ubuf__ uint16_t* src, vector_uint32_t index, vector_bool mask)
__simd_callee__ inline void asc_gather(vector_half& dst, __ubuf__ half* src, vector_uint32_t index, vector_bool mask)
__simd_callee__ inline void asc_gather(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, vector_uint32_t index, vector_bool mask)

// 收集矢量数据寄存器中的元素
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
<!-- end id2 -->

## 参数说明

**表 2**  参数说明

<!-- npu="950" id3 -->
| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `dst` | 输出 | 目的操作数（矢量数据寄存器）。 |
| `src` | 输入 | 源操作数（矢量）的起始地址，或源操作数（矢量数据寄存器）。 |
| `index` | 输入 | 数据索引（矢量数据寄存器），dst中每个元素在UB中相对于src的索引位置。 |
| `mask` | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 |
<!-- end id3 -->

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义.md](../reg数据类型定义.md)。

## 数据类型支持

<!-- npu="950" id4 -->
目的操作数与源操作数的数据类型需要保持一致。支持的数据类型为：`vector_int8_t`、`vector_uint8_t`、`vector_int16_t`、`vector_uint16_t`、`vector_int32_t`、`vector_uint32_t`、`vector_half`、`vector_bfloat16_t`、`vector_fp8_e4m3fn_t`、`vector_fp8_e5m2_t`、`vector_fp8_e8m0_t`、`vector_hifloat8_t`、`vector_float`。
<!-- end id4 -->

## 返回值说明

无

## 流水类型

PIPE_V

## 理论性能

| 数据类型 | 理论并行度（element/cycle） | 
|---|---|
| `vector_int8_t` | <TODO> |
| `vector_uint8_t` | <TODO> |
| `vector_int16_t` | <TODO> |
| `vector_uint16_t` | <TODO> |
| `vector_int32_t` | <TODO> |
| `vector_uint32_t` | <TODO> |
| `vector_half` | <TODO> |
| `vector_bfloat16_t` | <TODO> |
| `vector_fp8_e4m3fn_t` | <TODO> |
| `vector_fp8_e5m2_t` | <TODO> |
| `vector_fp8_e8m0_t` | <TODO> |
| `vector_hifloat8_t` | <TODO> |
| `vector_float` | <TODO> |

## 约束说明

<!-- npu="950" id5 -->
- 源操作数在UB中的起始地址需要32B对齐。
- 收集UB中的元素时，对于mask未筛选的元素，对应的地址不会触发任何地址溢出异常，同时dst中对应的元素将被置零。
- 收集UB中的元素：当src的数据类型为b8数据类型时，dst为b16数据类型，目的操作数的低8位与源操作数相同，高8位自动补0。当src的数据类型为b16数据类型时，如果dst为b16数据类型，目的操作数与源操作数相同；如果dst为b32数据类型，目的操作数的低16位与源操作数相同，高16位自动补0。
- 收集矢量数据寄存器中的元素：如果index中索引值超出当前矢量数据寄存器中能存储的最大元素个数时，索引值更新为`i % vl_length`。
<!-- end id5 -->

## 调用示例

<!-- npu="950" id6 -->
```cpp
vector_half dst;
__ubuf__ half* src = (__ubuf__ half*)asc_get_phy_buf_addr(0);
vector_uint16_t index;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
asc_loadalign(index, index_addr); // index_addr是外部输入的UB内存空间地址。
asc_gather(dst, src, index, mask);
```
<!-- end id6 -->
