# asc_e5m22float (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T01:34:10.578Z pushedAt=2026-10-28T12:55:18.883Z nl=2 -->

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

**The prototype of this API without explicit position parameters is deprecated. Use the explicit position parameter overload of [asc_e5m22float](../reg_compute/reg_convert/asc_e5m22float.md) instead.**

Divides the source operand of the vector_fp8_e5m2_t type into four parts in units of 256B, reads the elements of one part, converts them to the vector_float type, and writes them to the destination operand.

- asc_e5m22float: Reads the first part.

- asc_e5m22float_v2: Reads the second part.

- asc_e5m22float_v3: Reads the third part.

- asc_e5m22float_v4: Reads the fourth part.

## Prototype

```cpp
__simd_callee__ inline void asc_e5m22float(vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask)
__simd_callee__ inline void asc_e5m22float_v2(vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask)
__simd_callee__ inline void asc_e5m22float_v3(vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask)
__simd_callee__ inline void asc_e5m22float_v4(vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask)
```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| --------- | ----- | ---------------- |
| dst       | Output    | Destination operand (vector data register).            |
| src | Input    | Source operand (vector data register).            |
| mask     | Input    | Source operand mask (mask register), which indicates the elements that participate in the computation. A value of 1 at the corresponding position indicates participation in the computation, and 0 indicates no participation. Elements not selected by mask are set to zero in the output.        |

For details about vector data registers and mask registers, see [reg data type definition](../defs/type/data_type_definition.md).

## Return Value

None

## Pipeline Type

PIPE_V

## Constraints

None

## Examples

```cpp
vector_float dst;
vector_fp8_e5m2_t src;
vector_bool mask;
asc_e5m22float(dst, src, mask);    // Convert the first part of src to the vector_float type and write it to dst.
asc_e5m22float_v2(dst, src, mask);    // Convert the second part of src to the vector_float type and write it to dst.
asc_e5m22float_v3(dst, src, mask);    // Convert the third part of src to the vector_float type and write it to dst.
asc_e5m22float_v4(dst, src, mask);    // Convert the fourth part of src to the vector_float type and write it to dst.
```
