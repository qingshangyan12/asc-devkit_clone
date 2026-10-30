# asc_e4m32float (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T01:33:12.270Z pushedAt=2026-10-28T12:55:18.882Z nl=2 -->

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

The header file path is `"c_api/reg_compute/reg_convert.h"`.

**The prototype of this API without explicit positional parameters is deprecated. Use the overload with explicit positional parameters of [asc_e4m32float](../reg_compute/reg_convert/asc_e4m32float.md) instead.**

Converts the fp8_e4m3fn_t type to the float type.

Because the bit width ratio between the source operand and the destination operand is 1:4, the data of one VL size must be divided into four parts when reading. Select index 0, index 1, index 2, or index 3 according to the API used.

## Prototype

```cpp
// Position for reading data at index 0.
__simd_callee__ inline void asc_e4m32float(vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask)
// Position for reading data at index 1.
__simd_callee__ inline void asc_e4m32float_v2(vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask)
// Position for reading data at index 2.
__simd_callee__ inline void asc_e4m32float_v3(vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask)
// Position for reading data at index 3.
__simd_callee__ inline void asc_e4m32float_v4(vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask)
```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| :------ | :----- | :----------------- |
| dst    | Output    | Destination operand (vector data register). |
| src    | Input    | Source operand (vector data register). |
| mask | Input | Source operand mask (mask register), which indicates the elements that participate in the computation. Elements with the corresponding bit set to 1 participate in the computation, and those with the bit set to 0 do not. Elements not selected by mask are set to zero in the output. |

For details about vector data registers and mask registers, see [reg data type definition](../defs/type/data_type_definition.md).

## Return Value

None

## Pipeline Type

PIPE_V

## Constraints

None

## Examples

```cpp
vector_fp8_e4m3fn_t src;
vector_float dst;
vector_bool mask = asc_create_mask_b8(PAT_ALL);
asc_loadalign(src, src_addr); // src_addr is the Unified Buffer (UB) memory space address input from outside.
asc_e4m32float(dst, src, mask);
```
