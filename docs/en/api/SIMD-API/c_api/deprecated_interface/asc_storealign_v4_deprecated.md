# asc_storealign_pack_postupdate_v2 (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T04:44:02.804Z pushedAt=2026-10-28T12:55:49.503Z nl=2 -->

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

The header file path is `"c_api/reg_compute/store/storealign.h"`.

**This API is deprecated. Use [asc_storealign_pack_quarter_postupdate](../reg_compute/store/asc_storealign_pack_quarter_postupdate.md) instead.**

Compresses the low 8 bits of the valid 32-bit elements indicated by `mask` in the vector data register and moves them out to the Unified Buffer (UB). This API passes the offset through the `int32_t`-typed `offset` parameter and enables Post Update mode to automatically update the destination operand address.

## Prototype

```cpp
__simd_callee__ inline void asc_storealign_pack_postupdate_v2(__ubuf__ int32_t*& dst_align32b, vector_int32_t src, int32_t offset, vector_bool mask)
__simd_callee__ inline void asc_storealign_pack_postupdate_v2(__ubuf__ uint32_t*& dst_align32b, vector_uint32_t src, int32_t offset, vector_bool mask)
__simd_callee__ inline void asc_storealign_pack_postupdate_v2(__ubuf__ float*& dst_align32b, vector_float src, int32_t offset, vector_bool mask)
```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| --- | --- | --- |
| dst_align32b | Input/Output | Starting address of the destination operand, passed by pointer reference. The starting address must be 32-byte aligned. After the move-out completes, the hardware automatically updates this pointer. |
| src | Input | Source operand (vector data register). Supported data types are `int32_t`, `uint32_t`, and `float`. |
| offset | Input | Destination address update amount, of type `int32_t`, in elements. After the API executes, the destination address is incremented by `offset × sizeof(dtype)` bytes. |
| mask | Input | Source operand mask (mask register), indicating the elements that participate in the move-out. A value of 1 at the corresponding position means the element participates; 0 means it does not. |

For details about the vector data register and mask register, see [reg data type definition](../defs/type/data_type_definition.md).

## Return Value

None

## Pipeline Type

PIPE_V

## Constraints

- The starting address of `dst_align32b` must be 32-byte aligned.
- The new `dst_align32b` address after Post Update must still be 32-byte aligned.
- `mask` must be pre-assigned through the mask setting API before being passed in.

## Examples

```cpp
__ubuf__ int32_t* dst_align32b = (__ubuf__ int32_t*)asc_get_phy_buf_addr(0);
vector_int32_t src;
int32_t offset = 64;
vector_bool mask = asc_create_mask_b32(PAT_ALL);
asc_storealign_pack_postupdate_v2(dst_align32b, src, offset, mask);
```
