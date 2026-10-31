# asc_duplicate_merge

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-23T03:54:33.860077+00:00 pushedAt=2026-10-27T02:26:32.284Z nl=2 -->

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
<!-- npu="910" id4 -->
- Atlas training products: Not supported
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas inference products AI Core: Not supported
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas inference products Vector Core: Not supported
<!-- end id6 -->
<!-- npu="310b" id7 -->
- Atlas 200I/500 A2 inference products: Not supported
<!-- end id7 -->

## Description

The header file path is `"c_api/reg_compute/compute/reg_broadcast.h"`.

Broadcasts the lowest element of the source operand `src`, that is, the element at index 0, to the positions in the destination operand `dst` selected by `mask`. `mask` selects only element positions in `dst` and does not affect which element is read from `src`; elements in `dst` not selected by `mask` retain their original values.

This API broadcasts the same source data as [asc_duplicate](asc_duplicate.md), but uses merge mode to retain the original values of elements in `dst` that do not participate in the computation.

This API is a Reg vector computation API and takes effect only on AIV.

## Prototype

```cpp
// Placeholder form.
__simd_callee__ inline void asc_duplicate_merge(vector_<dtype>& dst,
                                                vector_<dtype> src,
                                                vector_bool mask)
```

**Placeholder description:**

- `<dtype>` values are: `int8_t`, `uint8_t`, `fp8_e8m0_t`, `fp8_e5m2_t`, `fp8_e4m3fn_t`, `int16_t`, `uint16_t`, `half`, `bfloat16_t`, `int32_t`, `uint32_t`, and `float`.

### Typical Prototype Example

```cpp
__simd_callee__ inline void asc_duplicate_merge(vector_float& dst,
                                                vector_float src,
                                                vector_bool mask)
```

## Parameters

**Table 1** Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| dst | Input/output | Destination operand (vector data register). Elements selected by mask are set to the broadcast value; unselected elements retain their values from before the API call. The data type must be the same as that of src. |
| src | Input | Source operand (vector data register), whose lowest element is the data to be broadcast. The data type must be consistent with dst. |
| mask | Input | Validity indicator for destination operand element operations (mask register). When the corresponding bit is 1, the element in dst is set to the lowest element of src; when it is 0, the element in dst retains its original value. |

For details about vector data registers and mask registers, see [Register Type Definition](../../defs/type/data_type_definition.md).

## Return Value

None

## Pipeline Type

PIPE_V

## Constraints

- For the general constraints on Reg vector computation C APIs, see [General Constraints](../overview.md#general-constraints).
- `mask` selects only the positions in `dst` to which the broadcast value is written, rather than elements in `src`. Regardless of whether the lowest bit of `mask` is valid, the data to broadcast is always the lowest element of `src`.
- Initialize `dst` before calling the API; otherwise, the values of elements not selected by `mask` are undefined.
- The hardware preserves the order of data dependencies involving the same register, so no additional synchronization instructions are required. If this API and preceding or subsequent Reg data movement APIs use different registers to access the same UB address and have read-after-write or write-after-write dependencies, call [asc_mem_bar](../reg_sync/asc_mem_bar.md) for synchronization.

## Examples

The following example broadcasts the lowest element of `src` to the first 4 elements of `dst`. The remaining elements in `dst` retain their original values.

```cpp
__simd_vf__ inline void duplicate_merge(__ubuf__ float* dst, __ubuf__ float* src)
{
    vector_float dst_reg;
    vector_float src_reg;
    uint32_t active_count = 4;
    uint32_t full_count = 64;
    vector_bool active_mask = asc_update_mask_b32(active_count);
    vector_bool full_mask = asc_update_mask_b32(full_count);

    asc_loadalign(dst_reg, dst);
    asc_loadalign(src_reg, src);
    asc_duplicate_merge(dst_reg, src_reg, active_mask);
    asc_storealign(dst, dst_reg, full_mask);
}
```
