# asc_set_gm2l1_loop2_stride

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-22T13:20:19.521Z pushedAt=2026-10-28T12:45:45.465Z nl=2 -->

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

The header file path is `"c_api/cube_datamove/cube_datamove.h"`.

When moving data from Global Memory (GM) to the L1 Buffer, sets the source and destination L1 strides of adjacent data blocks in the outer loop of the [asc_copy_gm2l1_align](asc_copy_gm2l1_align.md) Loop Fill mode.

The inner loop stride and the iteration counts of the two loops must be configured through [asc_set_gm2l1_loop1_stride](asc_set_gm2l1_loop1_stride.md) and [asc_set_gm2l1_loop_size](asc_set_gm2l1_loop_size.md), respectively.

This API takes effect only on the AIC.

## Prototype

```cpp
__aicore__ inline void asc_set_gm2l1_loop2_stride(uint64_t loop2_src_stride, uint64_t loop2_dst_stride)
```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| --- | --- | --- |
| loop2_src_stride | Input | Stride between the source operands of adjacent data blocks in the outer loop, in bytes, ranging from [0, $2^{40}-1$]. |
| loop2_dst_stride | Input | Stride between the destination L1 data blocks of adjacent iterations in the outer loop, in bytes, ranging from [0, $2^{21}-1$], and must be 32-byte aligned. |

## Return Value

None

## Pipeline Type

PIPE_S

## Constraints

- This API returns directly when it is not called by the AIC.
- The configuration is used only by the [asc_copy_gm2l1_align](asc_copy_gm2l1_align.md) Loop Fill mode; the Loop Fill mode does not support left or right padding.
- `loop2_dst_stride` must be 32-byte aligned, which takes effect even when `loop2_size` is 1.

## Examples

For a complete example of 2×2 GM-to-L1 loop data movement, L1-to-UB readback, and host-side byte-by-byte verification, see [asc_set_gm2l1_loop_size Examples](asc_set_gm2l1_loop_size.md#examples).

The following code configures the source stride of adjacent data blocks in the outer loop to 128B and the destination L1 stride to 64B:

```cpp
// loop1 executes twice, and loop2 executes twice.
asc_set_gm2l1_loop_size(2, 2);
asc_set_gm2l1_loop1_stride(64, 32);
// Outer loop: adjacent iterations are 128B apart in GM and 64B apart in L1.
asc_set_gm2l1_loop2_stride(128, 64);
asc_copy_gm2l1_align(dst, src, 1, 32, 0, 0, false, asc_load_l2_cache_mode::NOTALLOC_KEEP, 0, 32);
// Reset the loop count after the data transfer completes.
asc_set_gm2l1_loop_size(1, 1);
```
