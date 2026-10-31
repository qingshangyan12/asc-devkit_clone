# asc_uint162uint32 (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T04:50:06.669Z pushedAt=2026-10-28T12:55:49.513Z nl=2 -->

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

**The prototype of this API that controls the destination operand write position through the API suffix (including only the source operand, destination operand, and mask register parameters) is deprecated. Use the explicit position parameter overload of [asc_uint162uint32](../reg_compute/reg_convert/asc_uint162uint32.md) instead.**

Converts uint16_t data to uint32_t data.

Because the bit-width ratio of the source operand to the destination operand is 1:2, when reading data, a VL-sized data block must be split into two parts. Depending on the API, select either the odd-indexed or even-indexed positions of the input data.

## Prototype

```cpp
// Read data at even-indexed positions.
__simd_callee__ inline void asc_uint162uint32(vector_uint32_t& dst, vector_uint16_t src, vector_bool mask)    
// Read data at odd-indexed positions.
__simd_callee__ inline void asc_uint162uint32_v2(vector_uint32_t& dst, vector_uint16_t src, vector_bool mask)
```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| --- | --- | --- |
| dst | Output | Destination operand (vector data register). |
| src | Input | Source operand (vector data register). |
| mask | Input | Source operand mask (mask register), indicating which elements participate in the computation. Elements with a corresponding bit of 1 participate in the computation, and elements with a bit of 0 do not. Elements not selected by mask are set to zero in the output. |

For details about the vector data register and mask register, see [reg data type definition](../defs/type/data_type_definition.md).

## Return Value

None

## Constraints

- mask controls whether the source operand participates in the computation. Elements of the source operand that do not participate are set to zero at the corresponding positions in the output.

## Examples

```cpp
vector_uint16_t src;
vector_uint32_t dst;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
asc_loadalign(src, src_addr); // src_addr is the address of the external Unified Buffer (UB) memory space.
asc_uint162uint32(dst, src, mask);
```
