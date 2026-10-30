# asc_scatter (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T03:47:20.712Z pushedAt=2026-10-28T12:55:35.771Z nl=2 -->

## Applicable Products

<!-- npu="950" id1 -->
- Ascend 950PR&950DT products: Supported
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 products: Not supported
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 products: Not supported
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 inference products: Not supported
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas inference product AI Core: Not supported
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas inference product Vector Core: Not supported
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas training products: Not supported
<!-- end id7 -->

## Description

The header file path is `"c_api/reg_compute/scatter/ub_scatter.h"`.

**This API is deprecated. Use [asc_scatter](../reg_compute/scatter/asc_scatter.md) to implement this function.**

This instruction scatters the elements in the source operand src to the destination operand dst based on the index value index. The scattering process is shown in the following figure:

![scatter function description](../../../figures/reg_scatter.png)

## Prototype

```cpp
__simd_callee__ inline void asc_scatter(vector_int8_t& src, __ubuf__ int8_t* dst, vector_uint16_t index, vector_bool mask)
__simd_callee__ inline void asc_scatter(vector_uint8_t& src, __ubuf__ uint8_t* dst, vector_uint16_t index, vector_bool mask)
__simd_callee__ inline void asc_scatter(vector_int16_t& src, __ubuf__ int16_t* dst, vector_uint16_t index, vector_bool mask)
__simd_callee__ inline void asc_scatter(vector_uint16_t& src, __ubuf__ uint16_t* dst, vector_uint16_t index, vector_bool mask)
__simd_callee__ inline void asc_scatter(vector_int32_t& src, __ubuf__ int32_t* dst, vector_uint32_t index, vector_bool mask)
__simd_callee__ inline void asc_scatter(vector_uint32_t& src, __ubuf__ uint32_t* dst, vector_uint32_t index, vector_bool mask)
__simd_callee__ inline void asc_scatter(vector_bfloat16_t& src, __ubuf__ bfloat16_t* dst, vector_uint16_t index, vector_bool mask)
__simd_callee__ inline void asc_scatter(vector_half& src, __ubuf__ half* dst, vector_uint16_t index, vector_bool mask)
__simd_callee__ inline void asc_scatter(vector_float& src, __ubuf__ float* dst, vector_uint32_t index, vector_bool mask)
```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description                                                                      |
|:------| :--- |:------------------------------------------------------------------------|
| src   | Input | Source operand (vector data register).                                                           |
| dst   | Output | Start address of the destination operand (vector).                                                          |
| index | Input | Data index (vector data register).                                                          |
| mask  | Input | Source operand mask (mask register), which indicates the elements that participate in the computation. A value of 1 at the corresponding position indicates participation, and 0 indicates no participation. |

For details about the mask register, see [reg data type definition](../defs/type/data_type_definition.md).

## Return Value

None

## Pipeline Type

PIPE_V

## Constraints

- The start address of dst must be 32B-aligned.
- When dst is of the int8_t or uint8_t data type, only the even-indexed elements in the source operand are valid. That is, the data at even positions [0, 2, 4, ..., 252, 254] in src is scattered and stored into the destination operand.
- The values in index must be unique. If duplicate index values exist, the system retains only one of the corresponding data items and ignores the rest. Since it is impossible to determine which one is retained, ensure that index values are not duplicated.

## Examples

```cpp
// dst is an externally input half pointer to the Unified Buffer (UB) memory space.
vector_half src;
vector_uint16_t index;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
asc_scatter(src, dst, index, mask);
```
