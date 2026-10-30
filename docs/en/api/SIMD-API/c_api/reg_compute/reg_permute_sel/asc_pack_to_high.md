# asc_pack_to_high

<!-- md-trans-meta sourceCommit=10589b6e0347a8628628193466fcae403c924d16 translatedAt=2026-10-23T03:54:34.048923+00:00 pushedAt=2026-10-27T02:26:33.060Z nl=2 -->

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
- Atlas inference products AI Core: Not supported
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas inference products Vector Core: Not supported
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas training products: Not supported
<!-- end id7 -->

## Description

The header file path is `"c_api/reg_compute/compute/reg_permute_sel.h"`.

Packs the lower half of the bits in each source operand element and writes the packed bits sequentially to the upper half of the destination operand, setting the lower half to 0. For b16 source operands, the low 8 bits of each element are selected; for b32 source operands, the low 16 bits are selected. For mask registers, the even-indexed bits of the source operand are selected and written sequentially to the upper half of the destination operand.

The result can be output through a reference parameter or the function return value. Unlike [asc_pack_to_low](asc_pack_to_low.md), this API writes the packed result to the upper half of the destination operand.

**Figure 1** asc_pack_to_high packing process

![asc_pack_to_high packing process](../../../../figures/pack_high_draft.png)

This API is a Reg vector computation API and takes effect only on AIV.

## Prototype

```cpp
// Output the result through a reference parameter (placeholder form).
__simd_callee__ inline void asc_pack_to_high(vector_<dtype_dst>& dst,
                                             vector_<dtype_src> src)

// Return the result through the function return value (placeholder form).
__simd_callee__ inline vector_<dtype_dst> asc_pack_to_high(vector_<dtype_src> src)
```

**Placeholder description:**

- `<dtype_src>` can be `bool`, `int16_t`, `uint16_t`, `int32_t`, or `uint32_t`.
- `<dtype_dst>` is determined by `<dtype_src>`: `bool` corresponds to `bool`, `int16_t` and `uint16_t` correspond to `uint8_t`, and `int32_t` and `uint32_t` correspond to `uint16_t`.

### Typical Prototype Example

```cpp
// Output the result through a reference parameter.
__simd_callee__ inline void asc_pack_to_high(vector_uint16_t& dst,
                                             vector_uint32_t src)

// Return the result through the function return value.
__simd_callee__ inline vector_uint16_t asc_pack_to_high(vector_uint32_t src)
```

## Parameters

**Table 1** Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| dst | Output | Destination operand, which is a vector data register or a mask register. This parameter is included only in function prototypes that output the result through a reference parameter. The upper half holds the packed result, and the lower half is set to 0. The data type must match src according to the mapping specified in the function prototype. |
| src | Input | Source operand, which is a vector data register or a mask register. For vector data registers, the lower half of the bits in each element is extracted; for mask registers, the even-indexed bits are extracted. |

For details about vector data registers and mask registers, see [Register Type Definition](../../defs/type/data_type_definition.md).

## Return Value

- The function prototype that outputs the result through a reference parameter has no return value, and the compressed result is written to `dst`.
- Function prototypes that output the result through the function return value return a vector data register or a mask register holding the packed result, of type `vector_<dtype_dst>` corresponding to `dtype_dst`.

## Pipeline Type

PIPE_V

## Constraints

- For the general constraints on Reg vector computation C APIs, see [General Constraints](../overview.md#general-constraints).
- When a vector data register is packed, only the lower half of the bits in each source element is retained. The upper half is discarded, and no saturation is performed.
- The hardware preserves the order of data dependencies involving the same register, so no additional synchronization instructions are required. If this API and preceding or subsequent Reg data movement APIs use different registers to access the same UB address and have read-after-write or write-after-write dependencies, call [asc_mem_bar](../reg_sync/asc_mem_bar.md) for synchronization.

## Examples

The following example extracts the low 16 bits of each element in the `vector_uint32_t` source operand and writes the result to the upper half of the `vector_uint16_t` destination operand through the function return value.

```cpp
__simd_vf__ inline void pack_to_high(__ubuf__ uint16_t* dst, __ubuf__ uint32_t* src)
{
    vector_uint32_t src_reg;
    asc_loadalign(src_reg, src);
    vector_uint16_t dst_reg = asc_pack_to_high(src_reg);

    vector_bool mask = asc_create_mask_b16(PAT_ALL);
    asc_storealign(dst, dst_reg, mask);
}
```
