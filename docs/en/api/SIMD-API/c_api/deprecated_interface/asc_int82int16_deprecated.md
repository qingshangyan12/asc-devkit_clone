# asc_int82int16 (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T03:14:01.282Z pushedAt=2026-10-28T12:55:35.702Z nl=2 -->

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

**The prototype of this API that controls the destination operation write position through the API suffix (including only the three parameters: source operand, destination operand, and mask register) is deprecated. Use the explicit position parameter overload of [asc_int82int16](../reg_compute/reg_convert/asc_int82int16.md) instead.**

Converts int8_t data to int16_t data.

Because the bit-width ratio between the source operand and the destination operand is 1:2, data of one VL size must be split into two parts when reading. Depending on the API, the input data at odd-indexed or even-indexed positions is selected.

## Prototype

```cpp
// Read data at even-indexed positions.
__simd_callee__ inline void asc_int82int16(vector_int16_t& dst, vector_int8_t src, vector_bool mask)
// Read data at odd-indexed positions.
__simd_callee__ inline void asc_int82int16_v2(vector_int16_t& dst, vector_int8_t src, vector_bool mask)
```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| --- | --- | --- |
| dst | Output | Destination operand (vector data register). |
| src | Input | Source operand (vector data register). |
| mask | Input | Source operand mask (mask register), which indicates the elements that participate in the computation. Elements at positions where the corresponding bit is 1 participate in the computation, and elements at positions where the corresponding bit is 0 do not. Elements of src that are not selected by mask are set to zero in the output. |

For details about vector data registers and mask registers, see [reg Data Type Definition](../defs/type/data_type_definition.md).

## Return Value

None

## Constraints

- mask controls whether the source operand participates in the computation. Elements of the source operand that do not participate in the computation are set to zero at the corresponding positions in the output.

## Examples

```cpp
vector_int16_t dst;
vector_int8_t src;
vector_bool mask = asc_create_mask_b8(PAT_ALL);
asc_loadalign(src, src_addr); // src_addr is the externally input Unified Buffer (UB) memory space address.
asc_int82int16(dst, src, mask);
```
